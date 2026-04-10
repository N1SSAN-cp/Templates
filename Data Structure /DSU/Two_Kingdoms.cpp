#include <bits/stdc++.h>
using namespace std;

// Two Kingdoms — Grid coloring, find max component size when merging one pair
// of colors Each cell has a color; adjacent same-color cells are connected. For
// each pair (c1,c2), temporarily merge their edges — find max component. O(E
// log E) with Rollback DSU (no path compression)

struct RollbackDSU {
  vector<int> parent, sz;
  vector<array<int, 2>> history;
  int components, maxSize;

  RollbackDSU(int n) {
    parent.resize(n + 1);
    sz.assign(n + 1, 1);
    components = n;
    maxSize = 1;
    for (int i = 1; i <= n; i++) {
      parent[i] = i;
    }
  }

  int find(int x) const {
    while (parent[x] != x) {
      x = parent[x];
    }
    return x;
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    if (sz[a] < sz[b]) {
      swap(a, b);
    }
    history.push_back({b, sz[a]});
    parent[b] = a;
    sz[a] += sz[b];
    components--;
    maxSize = max(maxSize, sz[a]);
    return true;
  }

  bool connected(int a, int b) { return find(a) == find(b); }

  void rollback() {
    if (history.empty()) {
      return;
    }
    auto last = history.back();
    history.pop_back();
    int b = last[0], oldSzA = last[1], a = parent[b];
    sz[a] = oldSzA;
    parent[b] = b;
    components--;
    maxSize = 1;
  }

  int getMax() const { return maxSize; }
};

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<int>> grid(n, vector<int>(m));
  vector<vector<int>> node(n, vector<int>(m));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> grid[i][j];
    }
  }

  RollbackDSU dsu(n * m);
  int dx[2] = {1, 0}, dy[2] = {0, 1};
  int id = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      node[i][j] = id++;
    }
  }

  auto valid = [&](int x, int y) { return x >= 0 && x < n && y >= 0 && y < m; };

  vector<array<int, 4>> edges; // {c1, c2, u, v}
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      for (int k = 0; k < 2; k++) {
        int nx = i + dx[k], ny = j + dy[k];
        if (!valid(nx, ny)) {
          continue;
        }
        int c1 = grid[i][j], c2 = grid[nx][ny];
        if (c1 == c2) {
          dsu.unite(node[i][j], node[nx][ny]);
        } else {
          if (c1 > c2) {
            swap(c1, c2);
          }
          edges.push_back({c1, c2, node[i][j], node[nx][ny]});
        }
      }
    }
  }

  dsu.history.clear();
  sort(edges.begin(), edges.end());

  int ans = dsu.getMax();
  int totalEdges = edges.size();

  for (int i = 0; i < totalEdges;) {
    int j = i;
    while (j < totalEdges && edges[i][0] == edges[j][0] &&
           edges[i][1] == edges[j][1]) {
      dsu.unite(edges[j][2], edges[j][3]);
      j++;
    }
    ans = max(ans, dsu.getMax());
    while (!dsu.history.empty()) {
      dsu.rollback();
    }
    i = j;
  }

  cout << ans << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
