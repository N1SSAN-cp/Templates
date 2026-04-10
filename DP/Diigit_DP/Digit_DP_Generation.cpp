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

int dp[19][2][2][2];
int vis[19][2][2][2], cur_time;

void solve() {
  string L, R; cin >> L >> R;
  int n = R.size();

  auto fix_length = [&](string &s, int len) {
    if (s.size() < len)
      s = string(len - s.size(), '0') + s;
  };
  fix_length(L, n);

  cur_time++;

  auto recur = y_combinator(
      [&](auto self, int level, bool tlo, bool thi, bool started) -> long long {
        if (level == n)
          return 1;

        if (vis[level][tlo][thi][started] == cur_time && !tlo && !thi)
          return dp[level][tlo][thi][started];

        int lo = tlo ? (L[level] - '0') : 0;
        int hi = thi ? (R[level] - '0') : 9;

        long long ans = -1;
        for (int d = lo; d <= hi; d++) {
          bool ntlo = tlo && (d == lo);
          bool nthi = thi && (d == hi);
          bool nstarted = started || (d > 0);
          long long prod = (started || d > 0)
                               ? d * self(level + 1, ntlo, nthi, nstarted)
                               : self(level + 1, ntlo, nthi, nstarted);
          ans = max(ans, prod);
        }

        if (!tlo && !thi) {
          dp[level][tlo][thi][started] = ans;
          vis[level][tlo][thi][started] = cur_time;
        }
        return ans;
      });

  string num = "";
  y_combinator([&](auto self, int level, int tlo, int thi, int started,
                   string &res) -> void {
    if (level == n) {
      return;
    }
    int lo = tlo ? (L[level] - '0') : 0;
    int hi = thi ? (R[level] - '0') : 9;
    i64 ans = -1;
    int bestD = -1;

    for (int i = lo; i <= hi; i++) {
      int ntlo = tlo && i == lo;
      int nthi = thi && i == hi;
      int nstarted = started || i > 0;
      int THIS = 0;
      if (!started && i == 0) {
        THIS = recur(level + 1, ntlo, nthi, nstarted);
      } else {
        THIS = i * recur(level + 1, ntlo, nthi, nstarted);
      }
      if (THIS > ans) {
        bestD = i;
        ans = THIS;
      }
    }

    int ntlo = tlo && bestD == lo;
    int nthi = thi && bestD == hi;
    int nstarted = started || bestD > 0;
    res += ('0' + bestD);
    self(level + 1, ntlo, nthi, nstarted, res);
  })(0, 1, 1, 0, num);

  auto rem_leading = [](string &num) {
    int start = num.find_first_not_of('0');
    if (start == -1) {
      num = "0";
    } else {
      num = num.substr(start);
    }
  };
  rem_leading(num);
  cout << num << "\n";
}
signed main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int tc = 1;
  // cin >> tc;
  for (int i = 0; i < tc; i++) {
    solve();
  }
  return 0;
}
