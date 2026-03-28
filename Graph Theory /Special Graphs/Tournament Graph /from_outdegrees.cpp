// Algorithm: reconstruct tournament adjacency matrix from out-degree sequence
// Time: O(n^2)
// Precondition: A must be a valid score sequence (Landau's condition)

#include <bits/stdc++.h>
using namespace std;

// Returns n x n matrix where mat[i][j] = '1' means i beats j, '0' means j beats i
vector<string> from_outdegrees(vector<int> A) {
    int N = (int)A.size();
    // F[x] = count of players currently having out-degree x
    vector<int> F(N);
    for (int x : A) {
        F[x]++;
    }
    vector<string> mat(N, string(N, '-'));
    for (int i = 0; i < N; i++) {
        // lose = how many of the remaining players (j > i) must beat player i
        int lose = N - 1 - i - A[i];
        F[A[i]]--;
        // initially assume i beats everyone after it
        for (int j = i + 1; j < N; j++) {
            mat[i][j] = '1';
            mat[j][i] = '0';
        }
        // flip 'lose' of them to losses, picking players with highest current out-degree
        int p = N;
        for (int x = N - 1; x >= 0; x--) {
            int k = min(lose, F[x]);
            p -= F[x];
            for (int j = p; j < p + k; j++) {
                mat[i][j] = '0';
                mat[j][i] = '1';
            }
            lose -= k;
        }
        // update remaining players' out-degrees based on what we decided
        for (int j = i + 1; j < N; j++) {
            if (mat[i][j] == '0') {
                F[A[j]]--;
                A[j]--;
                F[A[j]]++;
            }
        }
    }
    return mat;
}

// Check: does player i have the correct out-degree in the matrix?
bool verify(const vector<string>& mat, const vector<int>& deg) {
    int N = (int)mat.size();
    for (int i = 0; i < N; i++) {
        int cnt = 0;
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == '1') { cnt++; }
        }
        if (cnt != deg[i]) { return false; }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Example 1: 4 players, scores [0,1,2,3] — a transitive tournament
    {
        vector<int> deg = {0, 1, 2, 3};
        vector<string> mat = from_outdegrees(deg);
        cout << "Example 1: out-degrees [0, 1, 2, 3]\n";
        cout << "Matrix (row i beats col j if '1'):\n";
        for (int i = 0; i < (int)mat.size(); i++) {
            cout << "  player " << i << ": " << mat[i] << "\n";
        }
        cout << "Verified: " << (verify(mat, {0, 1, 2, 3}) ? "yes" : "NO") << "\n\n";
    }

    // Example 2: 3 players, scores [1,1,1] — a 3-cycle
    {
        vector<int> deg = {1, 1, 1};
        vector<string> mat = from_outdegrees(deg);
        cout << "Example 2: out-degrees [1, 1, 1]\n";
        cout << "Matrix:\n";
        for (int i = 0; i < (int)mat.size(); i++) {
            cout << "  player " << i << ": " << mat[i] << "\n";
        }
        cout << "Verified: " << (verify(mat, {1, 1, 1}) ? "yes" : "NO") << "\n\n";
    }

    // Example 3: 5 players, scores [0,1,2,3,4]
    {
        vector<int> deg = {0, 1, 2, 3, 4};
        vector<string> mat = from_outdegrees(deg);
        cout << "Example 3: out-degrees [0, 1, 2, 3, 4]\n";
        cout << "Matrix:\n";
        for (int i = 0; i < (int)mat.size(); i++) {
            cout << "  player " << i << ": " << mat[i] << "\n";
        }
        cout << "Verified: " << (verify(mat, {0, 1, 2, 3, 4}) ? "yes" : "NO") << "\n";
    }

    return 0;
}
