#include <bits/stdc++.h>
using namespace std;

// 2D Difference Array — mark all k×k submatrices, then prefix sum
// O(n*m)

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<int>> pf(n + 2, vector<int>(m + 2, 0));

    for (int i = 1; i + k - 1 <= n; i++) {
        for (int j = 1; j + k - 1 <= m; j++) {
            pf[i][j]++;
            pf[i + k][j]--;
            pf[i][j + k]--;
            pf[i + k][j + k]++;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            pf[i][j] += pf[i][j - 1];
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            pf[i][j] += pf[i - 1][j];
        }
    }

    return 0;
}
