#include "StackLinkedList.h" // Include Array-based Stack implementation
#include <cstdio>  
#include <string>


int main(){
    string s;
    int a;
    cin>>s>>a;
    Stack st;
    int size = s.size();
    for(int i=a; i<size; i++){
        if(s[i] == '(') st.push(s[i]);
        else if(s[i] == ')') st.pop();
        if(st.length()==0){
            cout<<i<<endl;
            break;
        }
    }
}
