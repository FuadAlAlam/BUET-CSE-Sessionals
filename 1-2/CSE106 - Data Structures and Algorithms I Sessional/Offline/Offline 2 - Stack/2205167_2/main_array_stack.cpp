#include "StackArray.h" // Include Array-based Stack implementation
#include <cstdio>       // For FILE*, fopen, fscanf, fprintf

// Helper function to print the current stack
void printStack(Stack &stack, FILE *outputFile)
{
    Stack tempStack;
    fprintf(outputFile, "Current stack: ");
    while (!stack.isEmpty())
    {
        int value = stack.pop();
        tempStack.push(value);
    }
    while (!tempStack.isEmpty())
    {
        int value = tempStack.pop();
        stack.push(value);
        fprintf(outputFile, "%d ", value);
    }
    fprintf(outputFile, "\n");

    while (!tempStack.isEmpty())
    {
        stack.push(tempStack.pop());
    }
}

void processStackOperations(Stack &stack, FILE *inputFile, FILE *outputFile)
{
    int operation;
    while (fscanf(inputFile, "%d", &operation) != EOF)
    {
        switch (operation)
        {
        case 1:
        { // Push operation
            int value;
            if (fscanf(inputFile, "%d", &value) == 1)
            {
                stack.push(value);
                fprintf(outputFile, "Pushed %d onto the stack.\n", value);
            }
            else
            {
                fprintf(outputFile, "Invalid input for push.\n");
            }
            printStack(stack, outputFile);
            break;
        }
        case 2:
        { // Pop operation
            if (!stack.isEmpty())
            {
                int poppedValue = stack.pop();
                fprintf(outputFile, "Popped value: %d\n", poppedValue);
            }
            else
            {
                fprintf(outputFile, "Stack is empty, nothing to pop.\n");
            }
            printStack(stack, outputFile);
            break;
        }
        case 3:
        { // Top operation
            if (!stack.isEmpty())
            {
                fprintf(outputFile, "Top value: %d\n", stack.top());
            }
            else
            {
                fprintf(outputFile, "Stack is empty, no top element.\n");
            }
            printStack(stack, outputFile);
            break;
        }
        case 4:
        { // Length operation
            fprintf(outputFile, "Current stack length: %d\n", stack.length());
            printStack(stack, outputFile);
            break;
        }
        case 5:
        { // IsEmpty operation
            fprintf(outputFile, "Is stack empty? %s\n", stack.isEmpty() ? "Yes" : "No");
            printStack(stack, outputFile);
            break;
        }
        case 6:
        { // Clear operation
            stack.clear();
            fprintf(outputFile, "Cleared the stack.\n");
            printStack(stack, outputFile);
            break;
        }
        default:
        {
            fprintf(outputFile, "Invalid operation: %d\n", operation);
            printStack(stack, outputFile);
            break;
        }
        }
        fprintf(outputFile, "-------------------\n");
    }
}

int main()
{
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    if (inputFile == NULL || outputFile == NULL)
    {
        perror("Error opening input or output file");
        return 1;
    }

    Stack stackArray; // Stack using Array-based implementation

    fprintf(outputFile, "-------------Testing Array-based Stack-------------\n");
    processStackOperations(stackArray, inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
