#include <bits/stdc++.h>
using namespace std;

// 0/1 BFS — Shortest Path with edge weights 0 or 1
// Time: O(V + E), dist = -1 means unreachable

template <typename T, typename GT>
pair<vector<T>, vector<int>> bfs01(GT &G, vector<int> srcs) {
  int n = (int)G.size();
  vector<T> dist(n, -1);
  vector<int> par(n, -1); deque<int> q;
  for (int s : srcs) {
    dist[s] = 0;
    q.push_front(s);
  }
  while (!q.empty()) {
    int v = q.front();
    q.pop_front();
    for (auto [u, w] : G[v]) {
      T nd = dist[v] + w;
      if (dist[u] == -1 || dist[u] > nd) {
        dist[u] = nd;
        par[u] = v;
        if (w == 0) {
          q.push_front(u);
        } else {
          q.push_back(u);
        }
      }
    }
  }
  return {dist, par};
}

int main() {
  int n, m; cin >> n >> m;

  vector<vector<array<int, 2>>> G(n);
  for (int i = 0; i < m; i++) {
    int u, v, w; cin >> u >> v >> w;
    G[u].push_back({v, w});
    G[v].push_back({u, w});
  }

  auto [dist, par] = bfs01<int>(G, {0});

  for (int i = 0; i < n; i++) {
    cout << "dist[" << i << "] = " << dist[i] << "\n";
  }
}
