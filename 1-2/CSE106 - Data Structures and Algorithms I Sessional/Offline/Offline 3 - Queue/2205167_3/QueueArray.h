#include <iostream>
using namespace std;

class Queue
{
private:
    int *array;
    //write your code here. Add additional private variables if necessary
    int size, capacity;

public:
    // Constructor
    Queue()
    {
        array = new int[1];
        //write your code here. Initialize additional private variables if necessary
        size = 0; capacity = 1;
    }
    Queue(int initialCapacity)
    {
        array = new int[initialCapacity]; // Allocate initial memory
        //write your code here. Initialize additional private variables if necessary
        size = 0; capacity = initialCapacity;
    }

    // Destructor
    ~Queue()
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

        capacity = newCapacity;
        array = newArray;
    }

    // Enqueue an element onto the queue
    void enqueue(int x)
    {
        //write your code here. Check if the array is full and resize if necessary.
        if(size == capacity) resize(2*capacity);
        array[size] = x;
        size++;
    }

    // Remove and return the peek element
    int dequeue()
    {
       //write your code here. Check if the stack is empty and return -1 if it is.
       if(isEmpty()) return -1;
       //remove the peek element and return it
       int peek_element = array[0];
       for(int i=0; i<size-1; i++){
            array[i] = array[i+1];
       }
       size--;
       //resize the array if necessary
       if(4*size < capacity) resize(capacity/2);

       return peek_element;
    }

    // Return the peek element without removing it
    int peek()
    {
        //write your code here. Check if the queue is empty and return -1 if it is.
        if(isEmpty()) return -1;
        //return the peek element
        return array[0];
    }

    // Return the number of elements in the queue
    int length()
    {
        //write your code here. Return the number of elements in the queue
        return size;
    }

    // Check if the queue is empty
    bool isEmpty()
    {
        //write your code here. Return true if the queue is empty, false otherwise
        if(size==0) return true;
        else return false;
    }

    // Clear the queue
    void clear()
    {
        //write your code here. Clear the queue. resize the array to 1
        size = 0;
        resize(1);
    }
};
