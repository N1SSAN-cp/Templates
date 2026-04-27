#include <bits/stdc++.h>
using namespace std;

// Submask enumeration
// Time: O(3^n) over all masks of n bits

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int mask;
  cin >> mask;

  // iterate all submasks of mask (excluding 0)
  for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
    cout << sub << "\n";
  }

  return 0;
}
