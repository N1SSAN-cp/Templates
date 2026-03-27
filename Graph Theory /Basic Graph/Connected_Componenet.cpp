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

// Connected components using BFS
// Time: O(V + E)
// Returns comp[v] = component index of node v

template <typename GT> vector<int> connected_components(GT &G) {
  assert(!G.directed);

  vector<int> comp(G.n, -1);
  int num = 0;

  for (int s = 0; s < G.n; s++) {
    if (comp[s] != -1) {
      continue;
    }
    queue<int> q;
    q.push(s);
    comp[s] = num;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (auto &[to, w] : G.adj[v]) {
        if (comp[to] == -1) {
          comp[to] = num;
          q.push(to);
        }
      }
    }
    num++;
  }

  return comp;
}
