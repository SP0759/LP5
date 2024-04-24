#include <iostream>
#include <stack>
#include <omp.h>

using namespace std;

// Graph class representing an undirected graph using adjacency list representation
class Graph {
private:
    int numVertices;          // Number of vertices
    int* adj;                 // Adjacency list represented as a 1D array

public:
    Graph(int vertices) : numVertices(vertices) {
        adj = new int[numVertices * numVertices](); // Initialize with zeros
    }

    // Add an edge between two vertices
    void addEdge(int src, int dest) {
        adj[src * numVertices + dest] = 1;
        adj[dest * numVertices + src] = 1;
    }

    // View the graph
    void viewGraph() {
        cout << "Graph:\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << " -> ";
            for (int j = 0; j < numVertices; j++) {
                if (adj[i * numVertices + j] == 1) {
                    cout << j << " ";
                }
            }
            cout << endl;
        }
    }

    // Perform Depth First Search (DFS) in parallel
    void dfs(int startVertex) {
        bool* visited = new bool[numVertices]();
        stack<int> s;

        // Mark the start vertex as visited and push it onto the stack
        visited[startVertex] = true;
        s.push(startVertex);

        while (!s.empty()) {
            int currentVertex = s.top();
            s.pop();
            cout << currentVertex << " ";

            // Push all adjacent unvisited vertices onto the stack
            #pragma omp parallel for
            for (int j = 0; j < numVertices; j++) {
                if (adj[currentVertex * numVertices + j] == 1 && !visited[j]) {
                    visited[j] = true;
                    s.push(j);
                }
            }
        }

        delete[] visited;
    }

    ~Graph() {
        delete[] adj;
    }
};

int main() {
    int numVertices;
    cout << "Enter the number of vertices in the graph: ";
    cin >> numVertices;

    // Create a graph with the specified number of vertices
    Graph graph(numVertices);

    int numEdges;
    cout << "Enter the number of edges in the graph: ";
    cin >> numEdges;

    cout << "Enter the edges (source destination):\n";
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    // View the graph
    graph.viewGraph();

    int startVertex;
    cout << "Enter the starting vertex for DFS: ";
    cin >> startVertex;

    cout << "Depth First Search (DFS): ";
    graph.dfs(startVertex);
    cout << endl;

    return 0;
}
