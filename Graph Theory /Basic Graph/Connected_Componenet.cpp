#include <bits/stdc++.h>
using namespace std;

// Connected Components using BFS
// Time: O(V + E)
// Returns comp[v] = component index of node v
// G[v] = list of neighbors (unweighted adjacency list)
template<typename GT>
vector<int> connected_components(GT& G) {
    int n = (int)G.size();
    vector<int> comp(n, -1);
    int num = 0;
    for (int s = 0; s < n; s++) {
        if (comp[s] != -1) { continue; }
        queue<int> q;
        q.push(s);
        comp[s] = num;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : G[v]) {
                if (comp[to] == -1) {
                    comp[to] = num;
                    q.push(to);
                }
            }
        }
        num++;
    }
    return comp;
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

    auto comp = connected_components(G);

    for (int i = 0; i < n; i++) {
        cout << "comp[" << i << "] = " << comp[i] << "\n";
    }
}
