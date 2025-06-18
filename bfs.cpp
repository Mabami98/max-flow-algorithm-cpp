#include <iostream>
#include <queue>
#include <array>

using namespace std;

/**
 * Runs BFS on a graph to find a path from source to sink.
 * Returns an array of parent pointers representing the path.
 */
int* BFS(vector<pair<int, pair<int, int>>> graph[], int V, int source, int sink) {
    int visited[V];
    fill(visited, visited + V, 0);
    queue<int> q;

    q.push(source);
    visited[source] = 1;

    int* parent = new int[V];
    for (int i = 0; i < V; i++) parent[i] = -1;

    while (!q.empty()) {
        int current = q.front(); q.pop();
        for (auto it = graph[current].begin(); it != graph[current].end(); ++it) {
            int node = it->first;
            if (!visited[node]) {
                parent[node] = current;
                visited[node] = 1;
                if (node == sink) return parent;
                q.push(node);
            }
        }
    }
    return parent;
}

/**
 * Prints all edges in the graph with their capacities and current flows.
 */
void print_graph(vector<pair<int, pair<int, int>>> graph[], int size, int source, int sink) {
    cout << "Edges in graph:\n";
    for (int i = 0; i < size; i++) {
        for (auto it : graph[i]) {
            cout << "(" << i << "-" << it.first << "), Capacity: " << it.second.first
                 << ", Flow: " << it.second.second << "\n";
        }
    }
}

/**
 * Adds a directed edge from u to v with given capacity and initial flow.
 */
void add_edge(vector<pair<int, pair<int, int>>> graph[], int u, int v, int cap, int flow) {
    graph[u].push_back({v, {cap, flow}});
}

/**
 * Updates the flow of edge u → v.
 */
void update_flow(vector<pair<int, pair<int, int>>> graph[], int u, int v, int new_flow) {
    for (auto& edge : graph[u]) {
        if (edge.first == v) {
            edge.second.second = new_flow;
        }
    }
}

/**
 * Updates the capacity of edge u → v.
 */
void update_capacity(vector<pair<int, pair<int, int>>> graph[], int u, int v, int new_capacity) {
    for (auto& edge : graph[u]) {
        if (edge.first == v) {
            edge.second.first = new_capacity;
        }
    }
}

int main() {
    // Number of vertices, source and sink nodes
    int V = 5, source = 3, sink = 1;
    vector<pair<int, pair<int, int>>> graph[V];

    // Define test graph: add_edge(graph, from, to, capacity, flow)
    add_edge(graph, 3, 2, 1, 0);
    add_edge(graph, 2, 4, 1, 0);
    add_edge(graph, 4, 1, 1, 0);
    add_edge(graph, 3, 4, 1, 0);
    add_edge(graph, 2, 0, 1, 0);

    // Run BFS to find augmenting path
    int* parent = BFS(graph, V, source, sink);
    vector<pair<int, int>> path;

    int current = sink;
    while (current != source) {
        path.push_back({parent[current], current});
        current = parent[current];
    }

    // Print the path from source to sink
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        cout << "(" << it->first << "-" << it->second << ")\n";
    }

    // Report success or failure
    if (parent[sink] != -1)
        cout << "Found a path with BFS (see above)\n";
    else
        cout << "Did not find a path with BFS\n";

    return 0;
}
