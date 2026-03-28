#include <bits/stdc++.h>
using namespace std;

// Dijkstra — Non-negative Weighted Shortest Path
// Time: O((V + E) log V), dist = -1 means unreachable
// Single source : dijkstra(G, {src})
// Multi source  : dijkstra(G, {s1, s2, ...})
// G[v] = list of {neighbor, weight}

int main() {
    auto dijkstra = []<typename T, typename GT>(GT& G, vector<int> srcs)
        -> pair<vector<T>, vector<int>>
    {
        int n = (int)G.size();
        vector<T> dist(n, -1);
        vector<int> par(n, -1);
        priority_queue<array<T, 2>, vector<array<T, 2>>, greater<>> pq;
        for (int s : srcs) {
            dist[s] = 0;
            pq.push({0, s});
        }
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();
            if (d > dist[v]) { continue; }
            for (auto [u, w] : G[v]) {
                T nd = dist[v] + w;
                if (dist[u] == -1 || dist[u] > nd) {
                    dist[u] = nd;
                    par[u] = v;
                    pq.push({nd, u});
                }
            }
        }
        return {dist, par};
    };

    int n, m;
    cin >> n >> m;

    vector<vector<array<int, 2>>> G(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        G[u].push_back({v, w});
        G[v].push_back({u, w});
    }

    auto [dist, par] = dijkstra.operator()<long long>(G, {0});

    for (int i = 0; i < n; i++) {
        cout << "dist[" << i << "] = " << dist[i] << "\n";
    }
}
