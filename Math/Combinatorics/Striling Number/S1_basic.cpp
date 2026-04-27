// Stirling Number of the First Kind - Basic DP
// [n, k] = number of permutations of n elements with exactly k cycles
// O(n * k) | n, k <= 2000, mod = 998244353
// Problem: Count permutations of {1, 2, ..., n} that have exactly k disjoint cycles.
// Input:  n k
// Output: [n, k]

#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

// Stirling Number of the First Kind (unsigned), O(n * k)
// returns [n, k]
int stirling1(int n, int k) {
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            dp[i][j] = ((long long)(i - 1) * dp[i-1][j] + dp[i-1][j-1]) % mod;
        }
    }
    return dp[n][k];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    cout << stirling1(n, k) << "\n";
    return 0;
}
