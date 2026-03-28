// Algorithm: Hamiltonian path in tournament graph via divide & conquer merge
// Time: O(n log n)
// when to use: for a complete graph you know if check(u,v) and wants a ordering
// path 0 is the champion

#include <bits/stdc++.h>
using namespace std;

// check(u, v) returns true if u beats v
template <typename F> vector<int> hamiltonian_path(int n, F check) {
  auto dfs = [&](auto &dfs, int L, int R) -> vector<int> {
    if (R == L + 1) {
      return {L};
    }
    int M = (L + R) / 2;
    vector<int> X = dfs(dfs, L, M);
    vector<int> Y = dfs(dfs, M, R);
    vector<int> P;
    P.reserve(R - L);
    int i = 0, j = 0;
    while ((int)P.size() < R - L) {
      if (i == (int)X.size()) {
        P.push_back(Y[j++]);
      } else if (j == (int)Y.size()) {
        P.push_back(X[i++]);
      } else {
        if (check(X[i], Y[j])) {
          P.push_back(X[i++]);
        } else {
          P.push_back(Y[j++]);
        }
      }
    }
    return P;
  };
  return dfs(dfs, 0, n);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  // Example 1: transitive tournament (0 beats all, 1 beats 2&3, 2 beats 3)
  // win[u][v] = true means u beats v
  int n = 4;
  vector<vector<bool>> win = {
      {0, 1, 1, 1},
      {0, 0, 1, 1},
      {0, 0, 0, 1},
      {0, 0, 0, 0},
  };

  vector<int> path =
      hamiltonian_path(n, [&](int u, int v) { return win[u][v]; });

  for (int i = 0; i < (int)path.size(); i++) {
    cout << path[i];
    if (i + 1 < (int)path.size()) {
      cout << " -> ";
    }
  }
  cout << "\n";
}
