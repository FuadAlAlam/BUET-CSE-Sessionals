#include <iostream>
using namespace std;

class Stack
{
private:
    int *array;
    //write your code here. Add additional private variables if necessary
    int size, capacity;

public:
    // Constructor
    Stack()
    {
        array = new int[1]; 
        //write your code here. Initialize additional private variables if necessary
        size = 0, capacity = 1;
    }
    Stack(int initialCapacity)
    {
        array = new int[initialCapacity]; // Allocate initial memory
        //write your code here. Initialize additional private variables if necessary
        size = 0, capacity = initialCapacity;
    }

    // Destructor
    ~Stack()
    {
        delete[] array; // Free dynamically allocated memory
    }

    // Helper function to resize the array when full
    void resize(int newCapacity)
    {
        int *newArray = new int[newCapacity]; // Allocate new array
        
        //write your code here. Copy the elements from the old array to the new array

        for(int i=0; i<size; i++){
            newArray[i] = array[i];
        }

        delete[] array; // Free old memory
        
        //write your code here. Update the capacity and array pointers

        array = newArray;
    }

    // Push an element onto the stack
    void push(int x)
    {
        //write your code here. Check if the array is full and resize if necessary.
        if(size == capacity){
            resize(2*capacity);
        }
        //push the element onto the stack
        array[size] = x;
        size++;
    }

    // Remove and return the top element
    int pop()
    {
       //write your code here. Check if the stack is empty and return -1 if it is.
       if(size == 0){
            return -1;
       }
       //pop the top element and return it
       int val = array[size-1];
       size--;
       return val;
       //resize the array if necessary
       if(size * 4 < capacity){
            resize(capacity/2);
       }
    }

    // Return the top element without removing it
    int top()
    {
        //write your code here. Check if the stack is empty and return -1 if it is.
        if(size == 0){
            return -1;
        }
        //return the top element
        return array[size-1];
    }

    // Return the number of elements in the stack
    int length()
    {
        //write your code here. Return the number of elements in the stack
        return size;
    }

    // Check if the stack is empty
    bool isEmpty()
    {
        //write your code here. Return true if the stack is empty, false otherwise
        if(size == 0){
            return true;
        }
        return false;
    }

    // Clear the stack
    void clear()
    {
        //write your code here. Clear the stack. resize the array to 1
        delete[] array;
        array = new int[1];
        size = 0;
        resize(1);
    }
};
