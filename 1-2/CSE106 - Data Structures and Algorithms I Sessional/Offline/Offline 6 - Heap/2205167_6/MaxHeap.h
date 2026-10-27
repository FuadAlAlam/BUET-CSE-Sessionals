#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <iostream>
#include <fstream>
#include <stdexcept>

#define MAX_CAPACITY 100  //Defines the maximum capacity of the heap

class MaxHeap
{
private:
    int heap[MAX_CAPACITY];  //Array to store heap elements
    int size;                //Current number of elements in the heap

    //Sifts up the node at index i to maintain heap property
    void siftUp(int i)
    {
        /**Write your code here**/
        if(i==0 || heap[i] < heap[(i-1)/2]) return;

        int temp = heap[(i-1)/2];
        heap[(i-1)/2] = heap[i];
        heap[i] = temp;
        siftUp((i-1)/2);
    }

    //Sifts down the node at index i to maintain heap property
    void siftDown(int i)
    {
        /**Write your code here**/
        int k = INT_MAX;
        if(2*i+1 < size) k = 2*i+1;
        if(2*i+2 < size && heap[2*i+1] < heap[2*i+2]) k = 2*i+2;
        if(k != INT_MAX && heap[i] < heap[k]){
            int temp = heap[k];
            heap[k] = heap[i];
            heap[i] = temp;
            siftDown(k);
        }
    }

public:
    //Constructor initializes an empty heap
    MaxHeap() : size(0) {}

    //Inserts a new element x into the heap
    void insert(int x)
    {
        if (size == MAX_CAPACITY)
        {
            throw std::length_error("insert() called, but the heap is full.");
        }

        /**Write your code here**/
        heap[size] = x;
        siftUp(size);
        size++;
    }

    //Returns the maximum element without removing it
    int findMax()
    {
        if (size == 0)
        {
            throw std::runtime_error("findMax() called, but the heap is empty.");
        }

        /**Write your code here**/
        return heap[0];
    }

    //Removes and returns the maximum element from the heap
    int extractMax()
    {
        if (size == 0)
        {
            throw std::runtime_error("extractMax() called, but the heap is empty.");
        }

        /**Write your code here**/
        int ans = heap[0];
        heap[0] = heap[size-1];
        size--;
        siftDown(0);

        return ans;
    }


    //Returns the number of elements in the heap
    int getSize()
    {
        /**Write your code here**/
        return size;
    }

    //Checks if the heap is empty. Returns true if the heap is empty, false otherwise.
    bool isEmpty()
    {
        /**Write your code here**/
        if(size==0) return true;
        else return false;
    }

    //Increases the value of the element at index i to new_value
    void increaseKey(int i, int newValue)
    {
        if (i < 0 || i >= size || heap[i] >= newValue)
        {
            throw std::invalid_argument("increaseKey() called with invalid index or invalid new value.");
        }

        /**Write your code here**/
        heap[i] = newValue;
        siftUp(i);
    }


    //Deletes the element at index i
    void deleteKey(int i)
    {
        /**Write your code here**/
        if (i < 0 || i >= size)
        {
            throw std::invalid_argument("deleteKey() called with invalid index.");
        }

        heap[i] = heap[size-1];
        size--;
        siftDown(i);
    }

    //Prints the heap's content to the output file
    void printHeap(std::ofstream &outfile)
    {
        /**Write your code here**/
        for(int i=0; i<size; i++){
            outfile<<heap[i]<<" ";
        }
        outfile<<"\n";
    }

    //Checkes whether the Max Heap property is preserved or not. Returns true if the Max Heap property is valid and returns false otherwise.
    bool isValidMaxHeap()
    {
        /**Write your code here**/
        for(int i=0; i<size/2; i++){
            if(heap[i] < heap[2*i+1]) return false;
            else if(2*i+2<size && heap[i] < heap[2*i+2]) return false;
        }

        return true;
    }
};

#endif //MAXHEAP_H
