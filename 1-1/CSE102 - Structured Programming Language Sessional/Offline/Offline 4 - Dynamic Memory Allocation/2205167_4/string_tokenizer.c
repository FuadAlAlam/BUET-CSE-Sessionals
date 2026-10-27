#include <stdio.h>
#include <stdlib.h>


char **strTokenize(char *str, char delim){
    int token_n=0, length=0;
    while(*(str+length) != '\0'){
        if(*(str+length) == delim){
            if(length>0 && *(str+length-1) != delim) token_n++;
        }
        length++;
    }
    if(length>0 && *(str+length-1) != delim) token_n++;
    char **tokens = (char **)malloc(sizeof(char *) * (token_n + 1));
    for(int i=0; i<=token_n; i++){
        *(tokens+i) = (char *)malloc(sizeof(char) * (length + 1));
    }
    int j=0, k=0;
    for(int i=0; i<length; i++){
        if(*(str+i) != delim){
            *(*(tokens+j)+k) = *(str+i);
            k++;
        }
        else{
            if(i>0 && *(str+i-1) != delim){
                *(*(tokens+j)+k) = '\0';
                j++;
                k=0;
            }
        }
    }
    if(length>0 && *(str+length-1) != delim) *(*(tokens+token_n-1)+k) = '\0';
    **(tokens+token_n) = '\0';
    return tokens;
}
void _printTokens(char **tokens){
    printf("The tokens are:\n");
    int i=0;
    while(**(tokens+i) != '\0'){
        printf("%s\n", *(tokens+i));
        i++;
    }
}
void _printTokensUnique(char **tokens){
    printf("The unique tokens are:\n");
    if(**tokens != '\0'){
        printf("%s\n", *tokens);
    }
    if(**tokens != '\0'){
        int i=1;
        while(**(tokens+i) != '\0'){
            int flag=-1;
            for(int j=0; j<i; j++){
                int m=0;
                while(*(*(tokens+j)+m) == *(*(tokens+i)+m)){
                    m++;
                    if(*(*(tokens+j)+m-1) == '\0'){
                        flag=0;
                        break;
                    }
                }
                if(flag == 0){
                    i++;
                    break;
                }
            }
            if(flag == -1){
                printf("%s\n", *(tokens+i));
                i++;
            }
        }
    }
}
void _freeTokens(char **tokens){
    free(tokens);
}


int main(int argc, char **args)
{
    int string_length;
    scanf("%d", &string_length);

    char delim;
    scanf(" %c", &delim);

    char *str = (char *)malloc(sizeof(char) * (string_length + 1));
    scanf("%s", str);

    // printf("inputs are: %d %c(%d) %s\n", string_length, delim, (int)delim, str);

    char **tokens = strTokenize(str, delim);
    _printTokens(tokens);
    _printTokensUnique(tokens);
    _freeTokens(tokens);
    free(str);

    return 0;
}