#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int * array;
    // Add more fields here
    int size, capacity, pos;
} ArrayList;


void init_arraylist(ArrayList *list, int capacity)
{
    // printf("implement init_arraylist\n");
    // dynamically allocate space for the array
    // initialize the size, capacity, and current position
    list->array = (int*) malloc(capacity * sizeof(int));
    list->size = 0;
    list->capacity = capacity;
    list->pos = 0;
}

void resize(ArrayList *list, int new_capacity);

void clear(ArrayList *list)
{
    printf("The list is cleared\n");
    // printf("Implement clear\n");
    // clear the list but do not free the array
    // modify the size, capacity, and current position
    list->size=0;
    list->pos=0;
    resize(list, 1);
}

int get_size(ArrayList *list)
{
    printf("The size of the list is %d\n", list->size);
    // printf("Implement get_size\n");
    // return -1;
    return list->size;
}


void resize(ArrayList *list, int new_capacity)
{
    printf("Resized array from %d to %d\n", list->capacity, new_capacity);
    // printf("Implement resize\n");
    // allocate space for new array with new_capacity
    // print log message
    list->array = (int *) realloc(list->array, new_capacity * sizeof(int));
    list->capacity = new_capacity;
}


void append(ArrayList *list, int value)
{
    // printf("Implement append\n");
    // call resize if necessary
    // add value to the end of the list
    if(list->size == list->capacity){
        resize(list, 2*list->capacity);
    }
    
    printf("Appended %d\n", value);
    list->array[list->size] = value;
    list->size++;
}


void insert(ArrayList *list, int value)
{
    // printf("Implement insert\n");
    // call resize if necessary
    // shift the elements to the right to make space
    // add value at the current position
    if(list->size == list->capacity){
        resize(list, 2*list->capacity);
    }

    printf("Inserted %d at position %d\n", value, list->pos);
    for(int i=list->size; i>list->pos; i--){
        list->array[i] = list->array[i-1];
    }
    list->array[list->pos] = value;
    list->size++;
}


int remove_at_current(ArrayList *list)
{
    // printf("Implement remove_at_current\n");
    // return -1;
    // save the value of the current element in a variable
    // shift the elements to the left to fill the gap
    // change the size, and current position as necessary
    // call resize if necessary
    // return the saved value
    if(list->size == 0){
        printf("The list is empty\n");
        return -1;
    }
    int remove_value = list->array[list->pos];

    printf("Removed %d at position %d\n", remove_value, list->pos);
    for(int i=list->pos; i<list->size-1; i++){
        list->array[i] = list->array[i+1];
    }
    if(list->pos == list->size-1){
        list->pos--;
    }
    list->size--;
    if(list->size * 4 < list->capacity){
        resize(list, list->capacity/2);
    }
    return remove_value;
}

int find(ArrayList *list, int value)
{
    // printf("Implement find\n");
    // return -1;
    // traverse the list and return the position of the value
    // return -1 if the value is not found
    for(int i=0; i<list->size; i++){
        if(list->array[i] == value){
            printf("Found %d at position %d\n", value, i);
            return i;
        }
    }
    printf("Did not find %d in the list\n", value);
    return -1;
}


void move_to_start(ArrayList *list)
{
    printf("Moved current position from %d to the start\n", list->pos);
    // printf("Implement move_to_start\n");
    // consider the cases when the list is empty
    list->pos = 0;
}


void move_to_end(ArrayList *list)
{
    printf("Moved current position from %d to the end\n", list->pos);
    // printf("Implement move_to_end\n");
    // consider the cases when the list is empty
    list->pos = list->size-1;
}


void prev(ArrayList *list)
{
    // printf("Implement prev\n");
    // no change if the current position is at the start
    if(list->pos == 0){
        printf("Current position is already at the start\n");
        return;
    }

    printf("Moved current position from %d to %d\n", list->pos, list->pos-1);
    list->pos--;
}


void next(ArrayList *list)
{
    // printf("Implement next\n");
    // no change if the current position is at the end
    if(list->pos == list->size-1){
        printf("Current position is already at the end\n");
        return;
    }

    printf("Moved current position from %d to %d\n", list->pos, list->pos+1);
    list->pos++;
}


void move_to_position(ArrayList *list, int position)
{
    // printf("Implement move_to_position\n");
    if(position >= list->size){
        printf("The given position is out of range\n");
        return;
    }
    printf("Moved current position from %d to %d\n", list->pos, position);
    list->pos = position;
}


int get_current_position(ArrayList *list)
{
    // printf("Implement get_current_position\n");
    // return -1;
    if(list->size == 0){
        printf("The list is empty\n");
        return -1;
    }
    printf("The current position is %d\n", list->pos);
    return list->pos;
}


int get_current_element(ArrayList *list)
{
    // printf("Implement get_current_element\n");
    // return -1;
    if(list->size == 0){
        printf("The list is empty\n");
        return -1;
    }
    printf("The current element is %d\n", list->array[list->pos]);
    return list->array[list->pos];
}


void print_list(ArrayList *list)
{
    // printf("< list elements here >");
    printf("< ");
    for(int i=0; i<list->size; i++){
        if(list->pos == i) printf("|");
        if(i == list->size-1){
            printf("%d", list->array[i]);
            break;
        }
        printf("%d  ", list->array[i]);
    }
    printf(" >\n");
}


void free_list(ArrayList *list)
{
    printf("The list is freed\n");
    // printf("Implement free_list\n");
    // free the array before terminating the program
    free(list->array);
}


