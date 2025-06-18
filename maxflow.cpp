#include <iostream>
#include <queue>
#include <array>
#include <vector>
#define LARGE_NUMBER 2000000000 

using namespace std;

// parent array for BFS path reconstruction
int parent[20000];
int max_flow = 0;
int edges_in_flow_graph = 0;

/**
 * BFS to find an augmenting path from source to sink.
 * Returns 1 if a path is found, 0 otherwise.
 */
int BFS(vector<pair< pair<int,int>, pair<int,int> > > graph[], int V, int source, int sink) {
    int visited[V];
    for (int i = 0; i < V; i++) visited[i] = 0;
    queue<int> q;
    q.push(source);
    visited[source] = 1;
    int stop = 0;

    while (!q.empty()) {
        if (stop == 1) break;

        int current = q.front();
        q.pop();
        
        for (auto it = graph[current].begin(); it != graph[current].end(); it++) {
            int node = (it->first).first;
            if (!visited[node] && ((it->second).second > 0)) {
                parent[node] = current;
                visited[node] = 1;
                if (node == sink) {
                    stop = 1;
                    break;
                } else {
                    q.push(node);
                }
            } 
        }
    }

    return parent[sink] != -1;
} 

/**
 * Adds a directed edge to the graph with given capacity and residual capacity.
 */
void add_edge(vector<pair< pair<int,int>, pair<int,int> > > graph[], int u, int v, int flow, int cap, int res_cap) {
    graph[u].push_back({{v, flow}, {cap, res_cap}});
}

/**
 * Reconstructs the current augmenting path from sink to source.
 */
vector<pair<int, int> > get_path(vector<pair< pair<int,int>, pair<int,int> > > graph[], int sink, int source) {
    vector<pair<int,int>> path;
    int current = sink;
    while (current != source) {
        path.push_back({parent[current], current});
        current = parent[current];
    }
    return path;
}

/**
 * Finds the bottleneck (minimum residual capacity) along the path.
 */
int get_bottleneck(vector<pair< pair<int,int>, pair<int,int> > > graph[], vector<pair<int, int>> path) {
    int min_cap = LARGE_NUMBER;
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        int from = it->first, to = it->second;
        for (auto edge : graph[from]) {
            if (edge.first.first == to) {
                min_cap = min(min_cap, edge.second.second);
            }
        }
    }
    return min_cap;
}

/**
 * Pushes flow through the given path and updates forward and backward edges.
 */
void push_flow_through_path(vector<pair< pair<int,int>, pair<int,int> > > graph[], vector<pair<int, int>> path, int flow) {
    for (auto it : path) {
        int from = it.first, to = it.second;
        int new_flow = 0;

        for (auto& edge : graph[from]) {
            if (edge.first.first == to) {
                edge.first.second += flow;
                new_flow = edge.first.second;
            }
        }

        for (auto& edge : graph[to]) {
            if (edge.first.first == from) edge.first.second = -new_flow;
        }

        for (auto& edge : graph[from]) {
            if (edge.first.first == to) edge.second.second = edge.second.first - edge.first.second;
        }

        for (auto& edge : graph[to]) {
            if (edge.first.first == from) edge.second.second = edge.second.first - edge.first.second;
        }
    }
}

/**
 * Counts edges in the resulting flow graph (those with positive flow).
 */
void count_edges(vector<pair< pair<int,int>, pair<int,int> > > graph[], int V) {
    for (int i = 0; i < V; i++) {
        for (auto edge : graph[i]) {
            if (edge.first.second > 0) edges_in_flow_graph++;
        }
    }
}

/**
 * Prints edges that carry flow in the final solution.
 */
void print_flow_edges(vector<pair< pair<int,int>, pair<int,int> > > graph[], int V) {
    for (int i = 0; i < V; i++) {
        for (auto edge : graph[i]) {
            if (edge.first.second > 0) {
                cout << i + 1 << " " << edge.first.first + 1 << " " << edge.first.second << "\n";
            }
        }
    }
}

/**
 * Calculates total flow leaving the source.
 */
void extract_flow(vector<pair< pair<int,int>, pair<int,int> > > graph[], int source) {
    for (auto edge : graph[source]) {
        max_flow += edge.first.second;
    }
}

/**
 * Checks whether a reverse edge already exists in the residual graph.
 */
int is_edge(vector<pair< pair<int,int>, pair<int,int> > > graph[], int u, int v) {
    for (auto edge : graph[u]) {
        if (edge.first.first == v) return 1;
    }
    return 0;
}

int main() {
    int V, source, sink, E, u, v, c, bottleneck;

    cin >> V >> source >> sink >> E;
    source--; sink--;

    vector<pair<int, int>> original_edges;
    vector<pair< pair<int,int>, pair<int,int> > > graph[V];

    // Initialize parent array
    for (int i = 0; i < V; i++) parent[i] = -1;

    // Read edges and build forward graph
    for (int i = 0; i < E; i++) {
        cin >> u >> v >> c;
        u--; v--;
        original_edges.push_back({u, v});
        add_edge(graph, u, v, 0, c, c);
    }

    // Add reverse edges if they don't already exist
    for (auto it : original_edges) {
        if (!is_edge(graph, it.second, it.first)) {
            add_edge(graph, it.second, it.first, 0, 0, 0);
        }
    }

    // Run Edmonds-Karp loop: BFS + augment
    while (BFS(graph, V, source, sink)) {
        auto path = get_path(graph, sink, source);
        bottleneck = get_bottleneck(graph, path);
        max_flow += bottleneck;
        push_flow_through_path(graph, path, bottleneck);
        fill(parent, parent + V, -1);
    }

    // Output max flow result and flow-carrying edges
    count_edges(graph, V);
    cout << V << "\n" << source + 1 << " " << sink + 1 << " " << max_flow << "\n" << edges_in_flow_graph << "\n";
    print_flow_edges(graph, V);

    return 0;
}
