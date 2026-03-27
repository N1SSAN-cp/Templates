#include <bits/stdc++.h>
using namespace std;

// Count 1s at bit position i (0-indexed) in [1, n]
// O(1) per query
int count_set(int n, int i) {
  int cycle = 1 << (i + 1);
  int full = (n / cycle) * (1 << i);
  int rem = max(0, (n % cycle) - (1 << i) + 1);
  return full + rem;
}

int count_set_range(int L, int R, int i) {
  return count_set(R, i) - count_set(L - 1, i);
}

int main() {
  int L, R, i;
  cin >> L >> R >> i;
  cout << count_set_range(L, R, i) << "\n";
  return 0;
}
