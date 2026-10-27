#include<iostream>
#include<vector>

using namespace std;

const int N = 1e5;
vector<int> graph[N];
bool visited[N];

void DFS(int vertex){
    visited[vertex] = true;
    for(int child: graph[vertex]){
        if(visited[child]) continue;
        DFS(child);
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
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> root;
    for(int i=1; i<=n; i++){
        if(visited[i]) continue;
        root.push_back(i);
        DFS(i);
    }

    int component = root.size();
    cout<<component-1<<endl;
    for(int i=0; i<component-1; i++){
        cout<<root[i]<<" "<<root[i+1]<<endl;
    }
}