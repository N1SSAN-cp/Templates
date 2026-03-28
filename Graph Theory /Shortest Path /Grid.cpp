#include <bits/stdc++.h>
using namespace std;

const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

int main() {
    int n, m;
    cin >> n >> m;

    vector<string> Grid(n);
    for (auto& row : Grid) cin >> row;

    auto node   = [&](int x, int y)           { return x * m + y; };
    auto coords = [&](int v) -> pair<int, int> { return {v / m, v % m}; };
    auto inside = [&](int x, int y)           { return 0 <= x && x < n && 0 <= y && y < m; };
    auto free   = [&](int x, int y)           { return Grid[x][y] != '#'; };

    auto build_graph = [&]() {
        vector<vector<int>> G(n * m);
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < m; y++) {
                if (!free(x, y)) continue;
                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (inside(nx, ny) && free(nx, ny)) {
                        G[node(x, y)].push_back(node(nx, ny));
                    }
                }
            }
        }
        return G;
    };

    auto G = build_graph();

    auto bfs = [&](vector<vector<int>>& G, vector<int> srcs) {
        vector<int> dist(n * m, -1);
        queue<int> q;
        for (int s : srcs) { dist[s] = 0; q.push(s); }
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u : G[v]) {
                if (dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    q.push(u);
                }
            }
        }
        return dist;
    };

    auto dist = bfs(G, {node(0, 0)});

    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            cout << dist[node(x, y)] << "\t";
        }
        cout << "\n";
    }
}
