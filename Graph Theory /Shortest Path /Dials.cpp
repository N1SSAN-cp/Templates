#include <bits/stdc++.h>
using namespace std;

// Dial's Algorithm — Dijkstra with bucket queue for integer weights
// Time: O(V * W + E), W = max edge weight
// Use when W is small integer and N * W fits in memory (~1e7)
// Better than Dijkstra when W is small, worse when W is large
// W = 0/1 → use 0/1 BFS | W = 1 → use BFS | negative → use Bellman-Ford
// not verified

template <typename T>
pair<vector<T>, vector<int>> dial(int N, vector<tuple<int, int, T>> &edges,
                                  int s) {
  vector<vector<pair<int, T>>> adj(N);
  T W = 0;
  for (auto &[u, v, w] : edges) {
    adj[u].emplace_back(v, w);
    W = max(W, w);
  }

  assert((long long)N * W <= 1e7);  // bucket array size guard
  assert((int)edges.size() <= 1e6); // edge count guard

  int sz = N * W + 2;
  vector<int> S(sz, -1), T_arr(sz, -1);
  vector<int> prv(N), nxt(N);
  vector<T> dist(N, (T)(W * N + 1));
  vector<int> par(N, -1);
  dist[s] = 0;

  auto add = [&](int v) {
    T d = dist[v];
    prv[v] = T_arr[d];
    if (T_arr[d] != -1) {
      nxt[T_arr[d]] = v;
    }
    T_arr[d] = v;
    if (S[d] == -1) {
      S[d] = v;
    }
    nxt[v] = -1;
  };

  auto rm = [&](int v) {
    T d = dist[v];
    if (prv[v] != -1) {
      nxt[prv[v]] = nxt[v];
    }
    if (nxt[v] != -1) {
      prv[nxt[v]] = prv[v];
    }
    if (S[d] == v) {
      S[d] = nxt[v];
    }
    if (T_arr[d] == v) {
      T_arr[d] = prv[v];
    }
  };

  for (int v = 0; v < N; v++) {
    add(v);
  }

  for (int d = 0; d < N * W; d++) {
    int v = S[d];
    while (v != -1) {
      for (auto &[to, cost] : adj[v]) {
        T dw = d + cost;
        if (dw < dist[to]) {
          par[to] = v;
          rm(to);
          dist[to] = dw;
          add(to);
        }
      }
      v = nxt[v];
    }
  }

  for (int v = 0; v < N; v++) {
    if (dist[v] > N * W) {
      dist[v] = -1;
    }
  }
  return {dist, par};
}

int main() {
  int N, M; cin >> N >> M;

  vector<tuple<int, int, int>> edges(M);
  for (int i = 0; i < M; i++) {
    int u, v, w; cin >> u >> v >> w;
    edges[i] = {u, v, w};
  }

  auto [dist, par] = dial<int>(N, edges, 0);

  for (int i = 0; i < N; i++) {
    cout << "dist[" << i << "] = " << dist[i] << "\n";
  }
}
