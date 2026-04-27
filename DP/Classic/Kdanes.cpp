#include <bits/stdc++.h>
using namespace std;
// #define int long long

template <bool maximum_mode>
int optimal_subarray_sum(const vector<int>& A) {
    int sum = 0, best = 0;

    for (const auto& a : A) {
        sum = maximum_mode ? max(sum + a, 0) : min(sum + a, 0);
        best = maximum_mode ? max(best, sum) : min(best, sum);
    }

    return best;
}

void solve() {

}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
