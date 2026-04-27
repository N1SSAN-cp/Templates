// Partition number: count + enumerate partitions of n into at most k parts
// Time: O(n*k) for count, O(exponential) for enumeration

#include <bits/stdc++.h>
using namespace std;

int countPartitions(int n, int k) {
    vector<vector<int>> dp(k + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= k; i++) {
        dp[i][0] = 1;
    }
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j <= n; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= i) {
                dp[i][j] += dp[i][j - i];
            }
        }
    }
    return dp[k][n];
}

void enumerate(int n, int k, int maxPart, vector<int>& cur) {
    if (n == 0) {
        for (int x : cur) { cout << x << " "; }
        cout << "\n";
        return;
    }
    if (k == 0 || maxPart == 0) { return; }
    for (int i = min(n, maxPart); i >= 1; i--) {
        cur.push_back(i);
        enumerate(n - i, k - 1, i, cur);
        cur.pop_back();
    }
}

int main() {
    int n, k;
    cin >> n >> k;

    cout << "Count: " << countPartitions(n, k) << "\n";
    cout << "Partitions:\n";
    vector<int> cur;
    enumerate(n, k, n, cur);
    return 0;
}