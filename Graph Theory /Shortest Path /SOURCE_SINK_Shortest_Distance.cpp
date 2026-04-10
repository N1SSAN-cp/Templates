#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const i64 INF = 1e18;
template <typename T, typename GT>
pair<vector<T>, vector<int>> dijkstra(GT &G, vector<int> srcs) {
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
    if (d > dist[v]) {
      continue;
    }
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
}

int main() {
  int N, M; cin >> N >> M;

  vector<vector<pair<int, i64>>> G(N);

  for (int i = 0; i < M; i++) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    G[u].emplace_back(v, w);
  }

  auto build_reverse = [&](const auto &graph) {
    int n = graph.size();
    vector<vector<pair<int, i64>>> reverse_graph(n);
    for (int u = 0; u < n; u++) {
      for (auto [v, w] : graph[u]) {
        reverse_graph[v].emplace_back(u, w);
      }
    }
    return reverse_graph;
  };

  auto RG = build_reverse(G);

  int src, sink; cin >> src >> sink;

  // distances
  auto ds = dijkstra<i64>(G, {src}).first;   // src → v
  auto dt = dijkstra<i64>(RG, {sink}).first; // v → sink

  for (int i = 0; i < N; i++) {
    cout << "node " << i << " : src->node = " << (ds[i] == INF ? -1 : ds[i])
         << " , node->sink = " << (dt[i] == INF ? -1 : dt[i]) << "\n";
  }
}
