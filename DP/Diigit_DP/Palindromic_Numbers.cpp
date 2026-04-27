#include <bits/stdc++.h>
using namespace std;

#define int long long

template <class Fun>
class y_combinator_result {
    Fun fun_;

   public:
    template <class T>
    explicit y_combinator_result(T&& fun) : fun_(std::forward<T>(fun)) {}
    template <class... Args>
    decltype(auto) operator()(Args&&... args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template <class Fun>
decltype(auto) y_combinator(Fun&& fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }

int dp[20][2][2][3][3];
int vis[20][2][2][3][3]; int cur_time;

void solve() {
    string l, r; cin >> l >> r;
    if (stoll(l) > stoll(r)) {
        swap(l, r);
    }
    int lenR = r.size(), lenL = l.size();
    cur_time++;
    int ans = 0;
    for (int len = lenL; len <= lenR; len++) {
        cur_time++;
        string L = lenL == len ? l : string(1, '1') + string(len - 1, '0');
        string R = lenR == len ? r : string(len, '9');
        int lm = (len + 1) / 2;
        auto recur = y_combinator([&](auto self, int level, int tlo, int thi, int slo, int shi) -> int {
            if (level == lm) {
                if (tlo && slo == 1) {
                    return 0;
                }
                if (thi && shi == 2) {
                    return 0;
                }
                return 1;
            }
            if (vis[level][tlo][thi][slo][shi] == cur_time)
                return dp[level][tlo][thi][slo][shi];
            int lo = tlo ? (L[level] - '0') : 0;
            int hi = thi ? (R[level] - '0') : 9;
            int ans = 0;
            for (int d = lo; d <= hi; d++) {
                bool ntlo = tlo && (d == lo);
                bool nthi = thi && (d == hi);
                int nslo = slo, nshi = shi;
                int op = len - 1 - level;
                if (d > L[op] - '0') {
                    nslo = 2;
                }
                if (d < L[op] - '0') {
                    nslo = 1;
                }
                if (d > R[op] - '0') {
                    nshi = 2;
                }
                if (d < R[op] - '0') {
                    nshi = 1;
                }
                ans = (ans + self(level + 1, ntlo, nthi, nslo, nshi));
            }

            dp[level][tlo][thi][slo][shi] = ans;
            vis[level][tlo][thi][slo][shi] = cur_time;
            return ans;
        });
        ans += recur(0, 1, 1, 0, 0);
    }

    cout << ans << "\n";
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int tc = 1;
    cin >> tc;
    for (int i = 0; i < tc; i++) {
        cout << "Case " << i + 1 << ": ";
        solve();
    }
    return 0;
}
