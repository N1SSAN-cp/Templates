#include <bits/stdc++.h>
using namespace std;

// nCr using Pascal's Triangle DP
// Pascal DP: O(n^2) build, O(1) query

const int maxN = 3000;
long long C[maxN + 1][maxN + 1];

void ncr_dp_build(int n) {
  for (int i = 0; i <= n; i++) {
    C[i][0] = 1;
    for (int j = 1; j <= i; j++) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
}

long long nCr(int n, int r) {
  if (r < 0 || r > n) {
    return 0;
  }
  return C[n][r];
}

/*
    ncr_dp_build(1000);
    nCr(10, 3);   // 120
*/

int main() {
  ncr_dp_build(maxN);
  cout << nCr(10, 3) << "\n";   // 120
  cout << nCr(maxN, 1) << "\n"; // 3000
  cout << nCr(maxN, 0) << "\n"; // 1
}
