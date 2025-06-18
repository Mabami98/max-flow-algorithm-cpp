/**
 * Example input/output handler for the max flow lab in the ADK course.
 *
 * Converts a bipartite graph into a flow network,
 * sends it to a max flow solver, and interprets the result
 * as a maximum bipartite matching.
 *
 * Author: Per Austrin (minor edits/comments added)
 */

#include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;

using std::cin;
using std::cout;
using std::cerr;

int X; // number of nodes on the left side
int Y; // number of nodes on the right side

/**
 * Converts the bipartite graph into a flow network format.
 * Source is node 1, sink is node V (X+Y+2).
 * Each edge has capacity 1.
 */
void writeFlowGraph(int e, vector<string> edges) {
    int V = X + Y + 2, s = 1, t = V;

    cout << V << "\n" << s << " " << t << "\n" << X + Y + e << "\n";

    // Edges from source to left-side nodes
    for (int i = 0; i < X; i++)
        cout << 1 << " " << i + 2 << " 1\n";

    // Internal edges of bipartite graph
    for (string i : edges)
        cout << i << "\n";

    // Edges from right-side nodes to sink
    for (int i = X + 2; i < X + Y + 2; i++)
        cout << i << " " << X + Y + 2 << " 1\n";

    cout.flush();
}

/**
 * Reads the bipartite graph from standard input and sends it to writeFlowGraph.
 */
void readBipartiteGraph() {
    int E;
    cin >> X >> Y >> E;
    vector<string> edges;

    for (int i = 0; i < E; ++i) {
        int a, b;
        cin >> a >> b;
        // shift both endpoints by 1 to match flow network indexing
        edges.push_back(std::to_string(a + 1) + " " + std::to_string(b + 1) + " 1");
    }

    writeFlowGraph(E, edges);
}

/**
 * Outputs the final matching result in bipartite graph format.
 */
void writeBipMatchSolution(vector<string> maxMatching) {
    int maxMatch = maxMatching.size();
    cout << X << " " << Y << "\n" << maxMatch << "\n";

    for (string edge : maxMatching)
        cout << edge << "\n";
}

/**
 * Reads the solution to the max flow problem and extracts a valid matching.
 * Ignores edges directly to/from source and sink.
 */
void readMaxFlowSolution() {
    int v, e, s, t, flow;
    vector<string> maxMatching;

    cin >> v >> s >> t >> flow >> e;

    for (int i = 0; i < e; ++i) {
        int u, v, f;
        cin >> u >> v >> f;

        if ((u != s) && (v != t) && (v != s) && (u != t)) {
            if (f == 1) {
                // shift node indices back down (reverse earlier shift)
                if (u < v)
                    maxMatching.push_back(std::to_string(u - 1) + " " + std::to_string(v - 1));
                else
                    maxMatching.push_back(std::to_string(v - 1) + " " + std::to_string(u - 1));
            }
        }
    }

    writeBipMatchSolution(maxMatching);
}

int main(void) {
    std::ios::sync_with_stdio(false);
    cin.tie(0);

    readBipartiteGraph();
    readMaxFlowSolution();

    return 0;
}
