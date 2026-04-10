#include <bits/stdc++.h>
using namespace std;

#define i64 long long
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

const int maxN = 1e6 + 10;

i64 dp[10001][2][2][11];

void solve() {
  string L, R; cin >> L >> R;
  int n = R.size();

  auto fix_length = [](string &s, int len) {
    if (s.size() < len) {
      s = string(len - s.size(), '0') + s;
    }
  };
  fix_length(L, n);
  memset(dp, -1, sizeof(dp));

  auto recur =
      y_combinator([&](auto self, int level, int tlo, int thi, int prev) {
        if (level == n) {
          return 1ll;
        }
        if (dp[level][tlo][thi][prev] != -1) {
          return dp[level][tlo][thi][prev];
        }

        int lo = tlo ? (L[level] - '0') : 0;
        int hi = thi ? (R[level] - '0') : 9;
        i64 ans = 0;

        for (int i = lo; i <= hi; i++) {
          int ntlo = tlo && i == lo;
          int nthi = thi && i == hi;
          if (i == prev) {
            continue;
          }
          int nprev = (prev == 10 && i == 0) ? 10 : i;
          ans += self(level + 1, ntlo, nthi, nprev);
        }

        return dp[level][tlo][thi][prev] = ans;
      });

  cout << (recur(0, 1, 1, 10)) << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
