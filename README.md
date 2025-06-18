# Max Flow Algorithms & Bipartite Matching

This repository contains a set of C++ implementations related to network flow algorithms, focusing primarily on the **Ford-Fulkerson method** (via the Edmonds-Karp variant) and its application to **maximum bipartite matching**. The code was developed as part of a university course on algorithms and data structures.

---

## Contents

- `maxflow.cpp`  
  Core implementation of the Edmonds-Karp algorithm for finding the maximum flow in a directed graph with capacities.

- `maxbipartitematch.cpp`  
  Builds a bipartite graph as a flow network and computes a maximum matching using max flow.

- `toflow.cpp` and `reducetoflow.cpp`  
  Convert between bipartite graphs and flow networks. Useful for testing and system I/O integration.

- `bfs.cpp`  
  Minimal test utility for BFS pathfinding and edge manipulation in flow networks.

---

## How It Works

The algorithms follow the classic Ford-Fulkerson method:
1. Use **BFS** to find augmenting paths.
2. Determine the **bottleneck capacity** on each path.
3. Update the flow and residual capacities.
4. Repeat until no more augmenting paths can be found.

The bipartite matching is implemented as a special case of max flow, where:
- The left set of nodes is connected to a source.
- The right set is connected to a sink.
- All edges have unit capacity.

---

## Compilation

All files are written in standard C++. To compile, use `g++` or any C++17-compatible compiler.

