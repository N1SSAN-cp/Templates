#include <bits/stdc++.h>
using namespace std;

// BFS — Unweighted Shortest Path
// Time: O(V + E), dist = -1 means unreachable

template <typename T, typename GT>
pair<vector<T>, vector<int>> bfs(GT &G, vector<int> srcs) {
  int n = (int)G.size();
  vector<T> dist(n, -1);
  vector<int> par(n, -1); queue<int> q;
  for (int s : srcs) {
    dist[s] = 0;
    q.push(s);
  }
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int u : G[v]) {
      if (dist[u] == -1) {
        dist[u] = dist[v] + 1;
        par[u] = v;
        q.push(u);
      }
    }
  }
  return {dist, par};
}

int main() {
  int n, m; cin >> n >> m;

  vector<vector<int>> G(n);
  for (int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    G[u].push_back(v); G[v].push_back(u);
  }

  auto [dist, par] = bfs<int>(G, {0});

  for (int i = 0; i < n; i++) {
    cout << "dist[" << i << "] = " << dist[i] << "\n";
  }
}
