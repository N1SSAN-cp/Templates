#include <bits/stdc++.h>
using namespace std;

// Rollback DSU (Union by Size, no path compression)
// find: O(log n) | unite: O(log n) | rollback: O(1)
struct RollbackDSU {
  vector<int> parent, sz;
  vector<array<int, 2>> history;

  RollbackDSU(int n) : parent(n + 1), sz(n + 1, 1) {
    for (int i = 1; i <= n; i++) {
      parent[i] = i;
    }
  }

  int find(int x) const {
    while (parent[x] != x) {
      x = parent[x];
    }
    return x;
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    if (sz[a] < sz[b]) {
      swap(a, b);
    }
    history.push_back({b, sz[a]});
    sz[a] += sz[b];
    parent[b] = a;
    return true;
  }

  int ccs(int x) {
    int root = find(x);
    return sz[root];
  }

  void rollback() {
    if (history.empty()) {
      return;
    }
    auto [b, oldA] = history.back();
    history.pop_back();
    int a = parent[b];
    sz[a] = oldA;
    parent[b] = b;
  }
};

int main() {
  RollbackDSU dsu(5);

  dsu.unite(1, 2);
  dsu.unite(2, 3);

  cout << dsu.find(1) << "\n";
  cout << dsu.find(3) << "\n";
  cout << dsu.ccs(1) << "\n"; // 3

  int snap = dsu.history.size();

  dsu.unite(3, 4);
  dsu.unite(4, 5);
  cout << dsu.ccs(1) << "\n"; // 5

  while ((int)dsu.history.size() > snap) {
    dsu.rollback();
  }

  cout << dsu.ccs(1) << "\n";  // 3
  cout << dsu.find(4) << "\n"; // 4
  cout << dsu.find(5) << "\n"; // 5

  return 0;
}
