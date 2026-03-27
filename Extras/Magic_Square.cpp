#include <bits/stdc++.h>
using namespace std;
// Siamese method (De la Loubère) for odd-order magic squares
// Time: O(N^2), Space: O(N^2)
vector<vector<int>> magic_square(int N) {
    assert(N % 2 == 1);
    vector<vector<int>> A(N, vector<int>(N, 0));
    int x = 0, y = N / 2;
    for (int i = 0; i < N * N; i++) {
        A[x][y] = i + 1;
        int nx = (x == 0 ? N - 1 : x - 1);
        int ny = (y == N - 1 ? 0 : y + 1);
        if (A[nx][ny] != 0) {
            nx = (x == N - 1 ? 0 : x + 1);
            ny = y;
        }
        tie(x, y) = make_pair(nx, ny);
    }
    return A;
}
