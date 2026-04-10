#include <bits/stdc++.h>
using namespace std;

// AND of all elements in [l, r] — O(log n)
int andRange(int l, int r) {
  int shfcnt = 0;
  while (l != r) {
    l >>= 1;
    r >>= 1;
    shfcnt++;
  }
  return r << shfcnt;
}

// OR of all elements in [l, r] — O(log n)
int orRange(int l, int r) {
  int shfcnt = 0;
  while (l != r) {
    l >>= 1;
    r >>= 1;
    shfcnt++;
  }
  return (r << shfcnt) | ((1 << shfcnt) - 1);
}

// XOR of [0, x] using cycle-of-4 pattern — O(1)
int xorUpto(int x) {
  if (x % 4 == 0) {
    return x;
  }
  if (x % 4 == 1) {
    return 1;
  }
  if (x % 4 == 2) {
    return x + 1;
  }
  return 0;
}

// XOR of all elements in [l, r] — O(1)
int xorRange(int l, int r) { return xorUpto(l - 1) ^ xorUpto(r); }

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  return 0;
}
