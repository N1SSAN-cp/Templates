#include <bits/stdc++.h>
using namespace std;

// Sparse Table (idempotent range queries)
// build: O(n log n) | query: O(1)
template <typename T, auto func> struct SparseTable {
  static const int B = 32; // number of bits
  int n, K;
  vector<vector<T>> st;

  SparseTable(const vector<T> &a) {
    n = a.size();
    K = B - __builtin_clz(n);
    st.assign(n, vector<T>(K));
    for (int i = 0; i < n; i++) {
      st[i][0] = a[i];
    }
    for (int j = 1; j < K; j++) {
      for (int i = 0; i + (1 << j) <= n; i++) {
        st[i][j] = func(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  T query(int l, int r) {
    int j = (B - 1) - __builtin_clz(r - l + 1);
    return func(st[l][j], st[r - (1 << j) + 1][j]);
  }
};

int main() {
  vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};

//   SparseTable<int, [](int x, int y) { return min(x, y); }> st(a);

//   cout << st.query(0, 7) << "\n"; // 1
//   cout << st.query(2, 5) << "\n"; // 1
//   cout << st.query(4, 7) << "\n"; // 2
//   cout << st.query(0, 0) << "\n"; // 3

  return 0;
}
