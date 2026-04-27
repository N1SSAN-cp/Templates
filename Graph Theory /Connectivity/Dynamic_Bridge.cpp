/*
Incremental Bridges (Online)

Maintains number of bridges while adding edges.

Time: O(m * alpha(n)) ~ almost O(1) per edge
Note: Undirected, edge insertions only
*/

#include <bits/stdc++.h>
using namespace std;

struct IncrementalBridges {
  int n, bridges = 0, timer = 0;

  vector<int> parent, dsu_2ecc, dsu_cc, visited, sz;

  IncrementalBridges(int n) : n(n) {
    parent.assign(n, -1);
    dsu_2ecc.resize(n);
    dsu_cc.resize(n);
    visited.assign(n, 0);
    sz.assign(n, 1);

    iota(dsu_2ecc.begin(), dsu_2ecc.end(), 0);
    iota(dsu_cc.begin(), dsu_cc.end(), 0);
  }

  int find_2ecc(int v) {
    if (v == -1)
      return -1;
    int root = v;
    while (root != dsu_2ecc[root])
      root = dsu_2ecc[root];

    while (v != root) {
      int p = dsu_2ecc[v];
      dsu_2ecc[v] = root;
      v = p;
    }
    return root;
  }

  int find_cc(int v) {
    v = find_2ecc(v);
    int root = v;
    while (root != dsu_cc[root])
      root = dsu_cc[root];

    while (v != root) {
      int p = dsu_cc[v];
      dsu_cc[v] = root;
      v = p;
    }
    return root;
  }

  void make_root(int v) {
    v = find_2ecc(v);
    int child = -1;

    while (v != -1) {
      int p = parent[v];
      if (p != -1)
        p = find_2ecc(p);

      parent[v] = child;

      child = v;
      v = p;
    }
  }

  int find_lca(int u, int v) {
    ++timer;
    while (true) {
      if (u != -1) {
        u = find_2ecc(u);
        if (visited[u] == timer)
          return u;
        visited[u] = timer;
        u = parent[u];
      }
      swap(u, v);
    }
  }

  void compress_path(int u, int lca) {
    while (u != lca) {
      int p = parent[u];
      p = (p == -1 ? -1 : find_2ecc(p));

      dsu_2ecc[u] = lca;
      bridges--;

      u = p;
    }
  }

  void add_edge(int u, int v) {
    u = find_2ecc(u);
    v = find_2ecc(v);

    if (u == v)
      return;

    int cu = find_cc(u); int cv = find_cc(v);

    if (cu != cv) {
      if (sz[cu] > sz[cv]) {
        swap(u, v);
        swap(cu, cv);
      }
      sz[cv] += sz[cu];

      bridges++;
      make_root(u);
      parent[u] = v;
      dsu_cc[cu] = cv;
    } else {
      int lca = find_lca(u, v);
      compress_path(u, lca);
      compress_path(v, lca);
    }
  }

  int get_bridge_count() const { return bridges; }
};
int main() {
  int n = 5;
  IncrementalBridges ib(n);

  ib.add_edge(0, 1); // bridges = 1
  ib.add_edge(1, 2); // bridges = 2
  ib.add_edge(2, 3); // bridges = 3

  cout << ib.get_bridge_count() << "\n"; // 3

  ib.add_edge(3, 1);                     // creates cycle → removes 2 bridges
  cout << ib.get_bridge_count() << "\n"; // 1

  ib.add_edge(3, 4);                     // new bridge
  cout << ib.get_bridge_count() << "\n"; // 2
}
