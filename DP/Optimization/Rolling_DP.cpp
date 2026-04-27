#include <bits/stdc++.h>
using namespace std;

/*
 * Recursive -> Iterative DP + Rolling Optimization
 *
 * Steps:
 *   (1) Same dp states + memo table
 *   (2) Fill base cases manually; set identity value for memo table
 *   (3) Fix order of iteration
 *         a) recur(level+1, _) -> iterate BACKWARD: n-1 -> 0
 *         b) recur(level-1, _) -> iterate FORWARD:  0 -> n-1
 *   (4) Perform same dp transitions
 *
 * Rolling DP:
 *   When: #states > 1e7 && < 1e8  (time ok, space not)
 *   Condition: level depends only on level+1 (not level+2, +3, ...)
 *   How: replace dp[level] with dp[level & 1]
 *        space: O(n * m) -> O(m)
 */

// ---------------------------------------------------------------------
// Recursive (top-down memoization)
// State: dp[level][sum][taken]
// O(n * m) time, O(n * m) space
// ---------------------------------------------------------------------
int arr_r[1005];
vector<vector<array<int, 2>>> dp_rec;

int recur(int level, int sum, int taken, int n, int m) {
    //basecase
    if (level == n) {
        return (int)(sum == 0 && taken);
    }
    //cache checking
    if (dp_rec[level][sum][taken] != -1) {
        return dp_rec[level][sum][taken];
    }
    //compute
    int ok = 0;
    //transition
    ok |= recur(level + 1, (sum + arr_r[level]) % m, 1, n, m); // take
    ok |= recur(level + 1, sum, taken, n, m);                   // skip
    //save and return 
    return dp_rec[level][sum][taken] = ok;
}

void solve_recursive() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> arr_r[i];

    dp_rec.assign(n, vector<array<int, 2>>(m, array<int, 2>{-1, -1}));
    cout << recur(0, 0, 0, n, m) << "\n";
}

// ---------------------------------------------------------------------
// Iterative + Rolling DP (bottom-up)
// State: dp[level][sum][taken]
// O(n * m) time, O(m) space
// ---------------------------------------------------------------------
void solve_iterative() {
    int n, m;
    cin >> n >> m;

    int arr[n];
    for (int i = 0; i < n; i++) cin >> arr[i];

    int dp[2][m][2];

    // Base case: level == n
    for (int sum = 0; sum < m; sum++) {
        dp[n & 1][sum][0] = 0;
        dp[n & 1][sum][1] = (sum == 0) ? 1 : 0;
    }

    for (int level = n - 1; level >= 0; level--) {
        for (int sum = 0; sum < m; sum++) {
            for (int taken = 0; taken <= 1; taken++) {
                int ok = 0;
                ok |= dp[(level + 1) & 1][(sum + arr[level]) % m][1]; // take
                ok |= dp[(level + 1) & 1][sum][taken];                 // skip
                dp[level & 1][sum][taken] = ok;
            }
        }
    }

    cout << dp[0][0][0] << "\n";
}

/*
 * Index mapping (recursive -> iterative rolling):
 *
 *   dp[level][s][t]     ->  dp[level & 1][s][t]
 *   dp[level+1][s][t]   ->  dp[(level+1) & 1][s][t]
 *   base case level==n  ->  init dp[n & 1] before loop
 *   recur(0, 0, 0)      ->  dp[0][0][0]
 */

int main() {
    // solve_recursive();
    solve_iterative();
}
