#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<chrono>
#include<random>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

using namespace std;

int N, M, x, y;
int total_units;

const int TIME_SCALE = 1000;

pthread_mutex_t log_lock;
chrono::high_resolution_clock::time_point start_time;

long long get_time(){
    auto now = chrono::high_resolution_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(now-start_time).count();
    return elapsed_ms/TIME_SCALE;
}

void print_log(const string& str){
    pthread_mutex_lock(&log_lock);
    cout<<str<<endl;
    pthread_mutex_unlock(&log_lock);
}

void sleep_units(int units){
    if(units > 0){
        usleep(units*TIME_SCALE*1000);
    }
}

random_device rd;
mt19937 gen(rd());

int get_poisson(double lambda){
    poisson_distribution<int> dist(lambda);
    return dist(gen);
}

sem_t station_sem[5];

struct Unit{
    int id;
    int leader_id;
    int docs_count;
    pthread_mutex_t lock;
    sem_t full_sem;
};

vector<Unit> units;

int operations_completed = 0;
int read_count = 0;
sem_t rw_mutex;
sem_t read_count_mutex;

bool finished = false;
pthread_mutex_t finish_lock;

struct Operative{
    int id;
    int unit_id;
    bool is_leader;
};

void* operative_runner(void* arg){
    Operative* op = (Operative*)arg;
    int id = op->id;
    int u_idx = op->unit_id-1;
    bool is_leader = op->is_leader;

    int arrival_delay = get_poisson(5.0)+1;
    sleep_units(arrival_delay);

    int station_id = (id % 4)+1;

    print_log("Operative "+to_string(id)+" has arrived at typewriting station at time "+to_string(get_time()));

    sem_wait(&station_sem[station_id]);

    sleep_units(x);

    print_log("Operative "+to_string(id)+" has completed document recreation at time "+to_string(get_time()));

    sem_post(&station_sem[station_id]);

    pthread_mutex_lock(&units[u_idx].lock);
    units[u_idx].docs_count++;
    if(units[u_idx].docs_count == M){
        sem_post(&units[u_idx].full_sem);
    }
    pthread_mutex_unlock(&units[u_idx].lock);

    if(is_leader){
        sem_wait(&units[u_idx].full_sem);

        print_log("Unit "+to_string(op->unit_id)+" has completed document recreation phase at time "+to_string(get_time()));

        sem_wait(&rw_mutex);

        sleep_units(y);

        operations_completed++;

        sem_post(&rw_mutex);

        print_log("Unit "+to_string(op->unit_id)+" has completed intelligence distribution at time "+to_string(get_time()));
    }

    return NULL;
}

void* staff_runner(void* arg){
    int staff_id = *(int*)arg;

    int start_delay = (staff_id == 1) ? 1 : 3;
    sleep_units(start_delay);

    while(true){
        sem_wait(&read_count_mutex);
        read_count++;
        if(read_count == 1){
            sem_wait(&rw_mutex);
        }
        sem_post(&read_count_mutex);

        int completed_now = operations_completed;
        print_log("Intelligence Staff "+to_string(staff_id) +
                  " began reviewing logbook at time "+to_string(get_time()) +
                  ". Operations completed = "+to_string(completed_now));

        int review_time = get_poisson(1.0)+1;
        sleep_units(review_time);

        sem_wait(&read_count_mutex);
        read_count--;
        if(read_count == 0){
            sem_post(&rw_mutex);
        }
        sem_post(&read_count_mutex);

        pthread_mutex_lock(&finish_lock);
        if(finished && operations_completed >= total_units){
            pthread_mutex_unlock(&finish_lock);
            break;
        }
        pthread_mutex_unlock(&finish_lock);

        int next_interval = get_poisson(4.0)+2;
        sleep_units(next_interval);

        pthread_mutex_lock(&finish_lock);
        if(finished && operations_completed >= total_units){
            pthread_mutex_unlock(&finish_lock);
            break;
        }
        pthread_mutex_unlock(&finish_lock);
    }

    return NULL;
}

int main(int argc, char* argv[]){
    ifstream in_file;
    ofstream out_file;
    streambuf* old_cin = nullptr;
    streambuf* old_cout = nullptr;

    if(argc >= 2){
        in_file.open(argv[1]);
        if(in_file.is_open()){
            old_cin = cin.rdbuf();
            cin.rdbuf(in_file.rdbuf());
        }
    }
    if(argc >= 3){
        out_file.open(argv[2]);
        if(out_file.is_open()){
            old_cout = cout.rdbuf();
            cout.rdbuf(out_file.rdbuf());
        }
    }

    if(!(cin >> N >> M >> x >> y)){
        return 1;
    }

    total_units = N/M;

    pthread_mutex_init(&log_lock, NULL);
    pthread_mutex_init(&finish_lock, NULL);

    for(int i = 1; i<= 4; i++){
        sem_init(&station_sem[i], 0, 1);
    }

    sem_init(&rw_mutex, 0, 1);
    sem_init(&read_count_mutex, 0, 1);

    units.resize(total_units);
    for(int i = 0; i<total_units; i++){
        units[i].id = i+1;
        units[i].leader_id = (i+1)*M;
        units[i].docs_count = 0;
        pthread_mutex_init(&units[i].lock, NULL);
        sem_init(&units[i].full_sem, 0, 0);
    }

    start_time = chrono::high_resolution_clock::now();

    pthread_t staff_threads[2];
    int staff_ids[2] ={1, 2};
    for(int i = 0; i<2; i++){
        pthread_create(&staff_threads[i], NULL, staff_runner, &staff_ids[i]);
    }

    pthread_t op_threads[N];
    vector<Operative> op_list(N);

    for(int i = 0; i<N; i++){
        int op_id = i+1;
        int u_id = (i/M)+1;
        bool is_ldr = (op_id == u_id*M);

        op_list[i] ={op_id, u_id, is_ldr};
        pthread_create(&op_threads[i], NULL, operative_runner, &op_list[i]);
    }

    for(int i = 0; i<N; i++){
        pthread_join(op_threads[i], NULL);
    }

    pthread_mutex_lock(&finish_lock);
    finished = true;
    pthread_mutex_unlock(&finish_lock);

    for(int i = 0; i<2; i++){
        pthread_join(staff_threads[i], NULL);
    }

    for(int i = 1; i<= 4; i++){
        sem_destroy(&station_sem[i]);
    }
    sem_destroy(&rw_mutex);
    sem_destroy(&read_count_mutex);

    for(int i = 0; i<total_units; i++){
        pthread_mutex_destroy(&units[i].lock);
        sem_destroy(&units[i].full_sem);
    }
    pthread_mutex_destroy(&log_lock);
    pthread_mutex_destroy(&finish_lock);

    if(old_cin) cin.rdbuf(old_cin);
    if(old_cout) cout.rdbuf(old_cout);

    return 0;
}
