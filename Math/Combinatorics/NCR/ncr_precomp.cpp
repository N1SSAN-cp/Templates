#include <bits/stdc++.h>
using namespace std;

// nCr precomputed without mint — manual mod arithmetic
// Precompute: O(n log MOD), Query: O(1), requires prime MOD

const long long MOD = 1e9 + 7;
const int maxN = 1e6;

vector<long long> fact_v, inv_fact_v;

long long power(long long base, long long exp) {
  long long res = 1;
  base %= MOD;
  while (exp > 0) {
    if (exp & 1) {
      res = res * base % MOD;
    }
    base = base * base % MOD;
    exp >>= 1;
  }
  return res;
}

void combo_build() {
  fact_v.resize(maxN + 1);
  inv_fact_v.resize(maxN + 1);
  fact_v[0] = 1;
  for (int i = 1; i <= maxN; i++) {
    fact_v[i] = fact_v[i - 1] * i % MOD;
  }
  inv_fact_v[maxN] = power(fact_v[maxN], MOD - 2);
  for (int i = maxN - 1; i >= 0; i--) {
    inv_fact_v[i] = inv_fact_v[i + 1] * (i + 1) % MOD;
  }
}

long long nCr(int n, int r) {
  if (r < 0 || r > n) {
    return 0;
  }
  return fact_v[n] * inv_fact_v[r] % MOD * inv_fact_v[n - r] % MOD;
}

int main() {
  combo_build();
  cout << nCr(10, 3) << "\n"; // 120
}
