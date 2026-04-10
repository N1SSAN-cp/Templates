#include <bits/stdc++.h>
using namespace std;

// nCr using multiplicative formula
// Formula: O(r), long long overflows beyond ~62

const int maxN = 60;

long long nCr(int n, int r) {
  if (n < r || n < 0 || r < 0) {
    return 0;
  }
  if (r > n / 2) {
    r = n - r;
  }
  long long res = 1;
  for (int i = 0; i < r; i++) {
    res *= (n - i);
    res /= (i + 1);
  }
  return res;
}

int main() {
  cout << nCr(10, 3) << "\n";      // 120
  cout << nCr(maxN, 1) << "\n";    // 60
  cout << nCr(maxN, maxN) << "\n"; // 1
}
