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

struct SCC {
    int V = 0;
    vector<vector<int>> adj;
    vector<int> tour_index, low_link;
    int tour;
    vector<int> stack;
    vector<bool> in_stack;
    vector<vector<int>> components;
    vector<int> which_component;

    SCC(int v = 0) { init(v); }
    SCC(const vector<vector<int>> &_adj) { init(_adj); }

    void init(int v) { V = v; adj.assign(V, {}); }
    void init(const vector<vector<int>> &_adj) { adj = _adj; V = int(adj.size()); }
    void add_edge(int a, int b) { adj[a].push_back(b); }

    // Tarjan's algorithm.
    void dfs(int node) {
        tour_index[node] = tour++;
        low_link[node] = tour_index[node];
        stack.push_back(node);
        in_stack[node] = true;
        for (int nb : adj[node]) {
            if (tour_index[nb] < 0) {
                // neighbor is part of our subtree.
                dfs(nb);
                low_link[node] = min(low_link[node], low_link[nb]);
            } else if (in_stack[nb]) {
                // neighbor is a candidate for low_link.
                low_link[node] = min(low_link[node], tour_index[nb]);
            }
        }
        if (low_link[node] == tour_index[node]) {
            // node is the highest node in an SCC, which includes everything on the stack up to it.
            components.emplace_back();
            vector<int> &comp = components.back();
            int x;
            do {
                x = stack.back(); stack.pop_back();
                in_stack[x] = false;
                which_component[x] = int(components.size()) - 1;
                comp.push_back(x);
            } while (x != node);
        }
    }

    void build() {
        tour_index.assign(V, -1); low_link.resize(V);
        which_component.assign(V, -1);
        stack.clear(); stack.reserve(V);
        in_stack.assign(V, false);
        tour = 0; components = {};
        // Note that Tarjan's algorithm provides the SCCs in reverse topological order.
        for (int i = 0; i < V; i++) {
            if (tour_index[i] < 0) { dfs(i); }
        }
    }
};

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
    SCC _scc(dg);
    _scc.build();
    int K = (int)_scc.components.size() + 1;
    for (int i = 0; i < n; i++) {
      W[V[i]] = 1 + _scc.which_component[i];
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
