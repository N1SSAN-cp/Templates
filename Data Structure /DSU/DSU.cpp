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
