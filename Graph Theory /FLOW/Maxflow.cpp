#include <bits/stdc++.h>
using namespace std;

template <typename Cap> struct MaxFlow {
  struct Edge {
    int to, rev;
    Cap cap;
  };
  vector<tuple<int, int, Cap, Cap>> edge_pool;
  vector<Edge> edges;
  vector<int> indptr, level;
  vector<Cap> init_cap;
  vector<int> pos;

  const int N, source, sink;
  bool calculated;
  Cap flow_ans;

  MaxFlow(int N, int source, int sink)
      : N(N), source(source), sink(sink), calculated(false), flow_ans(0) {}

  void add(int frm, int to, Cap cap, Cap rev_cap = 0) {
    calculated = false;
    assert(0 <= frm && frm < N);
    assert(0 <= to && to < N);
    assert(Cap(0) <= cap);
    edge_pool.emplace_back(frm, to, cap, rev_cap);
  }

  void build_csr() {
    int M = (int)edge_pool.size();
    indptr.assign(N + 1, 0);
    for (auto [a, b, c, d] : edge_pool) {
      indptr[a + 1]++;
      indptr[b + 1]++;
    }
    for (int i = 0; i < N; i++) {
      indptr[i + 1] += indptr[i];
    }
    vector<int> prog = indptr;
    edges.resize(2 * M);
    init_cap.resize(2 * M);
    for (auto [a, b, c, d] : edge_pool) {
      int I1 = prog[a], I2 = prog[b];
      edges[I1] = Edge{b, I2, c};
      init_cap[I1] = c;
      prog[a]++;
      edges[I2] = Edge{a, I1, d};
      init_cap[I2] = d;
      prog[b]++;
      pos.emplace_back(I1);
    }
  }

  Cap flow() {
    if (calculated) {
      return flow_ans;
    }
    calculated = true;
    if (indptr.empty()) {
      build_csr();
    }
    vector<int> prog = indptr;
    level.resize(N);
    vector<int> que(N);

    auto bfs = [&]() {
      fill(level.begin(), level.end(), -1);
      int ql = 0, qr = 0;
      level[source] = 0;
      que[qr++] = source;
      while (ql < qr) {
        int v = que[ql++];
        for (int i = indptr[v]; i < indptr[v + 1]; i++) {
          Edge &e = edges[i];
          if (e.cap > 0 && level[e.to] == -1) {
            level[e.to] = level[v] + 1;
            que[qr++] = e.to;
            if (e.to == sink) {
              return;
            }
          }
        }
      }
    };

    // ACL reference. Method of tracking how much flow reaches v.
    // Faster because only nodes on source-sink path are explored.
    auto dfs = [&](auto &dfs, int v, Cap lim) -> Cap {
      if (v == source) {
        return lim;
      }
      Cap res = 0;
      int nxt_lv = level[v] - 1;
      for (int &i = prog[v]; i < indptr[v + 1]; ++i) {
        auto &e = edges[i];
        if (level[e.to] != nxt_lv || edges[e.rev].cap == 0) {
          continue;
        }
        Cap a = dfs(dfs, e.to, min(lim - res, edges[e.rev].cap));
        if (a <= 0) {
          continue;
        }
        e.cap += a;
        edges[e.rev].cap -= a;
        res += a;
        if (res == lim) {
          return res;
        }
      }
      level[v] = N;
      return res;
    };

    Cap INF = numeric_limits<Cap>::max() / 2;
    while (1) {
      bfs();
      if (level[sink] == -1) {
        break;
      }
      copy(indptr.begin(), indptr.end(), prog.begin());
      Cap x = dfs(dfs, sink, INF);
      flow_ans += x;
      if (flow_ans >= INF) {
        return INF;
      }
    }
    return flow_ans;
  }

  void change_capacity(int i, Cap after) {
    i = pos[i];
    auto &e = edges[i];
    Cap before = init_cap[i];
    Cap f = before - e.cap;
    init_cap[i] = after;
    if (before < after) {
      calculated = (e.cap > 0);
      e.cap += after - before;
      return;
    }
    e.cap = after - f;
    // push back the difference
    if (e.cap < 0) {
      flow_push_back(e);
    }
  }

  void flow_push_back(Edge &e0) {
    auto &re0 = edges[e0.rev];
    int a = re0.to;
    int b = e0.to;
    /*
    Make capacity of edge e0 non-negative.
    Considering path-cycle decomposition:
    - Remove cycles containing edge uv
    - Remove s-u-v-t paths
    Former: a-b path in residual graph (flow_ans unchanged)
    Latter: t-b path and a-s path in residual graph
    */
    vector<int> prog(N + 1);
    auto find_path = [&](int s, int t, Cap lim) -> Cap {
      vector<bool> vis(N);
      copy(indptr.begin(), indptr.end(), prog.begin());
      auto dfs = [&](auto &dfs, int v, Cap f) -> Cap {
        if (v == t) {
          return f;
        }
        for (int &i = prog[v]; i < indptr[v + 1]; ++i) {
          auto &e = edges[i];
          if (vis[e.to] || e.cap <= Cap(0)) {
            continue;
          }
          vis[e.to] = true;
          Cap got = dfs(dfs, e.to, min(f, e.cap));
          assert(got >= 0);
          if (got == Cap(0)) {
            continue;
          }
          e.cap -= got;
          edges[e.rev].cap += got;
          return got;
        }
        return Cap(0);
      };
      return dfs(dfs, s, lim);
    };

    while (e0.cap < 0) {
      Cap x = find_path(a, b, -e0.cap);
      if (x == Cap(0)) {
        break;
      }
      e0.cap += x;
      re0.cap -= x;
    }
    Cap c = -e0.cap;
    while (c > 0 && a != source) {
      Cap x = find_path(a, source, c);
      assert(x > 0);
      c -= x;
    }
    c = -e0.cap;
    while (c > 0 && b != sink) {
      Cap x = find_path(sink, b, c);
      assert(x > 0);
      c -= x;
    }
    c = -e0.cap;
    e0.cap += c;
    re0.cap -= c;
    flow_ans -= c;
  }

  // returns (frm, to, flow) for each edge with positive flow
  vector<tuple<int, int, Cap>> get_flow_edges() {
    vector<tuple<int, int, Cap>> res;
    for (int v = 0; v < N; v++) {
      for (int i = indptr[v]; i < indptr[v + 1]; i++) {
        Cap f = init_cap[i] - edges[i].cap;
        if (f > 0) {
          res.emplace_back(v, edges[i].to, f);
        }
      }
    }
    return res;
  }

  // adjacency list of edges with remaining capacity
  vector<vector<int>> res_graph() {
    vector<vector<int>> G(N);
    for (int v = 0; v < N; v++) {
      for (int i = indptr[v]; i < indptr[v + 1]; i++) {
        if (edges[i].cap > 0) {
          G[v].push_back(edges[i].to);
        }
      }
    }
    return G;
  }

  // returns {cut_value, side[v]} where side[v]=0 -> source side, 1 -> sink side
  pair<Cap, vector<int>> cut() {
    flow();
    vector<int> res(N);
    for (int v = 0; v < N; v++) {
      res[v] = (level[v] >= 0 ? 0 : 1);
    }
    return {flow_ans, res};
  }

  // O(F*(N+M)) — only use when flow value is small
  vector<vector<int>> path_decomposition() {
    flow();
    auto fe = get_flow_edges();
    vector<vector<int>> TO(N);
    for (auto &[frm, to, fl] : fe) {
      for (Cap i = 0; i < fl; i++) {
        TO[frm].push_back(to);
      }
    }
    vector<vector<int>> res;
    vector<int> vis(N);
    for (Cap iter = 0; iter < flow_ans; iter++) {
      vector<int> path = {source};
      vis[source] = 1;
      while (path.back() != sink) {
        int to = TO[path.back()].back();
        TO[path.back()].pop_back();
        while (vis[to]) {
          vis[path.back()] = 0;
          path.pop_back();
        }
        path.push_back(to);
        vis[to] = 1;
      }
      for (int v : path) {
        vis[v] = 0;
      }
      res.push_back(path);
    }
    return res;
  }
};

int main() {
  int N = 6;
  int source = 0, sink = 5;

  MaxFlow<int> mf(N, source, sink);

  // Add edges
  mf.add(0, 1, 10);
  mf.add(0, 2, 5);
  mf.add(1, 2, 15);
  mf.add(1, 3, 10);
  mf.add(2, 4, 10);
  mf.add(3, 4, 10);
  mf.add(3, 5, 10);
  mf.add(4, 5, 10);

  // Compute max flow
  int maxflow = mf.flow();
  cout << "Max Flow = " << maxflow << "\n";

  // Flow edges
  auto flow_edges = mf.get_flow_edges();
  cout << "\nFlow edges:\n";
  for (auto &[u, v, f] : flow_edges) {
    cout << u << " -> " << v << " : " << f << "\n";
  }

  // Residual graph
  auto resG = mf.res_graph();
  cout << "\nResidual graph (edges with remaining capacity):\n";
  for (int i = 0; i < N; i++) {
    cout << i << ": ";
    for (int j : resG[i])
      cout << j << " ";
    cout << "\n";
  }

  // Min cut
  auto [cut_value, side] = mf.cut();
  cout << "\nMin Cut = " << cut_value << "\n";
  cout << "Side array (0=source side, 1=sink side): ";
  for (int i = 0; i < N; i++)
    cout << side[i] << " ";
  cout << "\n";

  // Print actual cut edges: edges going from source side to sink side with flow
  // > 0
  cout << "\nCut edges (source side -> sink side):\n";
  for (auto &[u, v, f] : flow_edges) {
    if (side[u] == 0 && side[v] == 1) {
      cout << u << " -> " << v << " : " << f << "\n";
    }
  }

  // Path decomposition
  auto paths = mf.path_decomposition();
  cout << "\nFlow path decomposition:\n";
  int pnum = 1;
  for (auto &path : paths) {
    cout << "Path " << pnum++ << ": ";
    for (int v : path)
      cout << v << " ";
    cout << "\n";
  }

  return 0;
}
