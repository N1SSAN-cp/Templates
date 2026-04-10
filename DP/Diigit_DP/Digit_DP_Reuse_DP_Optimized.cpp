#include <bits/stdc++.h>
using namespace std;

/*
Total Time =
    (DP states × transitions)          ← reusable part
  + (queries × num_of_digit(n) × 10)      ← tight part
*/

template <class Fun> class y_combinator_result {
  Fun fun_;

public:
  template <class T>
  explicit y_combinator_result(T &&fun) : fun_(std::forward<T>(fun)) {}
  template <class... Args> decltype(auto) operator()(Args &&...args) {
    return fun_(std::ref(*this), std::forward<Args>(args)...);
  }
};
template <class Fun> decltype(auto) y_combinator(Fun &&fun) {
  return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

const int MOD = 1e9 + 7;
int dp[10001][11]; bool computed[10001][11];

void solve() {
  string L, R; cin >> L >> R;
  int n = R.size();

  auto fix_length = [](string &s, int len) {
    if (s.size() < len)
      s = string(len - s.size(), '0') + s;
  };
  fix_length(L, n);

  auto recur = y_combinator(
      [&](auto self, int level, bool tlo, bool thi, int last) -> int {
        if (level == n)
          return 1;

        if (!tlo && !thi && computed[level][last])
          return dp[level][last];

        int lo = tlo ? (L[level] - '0') : 0;
        int hi = thi ? (R[level] - '0') : 9;
        int ans = 0;

        for (int d = lo; d <= hi; d++) {
          if (d == last)
            continue;
          bool ntlo = tlo && d == lo;
          bool nthi = thi && d == hi;
          ans = (ans + self(level + 1, ntlo, nthi, d)) % MOD;
        }

        if (!tlo && !thi) {
          dp[level][last] = ans;
          computed[level][last] = true;
        }

        return ans;
      });

  cout << recur(0, 1, 1, 10) << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
