#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct Graph {
  int n;
  bool directed;
  vector<vector<array<T, 2>>> adj; // adj[u] = {{to, weight}, ...}
  vector<array<int, 2>> edges;     // edges[id] = {frm, to}
  Graph(int n, bool dir = false) : n(n), adj(n), directed(dir) {}
  void addEdge(int u, int v, T w = 1) {
    edges.push_back({u, v});
    adj[u].push_back({v, w});
    if (!directed) {
      adj[v].push_back({u, w});
    }
  }
};

// Bipartite vertex coloring using BFS
// Time: O(V + E)
// Returns empty if graph is not bipartite

template <typename GT> vector<int> bipartite_vertex_coloring(GT &G) {
  assert(!G.directed);

  vector<int> color(G.n, -1);
  bool ok = true;

  for (int s = 0; s < G.n; s++) {
    if (color[s] != -1) {
      continue;
    }
    queue<int> q;
    q.push(s);
    color[s] = 0;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (auto &[to, w] : G.adj[v]) {
        if (color[to] == color[v]) {
          ok = false;
          break;
        }
        if (color[to] == -1) {
          color[to] = 1 - color[v];
          q.push(to);
        }
      }
    }
  }

  if (!ok) {
    return {};
  }
  return color;
}
