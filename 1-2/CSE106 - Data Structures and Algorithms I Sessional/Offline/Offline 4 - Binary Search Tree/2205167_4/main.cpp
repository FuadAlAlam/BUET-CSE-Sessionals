#include "bst.h"
#include <iostream>

using namespace std;


int main(){

    // Do not modify this file.
    // Input will be read from input.txt
    // Outputs will be written to output.txt

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    char option;
    int val;

    Node *root = NULL;
    
    while(scanf("%c %d\n", &option, &val) == 2){
        if(option == 'I'){
            root = insertNode(root, val);
            printTree(root);
            printf("\n");
        } else if (option == 'D'){
            root = deleteNode(root, val);
            printTree(root);
            printf("\n");
        } else if (option == 'F'){
            if(find(root, val) != NULL)
                printf("Found\n");
            else
                printf("Not Found\n");
        } else if(option == 'T'){
            if(val == 1){
                inOrder(root);
                printf("\n");
            }
            else if (val == 2)
            {
                preOrder(root);
                printf("\n");                
            }
            else{
                postOrder(root);
                printf("\n");
            }
        }
    }
    return 0;
}