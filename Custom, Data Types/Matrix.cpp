#include <bits/stdc++.h>
using namespace std;

struct Matrix {
  int n; long long mod;
  vector<vector<long long>> a;

  Matrix(int n, long long mod)
      : n(n), mod(mod), a(n, vector<long long>(n, 0)) {}

  Matrix operator*(const Matrix &o) const {
    Matrix res(n, mod);
    for (int i = 0; i < n; i++) {
      for (int k = 0; k < n; k++) {
        if (a[i][k] == 0) {
          continue;
        }
        for (int j = 0; j < n; j++) {
          res.a[i][j] = (res.a[i][j] + a[i][k] * o.a[k][j]) % mod;
        }
      }
    }
    return res;
  }

  // Matrix exponentiation: O(n^3 log p)
  Matrix pow(long long p) const {
    Matrix res(n, mod), base = *this;
    for (int i = 0; i < n; i++) {
      res.a[i][i] = 1;
    }
    while (p > 0) {
      if (p & 1) {
        res = res * base;
      }
      base = base * base;
      p >>= 1;
    }
    return res;
  }
};
int main() {
  int n, m; long long k, mod;
  cin >> n >> m >> k >> mod;

  vector<vector<int>> adj(n);
  for (int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    adj[u].push_back(v);
  }

  int src, dst; cin >> src >> dst;

  // Number of paths of length k in graph: O(n^3 log k)
  Matrix M(n, mod);
  for (int u = 0; u < n; u++) {
    for (int v : adj[u]) {
      M.a[u][v] = 1;
    }
  }
  Matrix R = M.pow(k);
  cout << R.a[src][dst] << "\n";
  return 0;
}
