#include "QueueLinkedList.h" // Include Linked List-based Queue implementation
#include <fstream>
#include <sstream>

using namespace std;

// Helper function to print the current queue
void printQueue(Queue &queue, ofstream &outputFile)
{
    Queue tempQueue;
    outputFile << "Current queue: ";
    while (!queue.isEmpty())
    {
        int value = queue.dequeue();
        tempQueue.enqueue(value);
    }
    while (!tempQueue.isEmpty())
    {
        int value = tempQueue.dequeue();
        queue.enqueue(value);
        outputFile << value << " ";
    }
    outputFile << endl;

    while (!tempQueue.isEmpty())
    {
        queue.enqueue(tempQueue.dequeue());
    }
}

void processQueueOperations(Queue &queue, ifstream &inputFile, ofstream &outputFile)
{
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        int operation;
        ss >> operation;

        switch (operation)
        {
        case 1:
        { // Enqueue operation
            int value;
            if (ss >> value)
            {
                queue.enqueue(value);
                outputFile << "Enqueued " << value << " onto the queue." << endl;
            }
            else
            {
                outputFile << "Invalid input for enqueue." << endl;
            }
            printQueue(queue, outputFile);
            break;
        }
        case 2:
        { // Dequeue operation
            if (!queue.isEmpty())
            {
                int dequeuedValue = queue.dequeue();
                outputFile << "Dequeued value: " << dequeuedValue << endl;
            }
            else
            {
                outputFile << "Queue is empty, nothing to dequeue." << endl;
            }
            printQueue(queue, outputFile);
            break;
        }
        case 3:
        { // Peek operation
            if (!queue.isEmpty())
            {
                outputFile << "Peek value: " << queue.peek() << endl;
            }
            else
            {
                outputFile << "Queue is empty, no peek element." << endl;
            }
            printQueue(queue, outputFile);
            break;
        }
        case 4:
        { // Length operation
            outputFile << "Current queue length: " << queue.length() << endl;
            printQueue(queue, outputFile);
            break;
        }
        case 5:
        { // IsEmpty operation
            outputFile << "Is queue empty? " << (queue.isEmpty() ? "Yes" : "No") << endl;
            printQueue(queue, outputFile);
            break;
        }
        case 6:
        { // Clear operation
            queue.clear();
            outputFile << "Cleared the queue." << endl;
            printQueue(queue, outputFile);
            break;
        }
        default:
        {
            outputFile << "Invalid operation: " << operation << endl;
            printQueue(queue, outputFile);
            break;
        }
        }
        outputFile << "-------------------" << endl;
    }
}

int main()
{
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        cerr << "Error opening input or output file." << endl;
        return 1;
    }

    Queue queueArray; // Queue using Array-based implementation

    outputFile << "-------------Testing Linked-List based Queue-------------" << endl;
    processQueueOperations(queueArray, inputFile, outputFile);


    inputFile.close();
    outputFile.close();

    return 0;
}
