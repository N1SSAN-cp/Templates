// Complement Graph — BFS Traversal
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

    auto bfs = [&](int src) -> vector<int> {
        vector<int> order;
        queue<int> q;
        q.push(src);
        not_vis.erase(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            set<int> remaining;
            for (int v : not_vis) {
                if (!G[u].count(v)) {
                    q.push(v);
                } else {
                    remaining.insert(v);
                }
            }
            not_vis = remaining;
        }
        return order;
    };

    // BFS from every unvisited component
    for (int i = 0; i < n; i++) {
        if (not_vis.count(i)) {
            vector<int> order = bfs(i);
            for (int node : order) { cout << node + 1 << ' '; }
            cout << '\n';
        }
    }
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
