// Complement Graph — Connected Components (DFS)
// O((N + M) log N)
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m; cin >> n >> m;
    vector<set<int>> G(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        u--; v--;
        G[u].insert(v);
        G[v].insert(u);
    }

    set<int> not_vis;
    for (int i = 0; i < n; i++) { not_vis.insert(i); }

    auto dfs = [&](auto&& self, int u) -> void {
        not_vis.erase(u);
        for (int child = 0; child < n; ) {
            auto it = not_vis.lower_bound(child);
            if (it == not_vis.end()) { break; }
            child = *it;
            if (!G[u].count(child)) {
                self(self, child);
            }
            child++;
        }
    };

    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (not_vis.count(i)) {
            dfs(dfs, i);
            ans++;
        }
    }
    cout << ans - 1 << '\n';
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int tc = 1;
    // cin >> tc;
    for (int i = 0; i < tc; i++) { solve(); }
    return 0;
}
/*
    Problem Statement:
    Observations/Questions:
    Claims / Algo :
     !Are You really sure??
     Better Implementation:
*/
