#include <bits/stdc++.h>
using namespace std;

// Subset Sum — Bitset Optimization
// O(n * maxN / 64)

const int maxN = 100001;

int main() {
  int n, W; cin >> n >> W;

  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  bitset<maxN> dp;
  dp[0] = 1;

  for (int i = 0; i < n; i++) {
    dp |= dp << a[i];
  }

  for (int s = 0; s <= W; s++) {
    if (dp[s]) {
      cout << s << "\n";
    }
  }
}
