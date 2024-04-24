#include <iostream>
#include <queue>
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

    // Perform Breadth First Search (BFS) in parallel
    void bfs(int startVertex) {
        bool* visited = new bool[numVertices]();
        queue<int> q;

        // Mark the start vertex as visited and enqueue it
        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();
            cout << currentVertex << " ";

            // Enqueue all adjacent unvisited vertices
            #pragma omp parallel for
            for (int j = 0; j < numVertices; j++) {
                if (adj[currentVertex * numVertices + j] == 1 && !visited[j]) {
                    visited[j] = true;
                    q.push(j);
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
    cout << "Enter the starting vertex for BFS: ";
    cin >> startVertex;

    cout << "Breadth First Search (BFS): ";
    graph.bfs(startVertex);
    cout << endl;

    return 0;
}
