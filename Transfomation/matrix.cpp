#include <bits/stdc++.h>
using namespace std;

// Matrix transformations — O(n*m) each

// n×m -> m×n
vector<vector<int>> transpose(const vector<vector<int>>& a) {
    int n = a.size(), m = a[0].size();
    vector<vector<int>> res(m, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[j][i] = a[i][j];
        }
    }
    return res;
}

// n×m -> m×n
vector<vector<int>> rotate90cw(const vector<vector<int>>& a) {
    auto res = transpose(a);
    for (auto& row : res) {
        reverse(row.begin(), row.end());
    }
    return res;
}
// a[i][j] goes into diag[i+j], k in [0, n+m-2]
vector<vector<int>> antiDiagonals(const vector<vector<int>>& a) {
    int n = a.size(), m = a[0].size();
    vector<vector<int>> diag(n + m - 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            diag[i + j].push_back(a[i][j]);
        }
    }
    return diag;
}
// a[i][j] goes into diag[i-j+(m-1)], k in [0, n+m-2]
vector<vector<int>> mainDiagonals(const vector<vector<int>>& a) {
    int n = a.size(), m = a[0].size();
    vector<vector<int>> diag(n + m - 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            diag[i - j + (m - 1)].push_back(a[i][j]);
        }
    }
    return diag;
}
