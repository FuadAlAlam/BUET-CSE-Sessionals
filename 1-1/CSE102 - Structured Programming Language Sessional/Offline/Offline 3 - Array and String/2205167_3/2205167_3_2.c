#include <stdio.h>

int main (){

    char str[5*104+1];
    fgets(str, 5*104+1, stdin);
    int max = 1, i = 0, j, k;
    while(str[i] != '\n'){
        j = i;
        while(str[j] != '\n'){
            for(k = i; k < j; k++){
                if(str[k] == str[j]){
                    break;
                }
            }
            if(k == j){
                if(j-i+1 > max){
                    max = j-i+1;
                }
            }
            else{
                break;
            }
            j++;
        }
        i++;
    }
    
    printf("%d", max);

}