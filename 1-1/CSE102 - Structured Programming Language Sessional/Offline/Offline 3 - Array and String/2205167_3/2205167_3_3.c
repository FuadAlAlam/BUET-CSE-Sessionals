#include <stdio.h>
#include <string.h>

int main(){

    char s[21], p[21], p2[21];
    scanf("%s %s", s, p);
    int s1 = strlen(s), p1 = strlen(p);
    int dot=-1, star=-1, dot1=0, star1=0;
    for(int i=0; i<p1; i++){
        if(p[i] == '.'){
            dot = i;
            dot1 = 1;
        }
        if(p[i] == '*'){
            star = i;
            star1 = 2;
        }
    }
    if(dot1+star1 == 0){
        if(strcmp(s, p) == 0){
            printf("true");
        }
        else{
            printf("false");
        }
    }
    else if(dot1+star1 == 1){
        if(s1 != p1){
            printf("false");
        }
        else{
            p[dot] = s[dot];
            if(strcmp(s, p) == 0){
                printf("true");
            }
            else{
                printf("false");
            }
        }
    }
    else if(dot1+star1 == 2 || (dot1+star1 == 3 && dot+1 != star)){
        if(p1>s1+2){
            printf("false");
        }
        else{
            for(int i=0; i<star-1; i++){
                if(i != dot) p2[i] = p[i];
                else p2[i] = s[i];
            }
            for(int i=p1-1; i>star; i--){
                if(i != dot) p2[s1-p1+i] = p[i];
                else p2[s1-p1+i] = s[s1-p1+i];
            }
            for(int i=star-1; i<=s1-p1+star; i++) p2[i] = p[star-1];
            if(strcmp(s, p2) == 0){
                printf("true");
            }
            else{
                printf("false");
            }
        }
    }
    else{
        if(p1>s1+2){
            printf("false");
        }
        else{
            for(int i=0; i<star-1; i++) p2[i] = p[i];
            for(int i=p1-1; i>star; i--) p2[s1-p1+i] = p[i];
            for(int i=star-1; i<=s1-p1+star; i++) p2[i] = s[i];
            if(strcmp(s, p2) == 0){
                printf("true");
            }
            else{
                printf("false");
            }
        }
    }
    
}