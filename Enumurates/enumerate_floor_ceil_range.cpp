#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

// Iterates over intervals [lo, hi) where floor(N/x) == q
// Time: O(sqrt(N))  Space: O(1)
template <typename F>
void enumurate_floor_range(i64 N, F f, bool Q_ASC = true, bool INCLUDE_Q_IS_0 = false) {
    i64 sq = sqrtl(N);
    i64 n = (sq * sq + sq <= N ? sq : sq - 1);

    if (Q_ASC) {
        if (INCLUDE_Q_IS_0) f(0, N + 1, LLONG_MAX);
        for (i64 q = 1; q <= n; ++q) {
            f(q, N / (q + 1) + 1, N / q + 1);
        }
        for (i64 l = sq; l >= 1; --l) {
            f(N / l, l, l + 1);
        }
    } else {
        for (i64 l = 1; l <= sq; ++l) {
            f(N / l, l, l + 1);
        }
        for (i64 q = n; q >= 1; --q) {
            f(q, N / (q + 1) + 1, N / q + 1);
        }
        if (INCLUDE_Q_IS_0) f(0, N + 1, LLONG_MAX);
    }
}
// Iterates over intervals [lo, hi) where ceil(N/x) == q
// Time: O(sqrt(N))  Space: O(1)
template <typename F> void ceil_range(i64 N, F f) {
  i64 sq = sqrtl(N);
  i64 prev = LLONG_MAX;

  for (i64 q = 1; q <= sq; ++q) {
    i64 x = (N + q - 1) / q;
    f(q, x, prev);
    prev = x;
  }

  i64 n = (sq * sq + sq >= N ? sq : sq + 1);
  if (N == sq * sq)
    --n;

  for (i64 l = n; l >= 1; --l)
    f((N + l - 1) / l, l, l + 1);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // sum of floor(20/x)
  i64 N = 20, total = 0;

  enumurate_floor_range(N, [&](i64 q, i64 lo, i64 hi) { total += q * (hi - lo); });

  cout << "sum of floor(20/x) = " << total << "\n";

  // print ceil groups
  cout << "\nceil(10/x) groups:\n";

  ceil_range(10, [](i64 q, i64 lo, i64 hi) {
    cout << "ceil(10/x) = " << q << " for x in [" << lo << ", " << hi << ")\n";
  });
}
