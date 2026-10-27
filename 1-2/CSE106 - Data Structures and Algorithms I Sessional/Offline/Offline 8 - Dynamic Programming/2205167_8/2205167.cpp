#include<iostream>
#include<string>

using namespace std;

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    string t, p;
    cin>>t>>p;

    int a = t.size(), b = p.size();

    bool dp[a+1][b+1];
    for(int i=0; i<=a; i++){
        for(int j=0; j<=b; j++){
            dp[i][j] = false;
        }
    }
    
    dp[0][0] = true;
    for(int i=1; i<=b; i++){
        if(p[i-1] == '*') dp[0][i] = dp[0][i-2];
    }

    for(int i=1; i<=a; i++){
        for(int j=1; j<=b; j++){
            if(t[i-1] == p[j-1]) dp[i][j] = dp[i-1][j-1];
            else if(p[j-1] == '.') dp[i][j] = dp[i-1][j-1];
            else if(p[j-1] == '*'){
                dp[i][j] = dp[i][j-2];
                if(t[i-1] == p[j-2] || p[j-2] == '.') dp[i][j] = dp[i][j] || dp[i-1][j];
            }
            else if(p[j-1] == '+'){
                if(t[i-1] == p[j-2] || p[j-2] == '.') dp[i][j] = dp[i-1][j];
            }
        }
    }

    if(dp[a][b]) cout<<"True";
    else cout<<"False";
}