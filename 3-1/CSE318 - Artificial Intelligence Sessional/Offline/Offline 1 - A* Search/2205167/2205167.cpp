#include<bits/stdc++.h>

using namespace std;


enum class HeuristicType{
    HAMMING,
    MANHATTAN,
    EUCLIDEAN,
    LINEAR_CONFLICT,
    CUSTOM
};


class Node{

    vector<vector<int>> board;
    int g, h;
    double f;
    shared_ptr<Node> parent;

public:
    Node(vector<vector<int>> b, int g_val, int h_val, double w, shared_ptr<Node> p){
        board = b;
        g = g_val;
        h = h_val;
        f = (double) g + w*h;
        parent = p;
    }

    const vector<vector<int>>& getBoard(){
        return board;
    }
    int getG(){
        return g;
    }
    int getH(){
        return h;
    }
    double getF(){
        return f;
    }
    shared_ptr<Node> getParent(){
        return parent;
    }


    bool operator>(const Node& other) const{
        if(f == other.f) return h > other.h;
        return f > other.f;
    }
};

struct CompareNode{
    bool operator()(const shared_ptr<Node>& a, const shared_ptr<Node>& b){
        return *a > *b;
    }
};


class AStarSearch{

    int k;
    double W;
    HeuristicType heuristicType;
    int total;

    
    int calculateHeuristic(const vector<vector<int>>& board){
        switch(heuristicType){
            case HeuristicType::HAMMING: return hammingDistance(board);
            case HeuristicType::MANHATTAN: return manhattanDistance(board);
            case HeuristicType::EUCLIDEAN: return euclideanDistance(board);
            case HeuristicType::LINEAR_CONFLICT: return linearConflict(board);
            case HeuristicType::CUSTOM: return customHeuristic(board);
            default: return manhattanDistance(board);
        }
    }

    int hammingDistance(const vector<vector<int>>& board){
        int cnt = 0;
        int val = 1;
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                if(i == k-1 && j == k-1) val = 0; 
                if(board[i][j] != 0 && board[i][j] != val) cnt++;
                val++;
            }
        }
        return cnt;
    }

    int manhattanDistance(const vector<vector<int>>& board){
        int dist = 0;
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                int val = board[i][j];
                if(val != 0){
                    int valX = (val-1)/k;
                    int valY = (val-1)%k;
                    dist += abs(i-valX) + abs(j-valY);
                }
            }
        }
        return dist;
    }

    int euclideanDistance(const vector<vector<int>>& board){
        double dist = 0.0;
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                int val = board[i][j];
                if(val != 0){
                    int valX = (val-1)/k;
                    int valY = (val-1)%k;
                    dist += sqrt((i-valX)*(i-valX) + (j-valY)*(j-valY));
                }
            }
        }
        return dist; 
    }

    int linearConflict(const vector<vector<int>>& board){
        int cnt = 0;
        
        for(int r = 0; r < k; r++){
            for(int c1 = 0; c1 < k; c1++){
                for(int c2 = c1+1; c2 < k; c2++){
                    int v1 = board[r][c1];
                    int v2 = board[r][c2];
                    if(v1 != 0 && v2 != 0){
                        int row1 = (v1-1)/k;
                        int row2 = (v2-1)/k;
                        
                        if(row1 == r && row2 == r){
                            int col1 = (v1-1)%k;
                            int col2 = (v2-1)%k;
                            
                            if(col1 > col2){
                                cnt++;
                            }
                        }
                    }
                }
            }
        }

        
        for(int c = 0; c < k; c++){
            for(int r1 = 0; r1 < k; r1++){
                for(int r2 = r1+1; r2 < k; r2++){
                    int v1 = board[r1][c];
                    int v2 = board[r2][c];
                    if(v1 != 0 && v2 != 0){
                        int col1 = (v1-1)%k;
                        int col2 = (v2-1)%k;
                        
                        if(col1 == c && col2 == c){
                            int row1 = (v1-1)/k;
                            int row2 = (v2-1)/k;
                            
                            if(row1 > row2){
                                cnt++;
                            }
                        }
                    }
                }
            }
        }
        
        return manhattanDistance(board) + 2*cnt; 
    }

    int customHeuristic(const vector<vector<int>>& board){
        int penalty = 0;
        
        if(board[0][0] != 1 && board[0][0] != 0){
            bool r = (k > 1 && board[0][1] == 2);
            bool d = (k > 1 && board[1][0] == k+1);
            if(r || d){
                penalty+=2;
            }
        }
        
        if(board[0][k-1] != k && board[0][k-1] != 0){
            bool l = (k > 1 && board[0][k-2] == k-1);
            bool d = (k > 1 && board[1][k-1] == 2*k);
            if(l || d){
                penalty+=2;
            }
        }
        
        int m = k*(k-1)+1;
        if(board[k-1][0] != m && board[k-1][0] != 0){
            bool r = (k > 1 && board[k-1][1] == m+1);
            bool u = (k > 1 && board[k-2][0] == m-k);
            if(r || u){
                penalty+=2;
            }
        }
        
        return linearConflict(board)+penalty;
    }

    
    void printBoard(const vector<vector<int>>& board){
        for(auto& row: board){
            for(int val: row){
                cout<<val<<" ";
            }
            cout<<"\n";
        }
        cout<<"\n";
    }

    void printSolutionPath(shared_ptr<Node> finalNode){
        vector<vector<vector<int>>> path;
        shared_ptr<Node> curr = finalNode;
        while(curr != nullptr){
            path.push_back(curr->getBoard());
            curr = curr->getParent();
        }
        
        reverse(path.begin(), path.end());
        for(auto& b: path){
            printBoard(b);
        }
    }

public:
    AStarSearch(int n, double w = 1.0, HeuristicType hType = HeuristicType::MANHATTAN){
        k = n;
        W = w;
        heuristicType = hType;
        total = 0;
    }

    bool isSolvable(const vector<vector<int>>& board){
        vector<int> arr;
        int m = 0;
        
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                if(board[i][j] == 0){
                    m = k-i;
                }
                else{
                    arr.push_back(board[i][j]);
                }
            }
        }
        
        int invCnt = 0;
        for(int i = 0; i < arr.size(); i++){
            for(int j = i+1; j < arr.size(); j++){
                if(arr[i]>arr[j]) invCnt++;
            }
        }
        
        if(k%2 != 0){
            return invCnt%2 == 0;
        }
        else{
            if(m%2 == 0) return invCnt%2 != 0;
            else return invCnt%2 == 0;
        }
    }

    void solve(const vector<vector<int>>& initialBoard){
        if(!isSolvable(initialBoard)){
            cout<<"Unsolvable puzzle\n";
            return;
        }

        priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, CompareNode> openList;
        
        
        set<vector<vector<int>>> closedList;
        
        total = 0;

        int h = calculateHeuristic(initialBoard);
        shared_ptr<Node> startNode = make_shared<Node>(initialBoard, 0, h, W, nullptr);
        openList.push(startNode);

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        while(!openList.empty()){
            shared_ptr<Node> curr = openList.top();
            openList.pop();
            total++;

            if(curr->getH() == 0){
                cout<<"Minimum number of moves = "<<curr->getG()<<"\n\n";
                // cout<<"Nodes explored = "<<total<<"\n\n";
                
                printSolutionPath(curr);
                return;
            }

            auto& currBoard = curr->getBoard();
            

            if(closedList.count(currBoard)) continue;
            closedList.insert(currBoard);

            int X = -1, Y = -1;
            for(int i = 0; i < k; i++){
                for(int j = 0; j < k; j++){
                    if(currBoard[i][j] == 0){
                        X = i;
                        Y = j;
                        break;
                    }
                }
                if(X != -1) break;
            }

            for(int i = 0; i < 4; i++){
                int newX = X+dx[i];
                int newY = Y+dy[i];

                if(newX >= 0 && newX < k && newY >= 0 && newY < k){
                    vector<vector<int>> newBoard = currBoard;
                    swap(newBoard[X][Y], newBoard[newX][newY]);

                    if(closedList.find(newBoard) == closedList.end()){
                        int hVal = calculateHeuristic(newBoard);
                        shared_ptr<Node> currNode = make_shared<Node>(newBoard, curr->getG()+1, hVal, W, curr);
                        openList.push(currNode);
                    }
                }
            }
        }
    }
};

int main(){
    // int t;
    // cin>>t;
    // for(int tt=1; tt<=t; tt++){
    // cout<<"INPUT "<<tt<<":\n";


    int k;
    cin>>k;

    vector<vector<int>> initialBoard(k, vector<int>(k));
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            cin>>initialBoard[i][j];
        }
    }

    AStarSearch AStar(k, 2.0, HeuristicType::LINEAR_CONFLICT);
    
    AStar.solve(initialBoard);

    // }

    return 0;
}