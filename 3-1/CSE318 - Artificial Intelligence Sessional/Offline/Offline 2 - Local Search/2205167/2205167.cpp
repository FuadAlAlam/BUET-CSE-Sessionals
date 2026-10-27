#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<numeric>
#include<string>
#include<map>
#include<tuple>

using namespace std;


struct Graph {
    int n, m;
    vector<vector<pair<int, int>>> adj;
    vector<tuple<int, int, int>> edges;
    
    Graph(int vertices, int edges){
        n = vertices;
        m = edges;
        adj.resize(n+1);
    }

    void addEdge(int u, int v, int weight){
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
        edges.push_back({u, v, weight});
    }
};


long long calculateCutWeight(const Graph& G, const vector<int>& partition){
    long long weight = 0;
    for(const auto& edge: G.edges){
        if(partition[get<0>(edge)] != partition[get<1>(edge)]){
            weight += get<2>(edge);
        }
    }
    return weight;
}

double RandomizedMaxCut(const Graph& G, int iterations){
    long long totalCutWeight = 0;
    for(int iter=0; iter < iterations; ++iter){
        vector<int> partition(G.n+1, 0); 
        for(int v=1; v <= G.n; ++v){
            partition[v] = (rand()%2 == 0) ? 0: 1;
        }
        totalCutWeight += calculateCutWeight(G, partition);
    }
    return (double)totalCutWeight/iterations; 
}


long long GreedyMaxCut(const Graph& G, vector<int>& best_partition){
    best_partition.assign(G.n+1, -1); 
    int max_u = 1, max_v = 2, max_w = -1;
    
    for(const auto& edge: G.edges){
        if(get<2>(edge) > max_w){
            max_w = get<2>(edge);
            max_u = get<0>(edge);
            max_v = get<1>(edge);
        }
    }
    
    best_partition[max_u] = 0; 
    best_partition[max_v] = 1; 
    
    vector<long long> conn0(G.n+1, 0);
    vector<long long> conn1(G.n+1, 0);

    for(const auto& neighbor: G.adj[max_u]) conn0[neighbor.first] += neighbor.second;
    for(const auto& neighbor: G.adj[max_v]) conn1[neighbor.first] += neighbor.second;

    for(int z=1; z <= G.n; ++z){
        if(best_partition[z] != -1) continue; 
        
        long long wX = conn1[z];
        long long wY = conn0[z];
        
        int assigned_part = (wX > wY) ? 0 : 1;
        best_partition[z] = assigned_part;

        for(const auto& neighbor: G.adj[z]){
            if(best_partition[neighbor.first] == -1){
                if(assigned_part == 0) conn0[neighbor.first] += neighbor.second;
                else conn1[neighbor.first] += neighbor.second;
            }
        }
    }
    return calculateCutWeight(G, best_partition);
}


long long SemiGreedyMaxCut(const Graph& G, double alpha, vector<int>& final_partition){
    vector<int> partition(G.n+1, -1);
    vector<int> unassigned;
    for(int i=1; i <= G.n; ++i) unassigned.push_back(i);
    
    vector<long long> conn0(G.n+1, 0);
    vector<long long> conn1(G.n+1, 0);

    while(!unassigned.empty()){
        long long w_min = -1, w_max = -1;
        vector<pair<int, long long>> greedy_values;
        greedy_values.reserve(unassigned.size());
        
        for(int v: unassigned){
            long long sigma_X = conn0[v]; 
            long long sigma_Y = conn1[v]; 
            
            long long greedy_val = max(sigma_X, sigma_Y);
            greedy_values.push_back({v, greedy_val});
            
            if(w_min == -1 || sigma_X < w_min) w_min = sigma_X;
            if(sigma_Y < w_min) w_min = sigma_Y;
            if(w_max == -1 || sigma_X > w_max) w_max = sigma_X;
            if(sigma_Y > w_max) w_max = sigma_Y;
        }
        
        double mu = w_min+alpha*(w_max-w_min);
        vector<int> RCL;
        for(const auto& p: greedy_values){
            if(p.second >= mu) RCL.push_back(p.first);
        }
        
        int v_star = RCL[rand()%RCL.size()];
        int assigned_part = (conn1[v_star] >= conn0[v_star]) ? 0: 1;
        partition[v_star] = assigned_part; 
        
        unassigned.erase(remove(unassigned.begin(), unassigned.end(), v_star), unassigned.end());

        for(const auto& neighbor: G.adj[v_star]){
            if(partition[neighbor.first] == -1){
                if(assigned_part == 0) conn0[neighbor.first] += neighbor.second;
                else conn1[neighbor.first] += neighbor.second;
            }
        }
    }
    
    final_partition = partition;
    return calculateCutWeight(G, partition);
}


pair<long long, int> LocalSearch(const Graph& G, vector<int>& partition){
    long long current_weight = calculateCutWeight(G, partition);
    int iterations = 0;
    
    vector<long long> sigma_S(G.n+1, 0);
    vector<long long> sigma_S_bar(G.n+1, 0);

    for(int v=1; v <= G.n; ++v){
        for(const auto& neighbor: G.adj[v]){
            int u = neighbor.first;
            int w = neighbor.second;
            if(partition[u] == partition[v]) sigma_S[v] += w;
            else sigma_S_bar[v] += w;
        }
    }

    bool improvement = true;
    while(improvement){
        improvement = false;
        long long best_delta = 0;
        int best_v = -1;
        
        for(int v=1; v <= G.n; ++v){
            long long delta = sigma_S[v]-sigma_S_bar[v]; 
            if(delta > best_delta){
                best_delta = delta;
                best_v = v;
            }
        }
        
        if(best_delta > 0){
            partition[best_v] = 1-partition[best_v]; 
            current_weight += best_delta;
            improvement = true;
            iterations++;

            for(const auto& neighbor: G.adj[best_v]){
                int u = neighbor.first;
                int w = neighbor.second;
                
                if(partition[u] == partition[best_v]){
                    sigma_S[u] += w;
                    sigma_S_bar[u] -= w;
                }
                else{
                    sigma_S_bar[u] += w;
                    sigma_S[u] -= w;
                }
            }
 
            swap(sigma_S[best_v], sigma_S_bar[best_v]);
        }
    }
    return {current_weight, iterations}; 
}

pair<long long, int> GRASP(const Graph& G, int MaxIterations, double alpha){
    long long best_weight = -1;
    vector<int> best_partition;
    
    for(int i = 1; i <= MaxIterations; ++i){
        vector<int> current_partition;
        SemiGreedyMaxCut(G, alpha, current_partition);
        pair<long long, int> ls_res = LocalSearch(G, current_partition);
        
        if(i == 1 || ls_res.first > best_weight){
            best_weight = ls_res.first;
            best_partition = current_partition;
        }
    }
    return {best_weight, MaxIterations};
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    string dir_path = "graph_GRASP/set1/";
    if(argc > 1){
        dir_path = argv[1];
        if(dir_path.back() != '/' && dir_path.back() != '\\'){
            dir_path += "/";
        }
    }
    
    map<string, string> known_best = {
        {"G1", "12078"}, {"G2", "12084"}, {"G3", "12077"}, {"G11", "627"}, 
        {"G12", "621"}, {"G13", "645"}, {"G14", "3187"}, {"G15", "3169"}, 
        {"G16", "3172"}, {"G22", "14123"}, {"G23", "14129"}, {"G24", "14131"}, 
        {"G32", "1560"}, {"G33", "1537"}, {"G34", "1541"}, {"G35", "8000"}, 
        {"G36", "7996"}, {"G37", "8009"}, {"G43", "7027"}, {"G44", "7022"}, 
        {"G45", "7020"}, {"G48", "6000"}, {"G49", "6000"}, {"G50", "5988"}
    };

    ofstream csv("2205167.csv");
    csv << "Problem,Problem,Problem,Constructive algorithm,Constructive algorithm,Constructive algorithm,Local search,Local search,GRASP,GRASP,Known best solution or upper bound\n";
    csv << "Name,|V| or n,|E| or m,Simple Randomized,Simple Greedy,Semi-greedy-1 (alpha=0.8),No. of iterations,Average value,No. of iterations,Best value,\n";
    
    cout << "Starting MAX-CUT Heuristic Benchmarking..." << endl;
    cout << "------------------------------------------" << endl;

    for(int i = 1; i <= 54; ++i){
        string graph_name = "G"+to_string(i);
        string filename = dir_path+"g"+to_string(i)+".rud";
        
        ifstream infile(filename);
        if(!infile){
            cout << "Skipping " << filename << " (File not found)" << endl;
            continue; 
        }
        
        int n, m;
        if(!(infile >> n >> m)) continue;
        
        Graph G(n, m);
        int u, v, w;
        for(int e = 0; e < m; ++e){
            infile >> u >> v >> w;
            G.addEdge(u, v, w);
        }
        infile.close();
        
        cout << "Processing " << graph_name << " (|V|=" << n << ", |E|=" << m << ")..." << flush;
        
        double rand_res = RandomizedMaxCut(G, 10);
        
        vector<int> greedy_part;
        long long greedy_res = GreedyMaxCut(G, greedy_part);
        
        vector<int> sg_part;
        double alpha = 0.8; 
        long long sg_res = SemiGreedyMaxCut(G, alpha, sg_part);
        
        vector<int> ls_part = greedy_part;
        pair<long long, int> ls_res = LocalSearch(G, ls_part);
        
        pair<long long, int> grasp_res = GRASP(G, 100, alpha);
        
        string best_known = known_best.count(graph_name) ? known_best[graph_name]: "";
        
        csv << graph_name << "," 
            << n << "," 
            << m << "," 
            << rand_res << "," 
            << greedy_res << "," 
            << sg_res << "," 
            << ls_res.second << "," 
            << ls_res.first << "," 
            << grasp_res.second << "," 
            << grasp_res.first << "," 
            << best_known << "\n";

        cout << " Done!\n";
        cout << "  > Randomized:  " << rand_res << "\n";
        cout << "  > Greedy:      " << greedy_res << "\n";
        cout << "  > Semi-Greedy: " << sg_res << "\n";
        cout << "  > Local Search:" << ls_res.first << " (Iters: " << ls_res.second << ")\n";
        cout << "  > GRASP:       " << grasp_res.first << " (Iters: " << grasp_res.second << ")\n";
        cout << "------------------------------------------" << endl;
    }
    
    csv.close();
    cout << "\nSuccess! The file 2205167.csv has been generated." << endl;
    return 0;
}