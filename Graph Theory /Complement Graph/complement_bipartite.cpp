// Complement Graph — Bipartite Check (BFS 2-coloring)
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

    vector<int> color(n, -1);
    bool ok = true;

    set<int> not_vis;
    for (int i = 0; i < n; i++) { not_vis.insert(i); }

    auto bfs_color = [&](int src) -> void {
        queue<int> q;
        q.push(src);
        not_vis.erase(src);
        color[src] = 0;

        while (!q.empty() && ok) {
            int u = q.front(); q.pop();
            set<int> remaining;
            for (int v : not_vis) {
                if (!G[u].count(v)) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        ok = false;
                    }
                } else {
                    remaining.insert(v);
                }
            }
            not_vis = remaining;
        }
    };

    for (int i = 0; i < n; i++) {
        if (not_vis.count(i)) {
            bfs_color(i);
        }
    }

    cout << (ok ? "YES" : "NO") << '\n';
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
