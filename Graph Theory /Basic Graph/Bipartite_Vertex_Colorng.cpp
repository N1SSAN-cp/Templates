#include <bits/stdc++.h>
using namespace std;

// Bipartite Vertex Coloring using BFS
// Time: O(V + E)
// Returns color[v] = 0 or 1 for each node
// Returns empty vector if graph is not bipartite
// G[v] = list of neighbors (unweighted adjacency list)
template<typename GT>
vector<int> bipartite_vertex_coloring(GT& G) {
    int n = (int)G.size();
    vector<int> color(n, -1);
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) { continue; }
        queue<int> q;
        q.push(s);
        color[s] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : G[v]) {
                if (color[to] == color[v]) { return {}; }
                if (color[to] == -1) {
                    color[to] = 1 - color[v];
                    q.push(to);
                }
            }
        }
    }
    return color;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> G(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    auto color = bipartite_vertex_coloring(G);

    if (color.empty()) {
        cout << "Graph is not bipartite\n";
    } else {
        for (int i = 0; i < n; i++) {
            cout << "color[" << i << "] = " << color[i] << "\n";
        }
    }
}
