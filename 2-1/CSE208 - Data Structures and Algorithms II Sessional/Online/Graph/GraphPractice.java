/**
 * Jan25 CSE208 
 * Practice Session on Graphs
 * 
 * 
 * Complete the following code by implementing the methods. Please be mindful of the corner cases. 
 * Run the provided input file. 
 * Be ready to explain the generated output to the evaluator.
 * 
 * 
 * The usage of any online tool and/or any GenAI is absolutely forbidden in this course 
 * unless otherwise mentioned. 
*/

import java.util.ArrayList;

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
    public abstract int inDegree(int u);
    public abstract int outDegree(int u);
    public abstract boolean removeVertex(int u);
    public abstract void printGraph();
    public abstract void printInNeighbors(int u);
    public abstract void printOutNeighbors(int u);

    // please note that once a vertex is removed, it will not be printed in the printGraph function 
}

// extend AbstractGraph into a concrete Graph class 



public class GraphPractice {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter number of initial vertices: ");
        int initialVertices = scanner.nextInt();
        Graph g = new Graph(initialVertices);

        while (true) {
            System.out.println("\n========== MENU ==========");
            System.out.println("1. Add Edge");
            System.out.println("2. Remove Edge");
            System.out.println("3. Remove Vertex");
            System.out.println("4. Print Graph");
            System.out.println("5. Find In-Degree of a Vertex");
            System.out.println("6. Find Out-Degree of a Vertex");
            System.out.println("7. Print In-Neighbors of a Vertex");
            System.out.println("8. Print Out-Neighbors of a Vertex");
            System.out.println("9. Exit");
            System.out.print("Choose an option: ");

            int choice = scanner.nextInt();

            if (choice == 9) break;

            switch (choice) {
                case 1:
                    System.out.print("Enter source and destination vertex: ");
                    int u1 = scanner.nextInt();
                    int v1 = scanner.nextInt();
                    g.addEdge(u1, v1);
                    break;
                case 2:
                    System.out.print("Enter source and destination vertex to remove edge: ");
                    int u2 = scanner.nextInt();
                    int v2 = scanner.nextInt();
                    g.removeEdge(u2, v2);
                    break;
                case 3:
                    System.out.print("Enter vertex to remove: ");
                    int u3 = scanner.nextInt();
                    if (g.removeVertex(u3)) {
                        System.out.println("Vertex " + u3 + " removed successfully.");
                    } else {
                        System.out.println("Failed to remove vertex " + u3 + ".");
                    }
                    break;
                case 4:
                    g.printGraph();
                    break;
                case 5:
                    System.out.print("Enter vertex to find in-degree: ");
                    int v5 = scanner.nextInt();
                    System.out.println("In-degree of " + v5 + " is: " + g.inDegree(v5));
                    break;
                case 6:
                    System.out.print("Enter vertex to find out-degree: ");
                    int v6 = scanner.nextInt();
                    System.out.println("Out-degree of " + v6 + " is: " + g.outDegree(v6));
                    break;
                case 7:
                    System.out.print("Enter vertex to find in-neighbors: ");
                    int u7 = scanner.nextInt();
                    g.printInNeighbors(u7);
                    break;
                case 8:
                    System.out.print("Enter vertex to find out-neighbors: ");
                    int u8 = scanner.nextInt();
                    g.printOutNeighbors(u8);
                    break;
                default:
                    System.out.println("Invalid choice. Try again.");
            }
        }

        System.out.println("Program exited.");
        scanner.close();
    }
}