#include <bits/stdc++.h>
using namespace std;

// Binary Indexed Tree (Fenwick Tree) — O(log n) update/query, 1-based indexing
struct BIT {
  int n;
  vector<int> tree;
  BIT(int n) : n(n), tree(n + 1, 0) {}
  void update(int p, int val) {
    for (; p <= n; p += p & -p) {
      tree[p] += val;
    }
  }
  int query(int p) {
    int res = 0;
    for (; p > 0; p -= p & -p) {
      res += tree[p];
    }
    return res;
  }
  int query(int l, int r) { return query(r) - query(l - 1); }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  // indices are 1-based: valid range is [1..n]
  BIT bit(10);
  bit.update(3, 5); // arr[3] += 5
  bit.update(7, 3); // arr[7] += 3
  bit.update(5, 2); // arr[5] += 2

  cout << bit.query(5) << "\n";    // prefix sum [1..5] = 7
  cout << bit.query(3, 7) << "\n"; // range sum  [3..7] = 10

  return 0;
}
