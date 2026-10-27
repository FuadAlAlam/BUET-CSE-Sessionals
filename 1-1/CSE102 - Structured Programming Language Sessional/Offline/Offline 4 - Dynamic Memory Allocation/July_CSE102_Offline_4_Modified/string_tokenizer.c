#include <stdio.h>
#include <stdlib.h>


char **strTokenize(char *str, char delim);
void _printTokens(char **tokens);
void _printTokensUnique(char **tokens);
void _freeTokens(char **tokens);


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

    return 0;
}
