#include <bits/stdc++.h>
using namespace std;

// Bellman-Ford — Shortest Path with Negative Weights
// Time: O(V * E), dist = -1 means unreachable
// Detects negative cycles (returns empty vectors if found)
// Single source : bellman_ford(edges, n, {src})
// Multi source  : bellman_ford(edges, n, {s1, s2, ...})
// edges = list of {u, v, weight}

int main() {
    auto bellman_ford = []<typename T>(vector<array<T, 3>>& edges, int n, vector<int> srcs)
        -> pair<vector<T>, vector<int>>
    {
        vector<T> dist(n, -1);
        vector<int> par(n, -1);
        for (int s : srcs) {
            dist[s] = 0;
        }
        for (int iter = 0; iter < n - 1; iter++) {
            for (auto [u, v, w] : edges) {
                if (dist[u] == -1) { continue; }
                T nd = dist[u] + w;
                if (dist[v] == -1 || dist[v] > nd) {
                    dist[v] = nd;
                    par[v] = u;
                }
            }
        }
        // Negative cycle detection: if any dist improves on the n-th iteration,
        // a negative cycle exists — return empty vectors to signal this
        for (auto [u, v, w] : edges) {
            if (dist[u] == -1) { continue; }
            if (dist[v] == -1 || dist[v] > dist[u] + w) {
                return {{}, {}};
            }
        }
        return {dist, par};
    };

    int n, m;
    cin >> n >> m;

    vector<array<long long, 3>> edges(m);
    for (int i = 0; i < m; i++) {
        long long u, v, w;
        cin >> u >> v >> w;
        edges[i] = {u, v, w};
        // For undirected graphs, also push: edges.push_back({v, u, w});
    }

    auto [dist, par] = bellman_ford.operator()<long long>(edges, n, {0});

    if (dist.empty()) {
        cout << "Negative cycle detected!\n";
    } else {
        for (int i = 0; i < n; i++) {
            cout << "dist[" << i << "] = " << dist[i] << "\n";
        }
    }
}
