#include <iostream>
#include <queue>
#include <array>
#include <vector>
#define LARGE_NUMBER 2000000000 

using namespace std;

// BFS parent array
int parent[20000];
int max_flow = 0;
int edges_in_flow_graph = 0;

// Store original input edges for reference
vector<pair<int,int>> original_edges;

/**
 * Standard BFS to find augmenting path from source to sink.
 */
int BFS(vector<pair< pair<int,int>, pair<int,int> > > graph[], int V, int source, int sink) {
    int visited[V];
    fill(visited, visited + V, 0);
    queue<int> q;
    q.push(source);
    visited[source] = 1;
    int stop = 0;

    while (!q.empty() && !stop) {
        int current = q.front(); q.pop();
        for (auto it : graph[current]) {
            int node = it.first.first;
            if (!visited[node] && it.second.second > 0) {
                parent[node] = current;
                visited[node] = 1;
                if (node == sink) {
                    stop = 1;
                    break;
                }
                q.push(node);
            }
        }
    }

    return parent[sink] != -1;
}

/**
 * Adds a directed edge with flow and capacity attributes.
 */
void add_edge(vector<pair< pair<int,int>, pair<int,int> > > graph[], int u, int v, int flow, int cap, int res_cap) {
    graph[u].push_back({{v, flow}, {cap, res_cap}});
}

/**
 * Reconstructs augmenting path using the parent array.
 */
vector<pair<int, int>> get_path(vector<pair< pair<int,int>, pair<int,int> > > graph[], int sink, int source) {
    vector<pair<int,int>> path;
    int current = sink;
    while (current != source) {
        path.push_back({parent[current], current});
        current = parent[current];
    }
    return path;
}

/**
 * Determines the bottleneck (minimum residual capacity) in a path.
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
 * Pushes flow along the path and updates forward/backward edges.
 */
void push_flow_through_path(vector<pair< pair<int,int>, pair<int,int> > > graph[], vector<pair<int, int>> path, int flow) {
    int new_flow = 0;
    for (auto it : path) {
        int from = it.first, to = it.second;

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
 * Prints final matching (edges with flow = 1), excluding source/sink edges.
 */
void print_flow_edges(vector<pair< pair<int,int>, pair<int,int> > > graph[], int V, int s, int t) {
    for (int i = 0; i < V; i++) {
        for (auto edge : graph[i]) {
            int j = edge.first.first;
            if (i != s && j != t && j != s && i != t) {
                if (edge.first.second == 1) {
                    if (i < j) cout << i << " " << j << "\n";
                    else      cout << j << " " << i << "\n";
                }
            }
        }
    }
}

/**
 * Utility to check whether edge u→v exists in graph.
 */
int is_edge(vector<pair< pair<int,int>, pair<int,int> > > graph[], int u, int v) {
    for (auto edge : graph[u]) {
        if (edge.first.first == v) return 1;
    }
    return 0;
}

/**
 * Reads the bipartite edges and builds the flow network.
 * Source = 0, Sink = X + Y + 1
 */
void readBipartiteEdges(vector<pair< pair<int,int>, pair<int,int> > > graph[], int X, int Y, int E) {
    for (int i = 0; i < X; i++) {
        original_edges.push_back({0, i + 1});
        add_edge(graph, 0, i + 1, 0, 1, 1);
    }

    for (int i = 0; i < E; ++i) {
        int a, b;
        cin >> a >> b;
        add_edge(graph, a, b, 0, 1, 1);
        original_edges.push_back({a, b});
    }

    for (int i = X + 1; i < X + Y + 1; i++) {
        add_edge(graph, i, X + Y + 1, 0, 1, 1);
        original_edges.push_back({i, X + Y + 1});
    }
}

/**
 * Main function: reads input, builds graph, runs Edmonds-Karp, prints matching.
 */
int main() {
    int E, X, Y, V;
    cin >> X >> Y >> E;
    V = X + Y + 2;

    vector<pair< pair<int,int>, pair<int,int> > > graph[V];
    readBipartiteEdges(graph, X, Y, E);

    int source = 0, sink = V - 1;
    for (int i = 0; i < V; i++) parent[i] = -1;

    // Add reverse edges if not already present
    for (auto edge : original_edges) {
        int from = edge.first, to = edge.second;
        if (!is_edge(graph, to, from)) {
            add_edge(graph, to, from, 0, 0, 0);
        }
    }

    // Run max flow algorithm
    while (BFS(graph, V, source, sink)) {
        auto path = get_path(graph, sink, source);
        int bottleneck = get_bottleneck(graph, path);
        max_flow += bottleneck;
        push_flow_through_path(graph, path, bottleneck);
        fill(parent, parent + V, -1);
    }

    // Output result
    cout << X << " " << Y << "\n" << max_flow << "\n";
    print_flow_edges(graph, V, source, sink);

    return 0;
}
