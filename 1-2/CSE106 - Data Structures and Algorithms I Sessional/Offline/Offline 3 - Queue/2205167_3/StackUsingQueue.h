#include "QueueArray.h"
#include <iostream>
using namespace std;

class Stack
{
private:
    
    //write your code here. Add additional private variables if necessary
    Queue stack;

public:
    // Constructor
    Stack()
    {
        //write your code here. Initialize additional private variables if necessary
    }
    Stack(int initialCapacity)
    {
        //write your code here. Initialize additional private variables if necessary
    }

    // Destructor
    ~Stack()
    {
    }

    
    // Push an element onto the stack
    void push(int x)
    {
        //write your code here.
        stack.enqueue(x);
        //push the element onto the stack
    }

    // Remove and return the top element
    int pop()
    {
       //write your code here. Check if the stack is empty and return -1 if it is.
       if(stack.isEmpty()) return -1;
       //pop the top element and return it
       for(int i=1; i<stack.length(); i++){
            int x = stack.dequeue();
            stack.enqueue(x);
       }
       return stack.dequeue();
    }

    // Return the top element without removing it
    int top()
    {
        //write your code here. Check if the stack is empty and return -1 if it is.
        if(stack.isEmpty()) return -1;
        //return the top element
        for(int i=1; i<stack.length(); i++){
            int x = stack.dequeue();
            stack.enqueue(x);
        }
        int x = stack.dequeue();
        stack.enqueue(x);
        return x;
    }

    // Return the number of elements in the stack
    int length()
    {
        //write your code here. Return the number of elements in the stack
        return stack.length();
    }

    // Check if the stack is empty
    bool isEmpty()
    {
        //write your code here. Return true if the stack is empty, false otherwise
        if(stack.isEmpty()) return true;
        else return false;
    }

    // Clear the stack
    void clear()
    {
        //write your code here. Clear the stack.
        stack.clear();
    }
};
