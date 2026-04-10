#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(x) x.begin(), x.end()
#define len(x) int((x).size())

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

const int maxN = 1000;

ll dp[64][2][2][maxN];
int vis[64][2][2][maxN], cur_time;

ll C[maxN + 1][maxN + 1];

void ncr_dp_build() {
  for (int i = 0; i <= maxN; i++) {
    C[i][0] = 1;
    for (int j = 1; j <= i; j++) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
}

void solve() {
  int n, L, R, Z; cin >> n >> L >> R >> Z;

  cur_time++;

  auto recur = y_combinator(
      [&](auto self, int level, int slo, int shi, int carry) -> ll {
        if (level == 64) {
          return (carry == 0 && slo == 1 && shi == 1);
        }

        if (vis[level][slo][shi][carry] == cur_time) {
          return dp[level][slo][shi][carry];
        }

        int lo = (L >> level) & 1;
        int hi = (R >> level) & 1;
        int B = (Z >> level) & 1;

        ll ans = 0;

        for (int ones = 0; ones <= n; ones++) {
          if ((ones & 1) != B)
            continue;

          int bit = (ones + carry) & 1;
          int ncarry = (ones + carry) >> 1;

          int nslo, nshi;
          if (bit < lo) {
            // NUM is smaller
            nslo = 0;
          } else if (bit > lo) {
            // NUM is larger
            nslo = 1;
          } else {
            // TIE
            nslo = slo;
          }

          if (bit > hi) {
            // NUM is larger
            nshi = 0;
          } else if (bit < hi) {
            // NUM is smaller
            nshi = 1;
          } else {
            // TIE
            nshi = shi;
          }
          ans += C[n][ones] * self(level + 1, nslo, nshi, ncarry);
        }

        dp[level][slo][shi][carry] = ans;
        vis[level][slo][shi][carry] = cur_time;
        return ans;
      });

  cout << recur(0, 1, 1, 0) << "\n";
}

signed main() {
  ios_base::sync_with_stdio(0), cin.tie(0);
  ncr_dp_build();
  solve();
  return 0;
}
