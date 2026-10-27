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

import java.util.*;

abstract class AbstractGraph {
    protected int V;
    protected ArrayList<ArrayList<Integer>> adj;

    public AbstractGraph(int vertices) {
        this.V = vertices;
        this.adj = new ArrayList<>(vertices);
        for (int i = 0; i < vertices; i++) {
            adj.add(new ArrayList<>());
        }
    }

    // Abstract methods
    public abstract void addEdge(int u, int v);
    public abstract void removeEdge(int u, int v);
    public abstract void printGraph();
    public abstract void checkBipartite(); // Method to check if the graph is bipartite
}

// Extend AbstractGraph into a concrete Graph class 
// You can use the code from previous practice session.

class Graph extends AbstractGraph {

    public Graph(int vertices) {
        super(vertices);
    }

    // Write the necessary code here

}

public class GraphPractice {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter number of vertices: ");
        int vertices = scanner.nextInt();
        Graph g = new Graph(vertices);

        // Write the necessary code here

        scanner.close();
    }
}