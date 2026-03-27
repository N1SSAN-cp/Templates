#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const i64 MOD = 1e9 + 7;
const int MAXN = 2e6 + 5;
// Bounds:
//   lower : K >= d*a  →  S >= 0
//   upper : S + d - 1 < MAXN  →  K - d*a + d - 1 < MAXN
//   d     : unlimited algorithmically, O(d) time
//   S     : S < MAXN - d + 1  (e.g. MAXN=2e6 → S < 2e6 for any d)
//   MOD   : answer always in [0, MOD-1], no overflow concern
i64 fact[MAXN], inv_fact[MAXN];

i64 power(i64 a, i64 b) {
  i64 res = 1;
  a %= MOD;
  while (b > 0) {
    if (b & 1) {
      res = res * a % MOD;
    }
    a = a * a % MOD;
    b >>= 1;
  }
  return res;
}

void precompute() {
  fact[0] = 1;
  for (int i = 1; i < MAXN; i++) {
    fact[i] = fact[i - 1] * i % MOD;
  }
  inv_fact[MAXN - 1] = power(fact[MAXN - 1], MOD - 2);
  for (int i = MAXN - 2; i >= 0; i--) {
    inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
  }
}

i64 nCr(i64 n, i64 r) {
  if (n < r || n < 0 || r < 0) {
    return 0;
  }
  return fact[n] % MOD * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

// Stars and bars with inclusion-exclusion
// Time: O(d), Space: O(MAXN)
i64 calc(i64 K, int d, i64 a, i64 b) {
  i64 S = K - (i64)d * a;
  i64 B = b - a;
  if (S < 0) {
    return 0;
  }
  i64 ans = 0;
  for (int i = 0; i <= d; i++) {
    i64 sign = (i % 2 ? MOD - 1 : 1);
    i64 term = nCr(d, i) * nCr(S + d - 1 - (B + 1) * i, d - 1) % MOD;
    ans = (ans + sign * term) % MOD;
  }
  return ans;
}
