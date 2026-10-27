#include<iostream>
#include<vector>
#include<queue>

using namespace std;

const int N = 1e3;
int graph[N][N];
bool visited[N];

void BFS(int vertex, int n){
    queue<int> q;
    visited[vertex] = true;
    q.push(vertex);
    while(!q.empty()){
        int v = q.front();
        q.pop();
        for(int i=1; i<=n; i++){
            if(graph[v][i]==0 || visited[i]) continue;
            visited[i] = true;
            q.push(i);
        }
    }
}

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, m;
    cin>>n>>m;
    for(int i=0; i<m; i++){
        int a, b;
        cin>>a>>b;
        graph[a][b] = 1;
        graph[b][a] = 1;
    }

    vector<int> root;
    for(int i=1; i<=n; i++){
        if(visited[i]) continue;
        root.push_back(i);
        BFS(i, n);
    }

    int component = root.size();
    cout<<component-1<<endl;
    for(int i=0; i<component-1; i++){
        cout<<root[i]<<" "<<root[i+1]<<endl;
    }
}
