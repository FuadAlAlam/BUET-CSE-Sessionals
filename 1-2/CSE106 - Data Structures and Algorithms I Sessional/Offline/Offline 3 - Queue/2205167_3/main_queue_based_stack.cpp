#include "StackUsingQueue.h" // Include Linked List-based Stack implementation
#include <fstream>
#include <sstream>

using namespace std;

// Helper function to print the current stack
void printStack(Stack &stack, ofstream &outputFile)
{
    Stack tempStack;
    outputFile << "Current stack: ";
    while (!stack.isEmpty())
    {
        int value = stack.pop();
        tempStack.push(value);
    }
    while (!tempStack.isEmpty())
    {
        int value = tempStack.pop();
        stack.push(value);
        outputFile << value << " ";
    }
    outputFile << endl;

    while (!tempStack.isEmpty())
    {
        stack.push(tempStack.pop());
    }
}

void processStackOperations(Stack &stack, ifstream &inputFile, ofstream &outputFile)
{
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        int operation;
        ss >> operation;

        switch (operation)
        {
        case 11:
        { // Push operation
            int value;
            if (ss >> value)
            {
                stack.push(value);
                outputFile << "Pushed " << value << " onto the stack." << endl;
            }
            else
            {
                outputFile << "Invalid input for push." << endl;
            }
            printStack(stack, outputFile);
            break;
        }
        case 12:
        { // Pop operation
            if (!stack.isEmpty())
            {
                int poppedValue = stack.pop();
                outputFile << "Popped value: " << poppedValue << endl;
            }
            else
            {
                outputFile << "Stack is empty, nothing to pop." << endl;
            }
            printStack(stack, outputFile);
            break;
        }
        case 13:
        { // Top operation
            if (!stack.isEmpty())
            {
                outputFile << "Top value: " << stack.top() << endl;
            }
            else
            {
                outputFile << "Stack is empty, no top element." << endl;
            }
            printStack(stack, outputFile);
            break;
        }
        default:
        {
            outputFile << "Invalid operation: " << operation << endl;
            printStack(stack, outputFile);
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

    Stack stack; // Stack using queue-based implementation

    outputFile << "-------------Testing Queue based Stack-------------" << endl;
    processStackOperations(stack, inputFile, outputFile);


    inputFile.close();
    outputFile.close();

    return 0;
}
