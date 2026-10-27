#include "Maxheap.h"

int main(){
    freopen("input.txt","r", stdin);
    freopen("output.txt","w", stdout);

    int n, k;
    std::cin>>n>>k;

    MaxHeap heap; 
    for(int i=0; i<n; i++){
        int val;
        std::cin>>val;
        heap.insert(val);
    }

    int ans=0;
    for(int i=0; i<k; i++){
        int val = heap.findMax();
        if(val==0) break;
        ans+=heap.extractMax();
        heap.insert(val-1);
    }

    std::cout<<ans;
}