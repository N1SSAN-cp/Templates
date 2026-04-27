#include <bits/stdc++.h>
using namespace std;

// Offline GCD subarray counting - BIT range add, point query
// O(n log^2 n + q log n)
struct BIT {
  int n;
  vector<int> bit;
  BIT(int n) : n(n), bit(n + 1, 0) {}
  void add(int i, int v) {
    for (++i; i <= n; i += i & -i) {
      bit[i] += v;
    }
  }
  void range_add(int l, int r, int v) {
    if (l > r) {
      return;
    }
    add(l, v);
    add(r + 1, -v);
  }
  int query(int i) {
    int s = 0;
    for (++i; i > 0; i -= i & -i) {
      s += bit[i];
    }
    return s;
  }
};
