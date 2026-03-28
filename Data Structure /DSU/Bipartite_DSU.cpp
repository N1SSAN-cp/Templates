#include <bits/stdc++.h>
using namespace std;

// Bipartite DSU with Parity Tracking + Union by Size
// find: O(α(n)) | unite: O(α(n))
struct BipartiteUnionFind {
  int num, comp;
  vector<int> par, comp_size, bipartite, edge_parity;

  BipartiteUnionFind(int n)
      : num(n), comp(n), par(n), comp_size(n, 1), bipartite(n, 1),
        edge_parity(n, 0) {
    iota(par.begin(), par.end(), 0);
  }

  int find(int x) {
    if (par[x] == x) {
      return x;
    }
    int root = find(par[x]);
    edge_parity[x] ^= edge_parity[par[x]];
    return par[x] = root;
  }

  int get_size(int x) { return comp_size[find(x)]; }

  // Returns true if x and y are in the same component.
  bool query_component(int x, int y) { return find(x) == find(y); }

  // Returns the parity status between x and y (0 = same, 1 = different).
  // Requires them to be in the same component.
  bool query_parity(int x, int y) {
    assert(query_component(x, y));
    return edge_parity[x] ^ edge_parity[y];
  }

  // Returns {union succeeded, edge consistent with bipartite conditions}.
  array<int, 2> unite(int x, int y, bool different = true) {
    int xr = find(x), yr = find(y);
    bool root_parity = edge_parity[x] ^ edge_parity[y] ^ different;
    x = xr;
    y = yr;
    if (x == y) {
      bool consistent = !root_parity;
      bipartite[x] = bipartite[x] && consistent;
      return {0, consistent};
    }
    comp--;
    if (comp_size[x] < comp_size[y]) {
      swap(x, y);
    }
    comp_size[x] += comp_size[y];
    bipartite[x] = bipartite[x] && bipartite[y];
    edge_parity[y] = root_parity;
    par[y] = x;
    return {1, 1};
  }

  // Add an assertion that x and y are different; i.e., a normal edge.
  array<int, 2> add_different_edge(int x, int y) { return unite(x, y, true); }

  // Add an assertion that x and y are the same.
  array<int, 2> add_same_edge(int x, int y) { return unite(x, y, false); }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  BipartiteUnionFind UF(N);

  for (int q = 0; q < Q; q++) {
    int type, a, b;
    cin >> type >> a >> b;
    assert(1 <= min(a, b) && max(a, b) <= N);
    a--;
    b--;
    if (type == 1) {
      bool same = UF.query_component(a, b);
      if (same) {
        cout << same << ' ' << UF.query_parity(a, b) << '\n';
      } else {
        cout << same << '\n';
      }
    } else if (type == 2) {
      int e;
      cin >> e;
      array<int, 2> res = UF.unite(a, b, e);
      cout << res[0] << ' ' << res[1] << '\n';
    } else {
      assert(false);
    }
  }

  for (int i = 0; i < N; i++) {
    cout << UF.bipartite[UF.find(i)];
  }
  cout << '\n';
}
