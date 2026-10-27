#include "MaxHeap.h"
#include <fstream>
#include <sstream>

int main()
{
    MaxHeap heap;

    std::ifstream infile("input.txt");
    std::ofstream outfile("output.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        std::cerr << "Error opening input/output file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        int command, value, index;
        iss >> command;

        try
        {
            switch (command)
            {
            case 1:  //insert
                iss >> value;
                heap.insert(value);
                outfile << "Inserted " << value << " into the heap." << std::endl;
                break;
            case 2:  //extract-max
                outfile << "Extracted Max: " << heap.extractMax() << std::endl;
                break;
            case 3:  //find-max
                outfile << "Max: " << heap.findMax() << std::endl;
                break;
            case 4:  //size
                outfile << "Heap size: " << heap.getSize() << std::endl;
                break;
            case 5:  //is-empty
                outfile << "Is heap empty? " << (heap.isEmpty() ? "Yes" : "No") << std::endl;
                break;
            case 6:  //delete(index)
                iss >> index;
                heap.deleteKey(index);
                outfile << "Deleted element at index " << index << std::endl;
                break;
            case 7:  //increase-key(index, new_value)
                iss >> index >> value;
                heap.increaseKey(index, value);
                outfile << "Increased key at index " << index << " to " << value << std::endl;
                break;
            case 8:  //print-heap
                outfile << "Heap: ";
                heap.printHeap(outfile);
                break;
            case 9:  //is-valid-max-heap
                outfile << "Max Heap property is "
                        << (heap.isValidMaxHeap() ? "preserved." : "violated!")
                        << std::endl;
                break;
            default:
                outfile << "Unknown command: " << command << std::endl;
                break;
            }
        }
        catch (const std::exception &e)
        {
            outfile << "Error: " << e.what() << std::endl;
        }
    }

    infile.close();
    outfile.close();

    return 0;
}
