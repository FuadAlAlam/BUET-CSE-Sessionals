#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iomanip>
#include <cmath>
#include <set>

using namespace std;
using namespace std::chrono;

enum Color{RED, BLACK};
enum collision{CHAINING, LINEAR_PROBING, DOUBLE_HASHING};

struct result{
    int collisions;
    double avg_time_before;
    double avg_probes_before;
    double avg_time_after;
    double avg_probes_after;
};

struct Node{
    string key;
    int value;
    Color color;
    Node *left, *right, *parent;

    Node(string key, int value){
        this->key = key;
        this->value = value;
        color = RED;
        left = right = parent = nullptr;
    }
};

class RedBlackTree{
    Node *root;
    Node *NIL;

    void leftRotate(Node *x){
        Node *y = x->right;
        x->right = y->left;
        if(y->left != NIL){
            y->left->parent = x;
        }
        y->parent = x->parent;
        if(x->parent == nullptr){
            root = y;
        }
        else if(x == x->parent->left){
            x->parent->left = y;
        }
        else{
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *y){
        Node *x = y->left;
        y->left = x->right;
        if(x->right != NIL){
            x->right->parent = y;
        }
        x->parent = y->parent;
        if(y->parent == nullptr){
            root = x;
        }
        else if(y == y->parent->right){
            y->parent->right = x;
        }
        else{
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertFix(Node *z){
        while(z->parent != nullptr && z->parent->color == RED){
            if(z->parent == z->parent->parent->left){
                Node *y = z->parent->parent->right;
                if(y->color == RED){
                    z->parent->color = y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->right){
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else{
                Node *y = z->parent->parent->left;
                if(y->color == RED){
                    z->parent->color = y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->left){
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node *minVal(Node *x){
        while(x->left != NIL){
            x = x->left;
        }
        return x;
    }

    void transplant(Node *u, Node *v){
        if(u->parent == nullptr){
            root = v;
        }
        else if(u == u->parent->left){
            u->parent->left = v;
        }
        else{
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteFix(Node *x){
        while(x != root && x->color == BLACK){
            if(x == x->parent->left){
                Node *w = x->parent->right;
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if(w->left->color == BLACK && w->right->color == BLACK){
                    w->color = RED;
                    x = x->parent;
                }
                else{
                    if(w->right->color == BLACK){
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else{
                Node *w = x->parent->left;
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if(w->left->color == BLACK && w->right->color == BLACK){
                    w->color = RED;
                    x = x->parent;
                }
                else{
                    if(w->left->color == BLACK){
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void clearHelper(Node *v) {
        if(v == NIL) return;
        clearHelper(v->left);
        clearHelper(v->right);
        delete v;
    }

public:
    RedBlackTree(){
        NIL = new Node("", 0);
        NIL->color = BLACK;
        root = NIL;
    }

    ~RedBlackTree(){
        clearHelper(root);
        delete NIL;
    }

    bool insert(string key, int value){
        Node *z = new Node(key, value);
        Node *y = nullptr;
        Node *x = root;

        while(x != NIL){
            y = x;
            if(z->key == x->key){
                delete z;
                return false;
            }
            else if(z->key < x->key){
                x = x->left;
            }
            else{
                x = x->right;
            }
        }

        z->parent = y;
        if(y == nullptr){
            root = z;
        }
        else if(z->key < y->key){
            y->left = z;
        }
        else{
            y->right = z;
        }

        z->left = z->right = NIL;
        z->color = RED;

        insertFix(z);
        return true;
    }

    Node *search(string key){
        Node *v = root;
        while(v != NIL && v->key != key){
            if(key < v->key){
                v = v->left;
            }
            else{
                v = v->right;
            }
        }
        if(v == NIL) return nullptr;
        return v;
    }

    void deleteNode(string key){
    	Node *z = search(key);
    	if(z == nullptr) return;

        Node *y = z;
        Node *x;
        Color original_color = y->color;

        if(z->left == NIL){
            x = z->right;
            transplant(z, z->right);
        }
        else if(z->right == NIL){
            x = z->left;
            transplant(z, z->left);
        }
        else{
            y = minVal(z->right);
            original_color = y->color;
            x = y->right;
            if(y->parent == z){
                x->parent = y;
            }
            else{
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if(original_color == BLACK){
            deleteFix(x);
        }
    }

    void clear(){
        clearHelper(root);
        root = NIL;
    }

    bool empty(){
        return root == NIL;
    }
};

class VaultXHashTable{
    int tableSize, currentSize, nextVal, stepSize;

    vector<RedBlackTree*> chainTable;
    vector<pair<string, int>> openTable;
    vector<bool> deleted;

    int collisions, totalProbes, searchCnt;
    int BASE1, BASE2, MOD;

    bool isPrime(int n){
        if(n==1) return false;

        for(int i=2; i*i<=n; i++){
            if(n%i==0) return false;
        }
        return true;
    }

    int generateRandomPrimeBase(){
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        const vector<int> primes = {29, 31, 37, 41};
        uniform_int_distribution<int> dist(0, primes.size()-1);
        return primes[dist(rng)];
    }

    int hash1(const string& key) {
        long long hsh = 0;
        long long po = 1;
        
        for(char c:key){
            int val = c-'a'+1;
            hsh = (hsh+1LL*val*po)%MOD;
            po = (po*BASE1)%MOD;
        }
        return hsh;
    }

    int hash2(const string& key) {
        long long hsh = 0;
        long long po = 1;
        
        for(char c:key){
            int val = c-'a'+1;
            hsh = (hsh+1LL*val*po)%MOD;
            po = (po*BASE2)%MOD;
        }
        return hsh;
    }

    int nextPrime(int n){
        while(!isPrime(n)) n++;
        return n;
    }

public:
    VaultXHashTable(int size){
        tableSize = nextPrime(size);
        currentSize = 0;
        nextVal = 1;
        stepSize = 5;

        chainTable.resize(tableSize);
        for (int i=0; i<tableSize; i++){
            chainTable[i] = new RedBlackTree();
        }
        openTable.resize(tableSize, {"", 0});
        deleted.resize(tableSize, false);

        collisions = 0;
        totalProbes = 0;
        searchCnt = 0;

        BASE1 = generateRandomPrimeBase();
        BASE2 = generateRandomPrimeBase();
        MOD = tableSize;

        while(BASE1 == BASE2){
            BASE2 = generateRandomPrimeBase();
        }
    }

    ~VaultXHashTable(){
        for(auto* rbt:chainTable){
            delete rbt;
        }
    }

    void reset(){
        collisions = 0;
        totalProbes = 0;
        searchCnt = 0;
    }

    void clear(){
        currentSize = 0;
        nextVal = 1;
        for(auto* rbt:chainTable){
            delete rbt;
        }
        chainTable.clear();
        chainTable.resize(tableSize);
        for(int i=0; i<tableSize; i++){
            chainTable[i] = new RedBlackTree();
        }
        openTable.assign(tableSize, {"", 0});
        deleted.assign(tableSize, false);
    }

    bool insertChaining(const string& key, int hshFunc){
        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        if(chainTable[idx]->search(key) != nullptr){
            return false;
        }

        if(!chainTable[idx]->empty()){
            collisions++;
        }
        chainTable[idx]->insert(key, nextVal);
        nextVal++;
        currentSize++;
        return true;
    }

    bool searchChaining(const string& key, int hshFunc){
        searchCnt++;

        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        return chainTable[idx]->search(key) != nullptr;
    }

    void deleteChaining(const string& key, int hshFunc){
        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        chainTable[idx]->deleteNode(key);
        currentSize--;
    }

    bool insertLinearProbing(const string& key, int hshFunc){
        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        int temp = idx;
        int probeCnt = 0;

        while(openTable[temp].first != "" && !deleted[temp]){
            if(openTable[temp].first == key){
                return false;
            }
            probeCnt++;
            temp = (idx+probeCnt*stepSize)%tableSize;
            if(temp == idx) break;
        }

        collisions+=probeCnt;

        openTable[temp] = {key, nextVal};
        nextVal++;
        deleted[temp] = false;
        currentSize++;
        return true;
    }

    bool searchLinearProbing(const string& key, int hshFunc){
        searchCnt++;

        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        int temp = idx;
        int probeCnt = 0;

        while(openTable[temp].first != "" || deleted[temp]){
            if(openTable[temp].first == key && !deleted[temp]){
                totalProbes += (probeCnt+1);
                return true;
            }
            probeCnt++;
            temp = (idx+probeCnt*stepSize)%tableSize;
            if(temp == idx) break;
        }

        totalProbes += (probeCnt+1);
        return false;
    }

    void deleteLinearProbing(const string& key, int hshFunc){
        int idx;
        if(hshFunc==1){
            idx = hash1(key);
        }
        else{
            idx = hash2(key);
        }

        int temp = idx;
        int probeCnt = 0;

        while(openTable[temp].first != "" || deleted[temp]){
            if(openTable[temp].first == key && !deleted[temp]){
                deleted[temp] = true;
                currentSize--;
                return;
            }
            probeCnt++;
            temp = (idx+probeCnt*stepSize)%tableSize;
            if(temp == idx) break;
        }
    }

    bool insertDoubleHashing(const string& key, int hshFunc){
        int h1 = hash1(key);
        int h2 = hash2(key);
        if(hshFunc==2){
            swap(h1, h2);
        }

        int idx = h1;
        int probeCnt = 0;

        while(openTable[idx].first != "" && !deleted[idx]){
            if(openTable[idx].first == key){
                return false;
            }
            probeCnt++;
            idx = (h1+probeCnt*h2)%tableSize;
            if(probeCnt >= tableSize) break;
        }

        collisions+=probeCnt;

        openTable[idx] = {key, nextVal};
        nextVal++;
        deleted[idx] = false;
        currentSize++;
        return true;
    }

    bool searchDoubleHashing(const string& key, int hshFunc){
        searchCnt++;

        int h1 = hash1(key);
        int h2 = hash2(key);
        if(hshFunc==2){
            swap(h1, h2);
        }

        int idx = h1;
        int probeCnt = 0;

        while(openTable[idx].first != "" || deleted[idx]){
            if(openTable[idx].first == key && !deleted[idx]){
                totalProbes += (probeCnt+1);
                return true;
            }
            probeCnt++;
            idx = (h1+probeCnt*h2)%tableSize;
            if(probeCnt>=tableSize) break;
        }

        totalProbes += (probeCnt+1);
        return false;
    }

    void deleteDoubleHashing(const string& key, int hshFunc){
        int h1 = hash1(key);
        int h2 = hash2(key);
        if(hshFunc==2){
            swap(h1, h2);
        }

        int idx = h1;
        int probeCnt = 0;

        while(openTable[idx].first != "" || deleted[idx]){
            if(openTable[idx].first == key && !deleted[idx]){
                deleted[idx] = true;
                currentSize--;
                return;
            }
            probeCnt++;
            idx = (h1+probeCnt*h2)%tableSize;
            if(probeCnt >= tableSize) break;
        }
    }

    int getCollisions() const{
        return collisions;
    }
    double getAverageProbes() const{
        if(searchCnt == 0) return 0.0;
        return (double)totalProbes/searchCnt; 
    }
    int getTableSize() const{
        return tableSize;
    }
    int getCurrentSize() const{
        return currentSize;
    }
};

void print_table_header(){
    cout << left << setw(20) << "Method" 
         << setw(15) << "Collisions" << setw(20) << "Avg Time (μs)" << setw(20) << "Avg Probes" 
         << setw(20) << "Post-Del Time" << setw(20) << "Post-Del Probes" 
         << setw(15) << "Collisions" << setw(20) << "Avg Time (μs)" << setw(20) << "Avg Probes" 
         << setw(20) << "Post-Del Time" << setw(20) << "Post-Del Probes" << endl;
}

void print_table_row(const string& method, 
                    int col1, double time1, double probes1, double post_time1, double post_probes1,
                    int col2, double time2, double probes2, double post_time2, double post_probes2){
    cout << left << setw(20) << method 
         << setw(15) << col1 << setw(20) << fixed << setprecision(2) << time1 << setw(20) << probes1 
         << setw(20) << post_time1 << setw(20) << post_probes1 
         << setw(15) << col2 << setw(20) << time2 << setw(20) << probes2 
         << setw(20) << post_time2 << setw(20) << post_probes2 << endl;
}

vector<string> generateRandomWords(int cnt, mt19937& rng){
    set<string> words;
    while(words.size() < cnt){
        string word;
        uniform_int_distribution<int> dist(5, 10);
        uniform_int_distribution<char> char_dist('a', 'z');
        int sz = dist(rng);
        for(int i=0; i<sz; i++){
            word += char_dist(rng);
        }
        words.insert(word);
    }
    return vector<string>(words.begin(), words.end());
}

result run_experiment(collision method, int hshFunc, int sz, double load_factor, 
                     const vector<string>& words, mt19937& rng){
    VaultXHashTable table(sz);
    result res;

    int insert_count = load_factor*sz;
    vector<string> insertedWords;

    for(int i=0; i<insert_count; i++){
        const string& word = words[i];
        bool inserted = false;
        switch(method){
            case CHAINING:
                inserted = table.insertChaining(word, hshFunc);
                break;
            case LINEAR_PROBING:
                inserted = table.insertLinearProbing(word, hshFunc);
                break;
            case DOUBLE_HASHING:
                inserted = table.insertDoubleHashing(word, hshFunc);
                break;
        }
        if(inserted) insertedWords.push_back(word);
    }

    res.collisions = table.getCollisions();

    // search before deletion

    table.reset();
    int search_count = max(1, (int)(insertedWords.size()*0.1));
    vector<string> searchWords;
    uniform_int_distribution<int> dist(0, insertedWords.size()-1);
    for (int i=0; i<search_count; i++){
        searchWords.push_back(insertedWords[dist(rng)]);
    }

    auto start = high_resolution_clock::now();
    for(const string& word:searchWords){
        switch(method){
            case CHAINING:
                table.searchChaining(word, hshFunc);
                break;
            case LINEAR_PROBING:
                table.searchLinearProbing(word, hshFunc);
                break;
            case DOUBLE_HASHING:
                table.searchDoubleHashing(word, hshFunc);
                break;
        }
    }
    auto end = high_resolution_clock::now();
    res.avg_time_before = duration_cast<microseconds>(end-start).count()/(double)search_count;
    res.avg_probes_before = table.getAverageProbes();
    
    // deletion

    vector<string> deletedWords;

    int delete_count = max(1, (int)(insertedWords.size()*0.1));
    for(int i=0; i<delete_count; i++){
        const string& word = insertedWords[dist(rng)];
        switch(method){
            case CHAINING:
                table.deleteChaining(word, hshFunc);
                break;
            case LINEAR_PROBING:
                table.deleteLinearProbing(word, hshFunc);
                break;
            case DOUBLE_HASHING:
                table.deleteDoubleHashing(word, hshFunc);
                break;
        }
        deletedWords.push_back(word);
    }

    // search after deletion

    vector<string> not_deleted_words;
    for(auto word: insertedWords){
        if(find(deletedWords.begin(), deletedWords.end(), word) == deletedWords.end()){
            not_deleted_words.push_back(word);
        }
    }

    // cout << "deletedWords size: " << deletedWords.size() << endl;
    // cout << "not_deleted_words size: " << not_deleted_words.size() << endl;

    table.reset();
    searchWords.clear();
    if(!deletedWords.empty() && !not_deleted_words.empty()){
        uniform_int_distribution<int> del_dist(0, deletedWords.size()-1);
        uniform_int_distribution<int> not_del_dist(0, not_deleted_words.size()-1);
        
        for(int i=0; i<min(search_count/2, (int)deletedWords.size()); i++){
            searchWords.push_back(deletedWords[del_dist(rng)]);
        }
        for(int i=0; i<min(search_count/2, (int)not_deleted_words.size()); i++){
            searchWords.push_back(not_deleted_words[not_del_dist(rng)]);
        }
    }

    start = high_resolution_clock::now();
    for(const string& word:searchWords){
        switch(method){
            case CHAINING:
                table.searchChaining(word, hshFunc);
                break;
            case LINEAR_PROBING:
                table.searchLinearProbing(word, hshFunc);
                break;
            case DOUBLE_HASHING:
                table.searchDoubleHashing(word, hshFunc);
                break;
        }
    }
    end = high_resolution_clock::now();
    res.avg_time_after = duration_cast<microseconds>(end-start).count()/(double)searchWords.size();
    res.avg_probes_after = table.getAverageProbes();

    return res;
}

int main() {
    int word_count = 10000;
    int table_size = 10000;
    double loadfactor[] = {0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    // cout << "Random word generation started" << endl;
    vector<string> words = generateRandomWords(word_count, rng);
    // cout << "Random word generation done" << endl;

    // ofstream outputFile("output.txt");
    // streambuf* coutBuf = cout.rdbuf();
    // cout.rdbuf(outputFile.rdbuf());

    for(double lf:loadfactor){
        cout << "\n===============================================================================================================================================================" << endl;
        cout << "                                                    PERFORMANCE FOR LOAD FACTOR: " << lf << endl;
        cout << "===============================================================================================================================================================" << endl;
        cout << left << setw(20) << "" << setw(80) << "------------------------------ Hash1 Function ------------------------------" << setw(80) << "------------------------------ Hash2 Function ------------------------------" << endl;
        print_table_header();

        collision methods[] = {CHAINING, LINEAR_PROBING, DOUBLE_HASHING};
        vector<string> method_names = {"CHAINING", "LINEAR_PROBING", "DOUBLE_HASHING"};
        int i=0;
        for(collision method:methods) {
            result res1, res2;
            res1 = run_experiment(method, 1, table_size, lf, words, rng);
            res2 = run_experiment(method, 2, table_size, lf, words, rng);
            print_table_row(method_names[i], 
                res1.collisions, res1.avg_time_before, res1.avg_probes_before, res1.avg_time_after, res1.avg_probes_after,
                res2.collisions, res2.avg_time_before, res2.avg_probes_before, res2.avg_time_after, res2.avg_probes_after);
            i++;
        }
    }

    // cout.rdbuf(coutBuf);
    // outputFile.close();
    // cout << "Performance results saved to output.txt" << endl;
    return 0;
}