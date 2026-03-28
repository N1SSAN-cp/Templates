#include <bits/stdc++.h>
using namespace std;

// DSU with Path Compression (halving) + Union by Size + Edge Count
// find: O(α(n)) | merge: O(α(n))
struct UnionFind {
  int num, comp;
  vector<int> comp_size, edge_count, par;

  UnionFind(int n)
      : num(n), comp(n), comp_size(n, 1), edge_count(n, 0), par(n) {
    iota(par.begin(), par.end(), 0);
  }

  int find(int x) {
    while (par[x] != x) {
      par[x] = par[par[x]];
      x = par[x];
    }
    return x;
  }

  int operator[](int x) { return find(x); }

  bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      edge_count[x]++;
      return false;
    }
    comp--;
    if (comp_size[x] < comp_size[y]) {
      swap(x, y);
    }
    edge_count[x] += edge_count[y] + 1;
    comp_size[x] += comp_size[y];
    par[y] = x;
    return true;
  }

  vector<int> find_all() {
    vector<int> A(num);
    for (int i = 0; i < num; i++) {
      A[i] = find(i);
    }
    return A;
  }
};

// Kruskal's MST
// O(E log E)
// returns {mst_weight, mst_edges}, mst_edges are indices into edges[]
// if graph is disconnected, returns forest spanning each component
pair<int, vector<int>> kruskal(int n, vector<array<int, 3>> &edges) {
  // edges[i] = {weight, u, v}
  vector<int> idx(edges.size());
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(),
       [&](int a, int b) { return edges[a][0] < edges[b][0]; });

  UnionFind uf(n);
  int mst_weight = 0;
  vector<int> mst_edges;
  for (int i : idx) {
    auto [w, u, v] = edges[i];
    if (uf.merge(u, v)) {
      mst_weight += w;
      mst_edges.push_back(i);
    }
  }
  return {mst_weight, mst_edges};
}

int main() {
  int n = 4;
  // {weight, u, v}
  vector<array<int, 3>> edges = {
      {1, 0, 1}, {4, 0, 2}, {3, 1, 2}, {2, 1, 3}, {5, 2, 3}};

  auto [w, mst] = kruskal(n, edges);
  cout << "MST weight: " << w << "\n";
  cout << "MST edges:\n";
  for (int i : mst) {
    cout << edges[i][1] << " - " << edges[i][2] << " (w=" << edges[i][0]
         << ")\n";
  }
}
