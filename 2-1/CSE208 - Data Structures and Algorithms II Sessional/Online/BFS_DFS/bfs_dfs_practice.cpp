/**
 * Jan25 CSE208 
 * Practice Session on BFS/DFS
 * 
 * 
 * Complete the following code by implementing the functions. Please be mindful of the corner cases. 
 * Run the provided input file. 
 * Be ready to explain the generated output to the evaluator.
 * 
 * 
 * The usage of any online tool and/or any GenAI is absolutely forbidden in this course 
 * unless otherwise mentioned. 
 * 
 * 
 * You may consider the following:
 *  
 * BFS(Graph, start_node):
    # Create a queue to store nodes to be explored
    # Create a set to track visited nodes
    
    # Start from the initial node and enqueue it
   
    # While the queue is not empty
        # Dequeue a node from the queue
        # Process the current node (e.g., print it or store it)
        # For each neighbor of the current node
            # If the neighbor has not been visited
                # Mark the neighbor as visited and enqueue it
 * 
 * 
 * DFS(Graph, current_node, visited):
    # Mark the current node as visited
    # Process the current node (e.g., print it or store it)
    
    # For each neighbor of the current node
        # If the neighbor has not been visited
            # Recursively call DFS on the neighbor
            DFS(Graph, neighbor, visited)
 *
 *
 *  **-**
*/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    // You can use the code from previous practice session.
    Graph(int vertices);
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    void printGraph();

    // Method to check if the graph is bipartite
    void checkBipartite(){
        // Write the necessary code here
     }
 };
 
 int main() {
     // Write the necessary code here
 }