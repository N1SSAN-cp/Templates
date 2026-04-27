// Stirling Number of the Second Kind - Basic DP
// S(n, k) = ways to partition n labeled elements into k non-empty indistinguishable groups
// O(n * k) | n, k <= 2000, mod = 998244353
// Problem: Count ways to distribute n distinct balls into exactly k non-empty
//          indistinguishable boxes.
// Input:  n k
// Output: S(n, k)

#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

// Stirling Number of the Second Kind, O(n * k)
// returns S(n, k)
int stirling2(int n, int k) {
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            dp[i][j] = ((long long)j * dp[i-1][j] + dp[i-1][j-1]) % mod;
        }
    }
    return dp[n][k];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    cout << stirling2(n, k) << "\n";
    return 0;
}
