#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    // add *next and *prev here
    struct Node *next;
    struct Node *prev;
} Node;


typedef struct
{
    // add Node* head, tail, current_position and other necessary fields here
    Node *head, *tail, *current;
} LinkedList;


void init_linkedlist(LinkedList *list)
{
    // printf("implement init_linkedlist\n");
    // initialize head, tail with null
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
}

void clear(LinkedList *list)
{
    printf("The list is cleared\n");
    // printf("Implement clear\n");
    // traverse the list and free each node
    // set head and tail to null
    Node *v = list->head;
    while(v != NULL){
        Node *temp = v->next;
        free(v);
        v = temp;
    }
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
}


int get_size(LinkedList *list)
{
    // printf("Implement get_size\n");
    // return -1;
    Node *temp = list->head;
    int size=0;
    while(temp != NULL){
        size++;
        temp = temp->next;
    }
    printf("The size of the list is %d\n", size);
    return size;
}


void append(LinkedList *list, int value)
{
    printf("Appended %d\n", value);
    // printf("Implement append\n");
    // create a new node and set its value
    // consider the case when the list is empty and when it isnt
    Node *v = (Node *) malloc(sizeof(Node));
    v->data = value;
    if(list->head == NULL){
        list->head = v;
        list->tail = v;
        list->current = v;
        return;
    }
    v->prev = list->tail;
    v->next = NULL;
    list->tail = v;
    v->prev->next = v;
}


void insert(LinkedList *list, int value)
{
    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }
    printf("Inserted %d at position %d\n", value, pos);
    // printf("Implement insert\n");
    // create a new node and set its value
    // place it at the current position (check order of operations)
    // consider the case when the list is empty and when it isnt
    Node *v = (Node *) malloc(sizeof(Node));
    v->data = value;
    if(list->head == NULL){
        list->head=v;
        list->tail=v;
        list->current=v;
        return;
    }
    if(list->current == list->head){
        v->prev = NULL;
        v->next = list->current;
        list->current->prev = v;
        list->current = v;
        list->head = v;
        return;
    }
    v->prev = list->current->prev;
    list->current->prev->next = v;
    v->next = list->current;
    list->current->prev = v;
    list->current = v;
}


int remove_at_current(LinkedList *list)
{
    // printf("Implement remove_at_current\n");
    // consider the case when current code is at the begining or at the end
    // return -1;
    if(list->head == NULL){
        printf("The list is empty\n");
        return -1;
    }
    if(list->current == list->head){
        Node *v = list->head;
        int temp = v->data;
        if(list->head == list->tail){
            free(v);
            list->head = NULL;
            list->tail = NULL;
            list->current = NULL;
            printf("Removed %d at position 0\n", temp);
            return temp;
        }
        list->head = v->next;
        list->current = v->next;
        list->head->prev = NULL;
        free(v);
        printf("Removed %d at position 0\n", temp);
        return temp;
    }
    if(list->current == list->tail){
        Node *temp1 = list->head;
        int pos=0;
        while(temp1 != list->current){
            pos++;
            temp1 = temp1->next;
        }
        Node *v = list->tail;
        int temp = v->data;
        list->tail = v->prev;
        list->current = v->prev;
        list->tail->next = NULL;
        free(v);
        printf("Removed %d at position %d\n", temp, pos);
        return temp;
    }
    Node *temp1 = list->head;
    int pos=0;
    while(temp1 != list->current){
        pos++;
        temp1 = temp1->next;
    }
    Node *v = list->current;
    int temp = v->data;
    v->prev->next = v->next;
    v->next->prev = v->prev;
    list->current = v->next;
    free(v);
    printf("Removed %d at position %d\n", temp, pos);
    return temp;
}


int find(LinkedList *list, int value)
{
    // printf("Implement find\n");
    // traverse the list and return the position of the value
    // return -1;
    Node *temp = list->head;
    int pos=0;
    while(temp != NULL){
        if(temp->data == value){
            printf("Found %d at position %d\n", value, pos);
            return pos;
        }
        pos++;
        temp = temp->next;
    }
    printf("Did not find %d in the list\n", value);
    return -1;
}


void move_to_start(LinkedList *list)
{
    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }
    printf("Moved current position from %d to the start\n", pos);
    // printf("Implement move_to_start\n");
    list->current = list->head;
}


void move_to_end(LinkedList *list)
{
    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }
    printf("Moved current position from %d to the end\n", pos);
    // printf("Implement move_to_end\n");
    list->current = list->tail;
}


void prev(LinkedList *list)
{
    if(list->current == list->head){
        printf("Current position is already at the start\n");
        return;
    }

    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }
    printf("Moved current position from %d to %d\n", pos, pos-1);
    // printf("Implement prev\n");
    list->current = list->current->prev;
}


void next(LinkedList *list)
{
    if(list->current == list->tail){
        printf("Current position is already at the end\n");
        return;
    }

    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }
    printf("Moved current position from %d to %d\n", pos, pos+1);
    // printf("Implement next\n");
    list->current = list->current->next;
}


void move_to_position(LinkedList *list, int position)
{
    Node *temp1 = list->head;
    int pos=0;
    while(temp1 != list->current){
        pos++;
        temp1 = temp1->next;
    }
    printf("Moved current position from %d to %d\n", pos, position);
    // printf("Implement move_to_position\n");
    // traverse the list and stop at the given position
    Node *temp = list->head;
    for(int i=0; i<position; i++){
        if(temp == NULL){
            printf("The given position is out of range\n");
            return;
        }
        temp = temp->next;
    }
    list->current = temp;
}


int get_current_position(LinkedList *list)
{
    // printf("Implement get_current_position\n");
    // traverse the list and stop when you are at the current position
    // return the position (integer)
    // return -1;
    if(list->head == NULL){
        printf("The list is empty\n");
        return -1;
    }
    Node *temp = list->head;
    int pos=0;
    while(temp != list->current){
        pos++;
        temp = temp->next;
    }

    printf("The current position is %d\n", pos);
    return pos;
}


int get_current_element(LinkedList *list)
{
    // printf("Implement get_current_element\n");
    // return the value at the current position
    // return -1;
    if(list->head == NULL){
        printf("The list is empty\n");
        return -1;
    }

    printf("The current element is %d\n", list->current->data);
    return list->current->data;
}


void print_list(LinkedList *list)
{
    // printf("< list elements here >");
    printf("< ");
    Node *temp = list->head;
    while(temp != NULL){
        if(temp == list->current) printf("|");
        if(temp == list->tail){
            printf("%d", temp->data);
            break;
        }
        printf("%d  ", temp->data);
        temp = temp->next;
    }
    printf(" >\n");
}


void free_list(LinkedList *list)
{
    printf("The list is freed\n");
    // printf("Implement free_list\n");
    // free each node in the list
    Node *v = list->head;
    while(v != NULL){
        Node *temp = v->next;
        free(v);
        v = temp;
    }
}
