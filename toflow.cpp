#include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;

using std::cin;
using std::cout;
using std::cerr;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);

    int x, y, e, a, b;

    // Read number of source nodes (x), sink nodes (y), and edges (e)
    cin >> x >> y >> e;

    // Output graph metadata:
    // total vertices, number of sources, number of sinks, and total edges
    cout << x + y + 2 << "\n" << 1 << " " << x + y + 2 << "\n" << x + y + e << "\n";

    // Add edges from source node (1) to each of the x nodes
    for (int i = 0; i < x; i++) {
        cout << 1 << " " << i + 2 << " " << 1 << "\n";
    }

    // Add the actual edges provided in the input
    for (int i = 0; i < e; i++) {
        cin >> a >> b;
        cout << a + 1 << " " << b + 1 << " " << 1 << "\n";
    }

    // Add edges from each of the y sink nodes to the sink vertex
    for (int i = x + 2; i < x + y + 2; i++) {
        cout << i << " " << x + y + 2 << " " << 1 << "\n";
    }

    cout.flush();

    // Read and filter output from max flow result
    int trash, V, E;
    vector<string> out;
    cin >> V >> trash >> trash >> trash >> E;

    // Parse resulting flow edges, excluding source-to-sink direct path
    for (int i = 0; i < E; i++) {
        cin >> a >> b >> trash;
        if (a != 1 || b != V) {
            out.push_back(std::to_string(a) + " " + std::to_string(b) + "\n");
        }
    }

    // Output the processed flow results
    cout << x << " " << y << "\n" << out.size() << "\n";
    for (const string& i : out) {
        cout << i << "\n";
    }

    cout.flush();
    return 0;
}
