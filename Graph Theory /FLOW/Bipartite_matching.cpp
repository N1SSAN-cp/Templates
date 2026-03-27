#include <bits/stdc++.h>
using namespace std;

struct Graph {
  int n;
  bool directed;
  vector<vector<array<int, 2>>> adj; // adj[u] = {{to, weight}, ...}
  vector<array<int, 2>> edges;       // edges[id] = {frm, to}
  Graph(int n, bool dir = false) : n(n), adj(n), directed(dir) {}
  void addEdge(int u, int v, int w = 1) {
    edges.push_back({u, v});
    adj[u].push_back({v, w});
    if (!directed) {
      adj[v].push_back({u, w});
    }
  }
};

// Kosaraju's SCC — iterative to avoid stack overflow on large graphs
// returns {num_components, comp[v]} in topological order (0 = source side)
pair<int, vector<int>> scc(int n, vector<vector<int>> &adj) {
  vector<vector<int>> radj(n);
  for (int v = 0; v < n; v++) {
    for (int u : adj[v]) {
      radj[u].push_back(v);
    }
  }
  // pass 1: compute finish order
  vector<int> order;
  vector<bool> vis(n);
  for (int s = 0; s < n; s++) {
    if (vis[s])
      continue;
    stack<pair<int, int>> st; // {node, edge_index}
    vis[s] = true, st.push({s, 0});
    while (!st.empty()) {
      auto &[v, i] = st.top();
      if (i < (int)adj[v].size()) {
        int u = adj[v][i++];
        if (!vis[u]) {
          vis[u] = true, st.push({u, 0});
        }
      } else {
        order.push_back(v), st.pop();
      }
    }
  }

  // pass 2: label components on reverse graph
  vector<int> comp(n, -1);
  int k = 0;
  for (int i = (int)order.size() - 1; i >= 0; i--) {
    int s = order[i];
    if (comp[s] != -1) {
      continue;
    }
    stack<int> st;
    st.push(s), comp[s] = k;
    while (!st.empty()) {
      int v = st.top();
      st.pop();
      for (int u : radj[v]) {
        if (comp[u] == -1) {
          comp[u] = k, st.push(u);
        }
      }
    }
    k++;
  }
  return {k, comp};
}

// BFS 2-coloring — returns {} if not bipartite
vector<int> bipartite_coloring(const Graph &G) {
  vector<int> color(G.n, -1);
  for (int s = 0; s < G.n; s++) {
    if (color[s] != -1) {
      continue;
    }
    color[s] = 0;
    queue<int> que;
    que.push(s);
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (auto &e : G.adj[v]) {
        int to = e[0];
        if (color[to] == -1) {
          color[to] = 1 - color[v];
          que.push(to);
        } else if (color[to] == color[v]) {
          return {};
        }
      }
    }
  }
  return color;
}

struct BipartiteMatching {
  int N;
  Graph &G;
  vector<int> color, dist, match, vis;

  BipartiteMatching(Graph &G) : N(G.n), G(G), dist(G.n, -1), match(G.n, -1) {
    color = bipartite_coloring(G);
    assert(!color.empty());
    run();
  }

  BipartiteMatching(Graph &G, vector<int> color)
      : N(G.n), G(G), color(color), dist(G.n, -1), match(G.n, -1) {
    run();
  }

  void run() {
    while (1) {
      bfs();
      vis.assign(N, false);
      int flow = 0;
      for (int v = 0; v < N; v++) {
        if (!color[v] && match[v] == -1 && dfs(v)) {
          flow++;
        }
      }
      if (!flow) {
        break;
      }
    }
  }

  void bfs() {
    dist.assign(N, -1);
    queue<int> que;
    for (int v = 0; v < N; v++) {
      if (!color[v] && match[v] == -1) {
        dist[v] = 0;
        que.push(v);
      }
    }
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (auto &e : G.adj[v]) {
        int to = e[0];
        dist[to] = 0;
        int w = match[to];
        if (w != -1 && dist[w] == -1) {
          dist[w] = dist[v] + 1;
          que.push(w);
        }
      }
    }
  }

  bool dfs(int v) {
    vis[v] = 1;
    for (auto &e : G.adj[v]) {
      int to = e[0];
      int w = match[to];
      if (w == -1 || (!vis[w] && dist[w] == dist[v] + 1 && dfs(w))) {
        match[to] = v;
        match[v] = to;
        return true;
      }
    }
    return false;
  }

  // matched pairs — each pair listed once
  vector<pair<int, int>> matching() {
    vector<pair<int, int>> res;
    for (int v = 0; v < N; v++) {
      if (match[v] != -1 && v < match[v]) {
        res.emplace_back(v, match[v]);
      }
    }
    return res;
  }

  // minimum vertex cover (König's theorem) — size == matching size
  vector<int> vertex_cover() {
    vector<int> res;
    for (int v = 0; v < N; v++) {
      if (color[v] ^ (dist[v] == -1)) {
        res.push_back(v);
      }
    }
    return res;
  }

  // maximum independent set — size == N - matching size
  vector<int> independent_set() {
    vector<int> res;
    for (int v = 0; v < N; v++) {
      if (!(color[v] ^ (dist[v] == -1))) {
        res.push_back(v);
      }
    }
    return res;
  }

  // minimum edge cover — returns edge ids (0-indexed addEdge() order)
  // only valid when every node has at least one incident edge
  vector<int> edge_cover() {
    vector<bool> done(N);
    vector<int> res;
    for (int id = 0; id < (int)G.edges.size(); id++) {
      int u = G.edges[id][0], v = G.edges[id][1];
      if (done[u] || done[v]) {
        continue;
      }
      if (match[u] == v) {
        res.push_back(id);
        done[u] = done[v] = true;
      }
    }
    for (int id = 0; id < (int)G.edges.size(); id++) {
      int u = G.edges[id][0], v = G.edges[id][1];
      if (!done[u]) {
        res.push_back(id);
        done[u] = true;
      }
      if (!done[v]) {
        res.push_back(id);
        done[v] = true;
      }
    }
    sort(res.begin(), res.end());
    return res;
  }

  /* Dulmage-Mendelsohn decomposition
     https://en.wikipedia.org/wiki/Dulmage%E2%80%93Mendelsohn_decomposition
     - edge uv can be in some max matching iff W[u] == W[v]
     - edge uv is always used: W[u]==W[v] and it's the only such edge
     - edge uv is never used: W[u] != W[v]
     - color=0 node always matched: W in 1..K
     - color=1 node always matched: W in 0..K-1
  */
  pair<int, vector<int>> DM_decomposition() {
    const int INF = INT_MAX / 2;
    vector<int> W(N, -1);
    vector<int> que;
    auto add = [&](int v, int x) {
      if (W[v] == -1) {
        W[v] = x;
        que.push_back(v);
      }
    };
    for (int v = 0; v < N; v++) {
      if (match[v] == -1 && color[v] == 0) {
        add(v, 0);
      }
    }
    for (int v = 0; v < N; v++) {
      if (match[v] == -1 && color[v] == 1) {
        add(v, INF);
      }
    }
    while (!que.empty()) {
      int v = que.back();
      que.pop_back();
      if (match[v] != -1) {
        add(match[v], W[v]);
      }
      if (color[v] == 0 && W[v] == 0) {
        for (auto &e : G.adj[v]) {
          add(e[0], W[v]);
        }
      }
      if (color[v] == 1 && W[v] == INF) {
        for (auto &e : G.adj[v]) {
          add(e[0], W[v]);
        }
      }
    }
    // build directed graph on unassigned nodes, run SCC
    vector<int> V;
    for (int v = 0; v < N; v++) {
      if (W[v] == -1) {
        V.push_back(v);
      }
    }
    int n = (int)V.size();
    vector<vector<int>> dg(n);
    for (int i = 0; i < n; i++) {
      int v = V[i];
      if (match[v] != -1) {
        int j = (int)(lower_bound(V.begin(), V.end(), match[v]) - V.begin());
        dg[i].push_back(j);
      }
      if (color[v] == 0) {
        for (auto &e : G.adj[v]) {
          int to = e[0];
          if (W[to] != -1 || to == match[v]) {
            continue;
          }
          int j = (int)(lower_bound(V.begin(), V.end(), to) - V.begin());
          dg[i].push_back(j);
        }
      }
    }
    auto [K, comp] = scc(n, dg);
    K += 1;
    for (int i = 0; i < n; i++) {
      W[V[i]] = 1 + comp[i];
    }
    for (int v = 0; v < N; v++) {
      if (W[v] == INF) {
        W[v] = K;
      }
    }
    return {K, W};
  }
};

int main() {
  int N = 6;
  cin >> N;
  Graph G(N, true);

  // Add edges (0..2 left side, 3..5 right side)
  G.addEdge(0, 3);
  G.addEdge(0, 4);
  G.addEdge(1, 3);
  G.addEdge(1, 5);
  G.addEdge(2, 4);
  G.addEdge(2, 5);

  // Explicit coloring: 0 = left partition, 1 = right partition
  vector<int> color = {0, 0, 0, 1, 1, 1};

  // Construct bipartite matching using explicit coloring
  BipartiteMatching bm(G, color);

  // Print the coloring
  cout << "Explicit coloring passed to BipartiteMatching:\n";
  for (int v = 0; v < N; v++) {
    cout << "Node " << v << ": " << bm.color[v] << "\n";
  }

  // Maximum matching
  auto matching = bm.matching();
  cout << "\nMaximum matching edges:\n";
  for (auto &[u, v] : matching) {
    cout << u << " - " << v << "\n";
  }

  // Minimum vertex cover
  auto vertex_cover = bm.vertex_cover();
  cout << "\nMinimum vertex cover nodes:\n";
  for (int v : vertex_cover)
    cout << v << " ";
  cout << "\n";

  // Maximum independent set
  auto independent_set = bm.independent_set();
  cout << "\nMaximum independent set nodes:\n";
  for (int v : independent_set)
    cout << v << " ";
  cout << "\n";

  // Minimum edge cover
  auto edge_cover = bm.edge_cover();
  cout << "\nMinimum edge cover edges (by addEdge() order):\n";
  for (int id : edge_cover)
    cout << id << " ";
  cout << "\n";

  // Dulmage-Mendelsohn decomposition
  auto [K, W] = bm.DM_decomposition();
  cout << "\nDulmage-Mendelsohn decomposition:\n";
  cout << "Number of components: " << K << "\n";
  for (int v = 0; v < N; v++)
    cout << "Node " << v << ": " << W[v] << "\n";

  return 0;
}
