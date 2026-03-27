#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using i128 = __int128;

// S = K - d*a, key term is C(S+d-1, d-1)
// T = i64  (~9.2e18):
//   d=2  →  S < 9e18
//   d=3  →  S < 4e9
//   d=4  →  S < 4e6
//   d=5  →  S < 4e5
//   d=10 →  S < 60
// T = i128 (~1.7e38):
//   d=2  →  S < 1e38
//   d=3  →  S < 2e19
//   d=4  →  S < 8e12
//   d=5  →  S < 8e9
//   d=10 →  S < 1e4

template <typename T> T nCr(T n, T r) {
  if (n < r || n < 0 || r < 0) {
    return 0;
  }
  if (r > n / 2) {
    r = n - r;
  }
  T res = 1;
  for (T i = 0; i < r; i++) {
    res *= (n - i);
    res /= (i + 1);
  }
  return res;
}

// Stars and bars with inclusion-exclusion
// Time: O(d^2), Space: O(1)
template <typename T> T calc(T K, int d, T a, T b) {
  T S = K - (T)d * a;
  T B = b - a;
  if (S < 0) {
    return 0;
  }
  T ans = 0;
  for (int i = 0; i <= d; i++) {
    T sign = (i % 2 ? -1 : 1);
    T t1 = nCr<T>(d, i);
    T t2 = nCr<T>(S + d - 1 - (B + 1) * i, d - 1);
    ans += sign * t1 * t2;
  }
  return ans;
}
