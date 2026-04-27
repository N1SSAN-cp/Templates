#include <bits/stdc++.h>
using namespace std;
/**
 * Problem: Count all cliques in a graph (including null clique)
 * Given n nodes and m edges, count every subset where every pair is connected
 * Algorithm: Meet in the Middle
 * Complexity: O((M/2) * 2^(M/2)), M <= 42
 */

const int M = 42;

int dp[1 << (M / 2)]; long long g[M];

long long countCliques(int n) {
  int k = n / 2;
  memset(dp, 0, sizeof(int) * (1 << k));
  dp[0] = 1;
  for (int i = 1; i < (1 << k); i++) {
    long long nw = (1LL << n) - 1;
    for (int j = 0; j < k; j++) {
      if ((i >> j) & 1) {
        nw &= g[j];
      }
    }
    if ((nw & i) == i) {
      dp[i] = 1;
    }
  }
  for (int i = 0; i < k; i++) {
    for (int mask = 0; mask < (1 << k); mask++) {
      if ((mask >> i) & 1) {
        dp[mask] += dp[mask ^ (1 << i)];
      }
    }
  }
  long long ans = dp[(1 << k) - 1];
  k = n - k;
  for (int i = 1; i < (1 << k); i++) {
    long long nw = (1LL << n) - 1;
    for (int j = 0; j < k; j++) {
      if ((i >> j) & 1) {
        nw &= g[n / 2 + j];
      }
    }
    long long p = (1LL * i) << (n / 2);
    if ((nw & p) == p) {
      int x = nw & ((1LL << (n / 2)) - 1);
      ans += dp[x];
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    --u;
    --v;
    g[u] |= 1LL << v;
    g[v] |= 1LL << u;
  }
  for (int i = 0; i < n; i++) {
    g[i] |= 1LL << i;
  }
  cout << countCliques(n) << '\n';
  return 0;
}
