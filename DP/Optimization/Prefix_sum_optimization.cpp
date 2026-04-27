#include <bits/stdc++.h>
using namespace std;

/*
  PREFIX SUM OPTIMIZATION IN DP

  TRIGGER:
    recursive dp has a loop inside summing f(j) over a range, no condition on j

    int f(int i) {
        for (int j = i - L; j < i; j++)
            ans += f(j);          // sum + no condition -> prefix sum
    }

  no condition on j  ->  prefix sum   O(n)
  condition on j     ->  BIT/segtree  O(n log n)
  min/max not sum    ->  mono deque   O(n)
*/

const int MOD = 1e9 + 7;

// Frog Jump
// from index 0, jump 1 to L steps, count ways to reach n
// f(i) = sum of f(j) for j in [i-L, i-1]  variable window, no condition

// recursive + memo: O(n*L)
int L;
vector<int> memo;

int f(int i) {
  if (i == 0) {
    return 1;
  }
  if (memo[i] != -1) {
    return memo[i];
  }
  int ans = 0;
  for (int j = max(0, i - L); j < i; j++) {
    ans = (ans + f(j)) % MOD;
  }
  return memo[i] = ans;
}

// iterative + prefix sum: O(n)
int solve(int n, int L) {
  vector<int> dp(n + 1, 0), pre(n + 1, 0);
  dp[0] = pre[0] = 1;
  for (int i = 1; i <= n; i++) {
    int lo = max(0, i - L);
    dp[i] = (pre[i - 1] - (lo > 0 ? pre[lo - 1] : 0) + MOD) % MOD;
    pre[i] = (pre[i - 1] + dp[i]) % MOD;
  }
  return dp[n];
}

/*
int main() {
    int n = 10; L = 3;
    memo.assign(n + 1, -1);
    cout << f(n) << "\n";          // recursive
    cout << solve(n, L) << "\n";   // iterative + prefix sum
    return 0;
}
*/
