#include <bits/stdc++.h>
using namespace std;

// K Shortest Paths (Yen's algorithm variant)
// Time: O(K * (V + E) log V)
// edges[i] = {frm, to, cost}

template <typename T>
vector<tuple<T, vector<int>, vector<int>>>
K_shortest_path(int N, vector<tuple<int, int, T>> &edges, int s, int t, int K) {
  int M = (int)edges.size();
  vector<vector<tuple<int, T, int>>> adj(N); // adj[v] = {to, cost, edge_id}
  for (int i = 0; i < M; i++) {
    auto [frm, to, cost] = edges[i];
    adj[frm].emplace_back(to, cost, i);
  }

  vector<tuple<T, vector<int>, vector<int>>> res;
  vector<pair<vector<int>, vector<int>>> nodes;
  vector<tuple<T, vector<int>, vector<int>, int>> paths;

  nodes.push_back({vector<int>(), vector<int>()});
  vector<T> dist(N, numeric_limits<T>::max() / 2);
  vector<bool> ng_v(N); vector<bool> ng_e(M);
  vector<int> par(N, -1);

  while ((int)res.size() < K) {
    for (auto &[es, ng_es] : nodes) {
      fill(par.begin(), par.end(), -1);
      fill(ng_v.begin(), ng_v.end(), false);
      fill(ng_e.begin(), ng_e.end(), false);
      fill(dist.begin(), dist.end(), numeric_limits<T>::max() / 2);

      T pref_cost = 0;
      for (auto &x : es) {
        pref_cost += get<2>(edges[x]);
      }

      for (auto &x : es) {
        ng_v[get<0>(edges[x])] = true;
        ng_e[x] = true;
      }
      for (auto &x : ng_es) {
        ng_e[x] = true;
      }

      priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
      auto add = [&](int v, T d, int p) {
        if (d < dist[v]) {
          dist[v] = d;
          que.emplace(d, v);
          par[v] = p;
        }
      };
      int s0 = (es.empty() ? s : get<1>(edges[es.back()]));
      add(s0, pref_cost, -1);
      while (!que.empty()) {
        auto [dv, v] = que.top();
        que.pop();
        if (dv != dist[v]) {
          continue;
        }
        if (v == t) {
          break;
        }
        for (auto &[to, cost, eid] : adj[v]) {
          if (ng_e[eid] || ng_v[to]) {
            continue;
          }
          add(to, dv + cost, eid);
        }
      }
      if (par[t] == -1) {
        continue;
      }

      vector<int> add_e;
      {
        int v = t;
        while (v != s0) {
          add_e.push_back(par[v]);
          v = get<0>(edges[par[v]]);
        }
      }
      reverse(add_e.begin(), add_e.end());
      int n = (int)es.size();
      es.insert(es.end(), add_e.begin(), add_e.end());
      paths.emplace_back(dist[t], es, ng_es, n);
    }

    if (paths.empty()) {
      break;
    }

    int best_idx = -1;
    T best_cost = numeric_limits<T>::max() / 2;
    for (int i = 0; i < (int)paths.size(); i++) {
      T cost = get<0>(paths[i]);
      if (cost < best_cost) {
        best_cost = cost;
        best_idx = i;
      }
    }
    swap(paths[best_idx], paths[(int)paths.size() - 1]);
    auto [cost, es, ng_es, n] = paths.back();
    paths.pop_back();

    vector<int> vs = {s};
    for (auto &x : es) {
      vs.push_back(get<1>(edges[x]));
    }
    res.emplace_back(cost, vs, es);

    nodes.clear();
    for (int k = n; k < (int)es.size(); k++) {
      vector<int> new_es(es.begin(), es.begin() + k);
      vector<int> new_ng = ng_es;
      new_ng.push_back(es[k]);
      nodes.push_back({new_es, new_ng});
    }
  }
  return res;
}

int main() {
  int N, M; cin >> N >> M;

  vector<tuple<int, int, long long>> edges(M);
  for (int i = 0; i < M; i++) {
    int u, v; long long w;
    cin >> u >> v >> w;
    edges[i] = {u, v, w};
  }

  int s, t, K; cin >> s >> t >> K;

  auto res = K_shortest_path<long long>(N, edges, s, t, K);
  for (auto &[cost, vs, es] : res) {
    cout << "cost = " << cost << ", path: ";
    for (int v : vs) {
      cout << v << " ";
    }
    cout << "\n";
  }
}
