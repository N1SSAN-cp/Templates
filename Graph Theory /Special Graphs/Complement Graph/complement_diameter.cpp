// Complement Graph — Diameter (Two-pass BFS)
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

    auto bfs_dist = [&](int src) -> vector<int> {
        set<int> not_vis;
        for (int i = 0; i < n; i++) { not_vis.insert(i); }

        vector<int> dist(n, -1);
        queue<int> q;
        q.push(src);
        not_vis.erase(src);
        dist[src] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            set<int> remaining;
            for (int v : not_vis) {
                if (!G[u].count(v)) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                } else {
                    remaining.insert(v);
                }
            }
            not_vis = remaining;
        }
        return dist;
    };

    // pass 1: farthest from node 0
    vector<int> d1 = bfs_dist(0);
    int far = (int)(max_element(d1.begin(), d1.end()) - d1.begin());

    // pass 2: diameter from farthest node
    vector<int> d2 = bfs_dist(far);
    int diam = *max_element(d2.begin(), d2.end());

    cout << diam << '\n';
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
