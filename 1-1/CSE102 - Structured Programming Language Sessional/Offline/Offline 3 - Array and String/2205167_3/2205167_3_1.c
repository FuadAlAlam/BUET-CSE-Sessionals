#include <stdio.h>

int main(){

    int m, n, p;
    float median;
    scanf("%d %d", &m, &n);
    p = m+n;
    int arr[p];
    int temp, min, idx;
    for(int i = 0; i < p; i++){
        scanf("%d", &arr[i]);
    }
    for(int j = 0; j < p; j++){
        min = arr[j];
        idx = j;
        for(int k = j+1; k < p; k++){
            if(arr[k] < min){
                min = arr[k];
                idx = k;
            }
        }
        temp = arr[j];
        arr[j] = min;
        arr[idx] = temp;
    }

    if(p%2 == 1){
        printf("%d", arr[p/2]);
    }
    else{
        if((arr[p/2-1]+arr[p/2])%2 == 0){
            printf("%d", (arr[p/2-1]+arr[p/2])/2);
        }
        else{
            median = (arr[p/2-1]+arr[p/2])/2 + 0.5;
            printf("%.1f", median);
        }
    }

}