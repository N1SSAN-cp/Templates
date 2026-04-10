#include <bits/stdc++.h>
using namespace std;

// Top-2 Dijkstra — two shortest distances per node with source tracking
// Time: O((V + E) log V)

template <typename T>
vector<array<pair<T, int>, 2>>
top2_dijkstra(int N, vector<tuple<int, int, T>> &edges, vector<int> srcs) {
  const T INF = numeric_limits<T>::max() / 2;
  vector<vector<tuple<int, T, int>>> adj(N);
  for (int i = 0; i < (int)edges.size(); i++) {
    auto [u, v, w] = edges[i];
    adj[u].emplace_back(v, w, i);
  }

  vector<array<pair<T, int>, 2>> dist(N, {{{INF, -1}, {INF, -1}}});
  priority_queue<tuple<T, int, int>, vector<tuple<T, int, int>>, greater<>> que;

  auto upd = [&](int v, int color, T d) {
    if (d < dist[v][0].first) {
      if (color != dist[v][0].second) {
        dist[v][1] = dist[v][0];
      }
      dist[v][0] = {d, color};
      que.emplace(d, v, color);
    } else if (color != dist[v][0].second && d < dist[v][1].first) {
      dist[v][1] = {d, color};
      que.emplace(d, v, color);
    }
  };

  for (int v : srcs) {
    upd(v, v, 0);
  }

  while (!que.empty()) {
    auto [dv, v, c] = que.top();
    que.pop();
    if (dist[v][0] != make_pair(dv, c) && dist[v][1] != make_pair(dv, c)) {
      continue;
    }
    for (auto &[to, cost, eid] : adj[v]) {
      upd(to, c, dv + cost);
    }
  }
  return dist;
}

int main() {
  int N, M; cin >> N >> M;

  vector<tuple<int, int, long long>> edges(M);
  for (int i = 0; i < M; i++) {
    int u, v; long long w;
    cin >> u >> v >> w;
    edges[i] = {u, v, w};
  }

  int K; cin >> K;
  vector<int> srcs(K);
  for (int i = 0; i < K; i++) {
    cin >> srcs[i];
  }

  auto dist = top2_dijkstra<long long>(N, edges, srcs);

  for (int i = 0; i < N; i++) {
    cout << "node " << i << " : nearest = (" << dist[i][0].first << ", src "
         << dist[i][0].second << ")"
         << " , 2nd    = (" << dist[i][1].first << ", src " << dist[i][1].second
         << ")\n";
  }
}
