#include <bits/stdc++.h>
using namespace std;



/*
Maximum Independent Set
A set of nodes where no two nodes are connected to each other, and the set is
as large as possible.
Clique: A complete Subgraph
Maximum Clique: The larget Clique
*/


/**
  Time Complexity: O(2^n)
  Given a graph as a symmetric bitset matrix (without any self edges)
  g[i][j] = g[j][i] = 1
  g[i][i] = 0 [always when flippe then also]
  Finds the maximum clique
  Can be used to find the maximum independent set by finding a clique of the
  complement graph.
  Runs in about 1s for n=155, and faster for sparse graphs 0 indexed

  */

/**
 * Problem: Kattis - Senso
 * Given n points and distance d, find the largest group of points
 * where every pair is within Euclidean distance d of each other.
 * Output: size of group + 1-indexed point indices
 */

const int N = 105;
struct Maxclique {
  double limit = 0.025, pk = 0;
  struct Vertex {
    int i, d = 0;
  };
  vector<bitset<N>> e; vector<Vertex> V;
  vector<vector<int>> C;
  vector<int> qmax, q, S, old;
  void init(vector<Vertex> &r) {
    for (auto &v : r) {
      v.d = 0;
    }
    for (auto &v : r) {
      for (auto j : r) {
        v.d += e[v.i][j.i];
      }
    }
    sort(r.begin(), r.end(), [](auto a, auto b) { return a.d > b.d; });
    int mxD = r[0].d;
    for (int i = 0; i < (int)r.size(); i++) {
      r[i].d = min(i, mxD) + 1;
    }
  }
  void expand(vector<Vertex> &R, int lev = 1) {
    S[lev] += S[lev - 1] - old[lev];
    old[lev] = S[lev - 1];
    while ((int)R.size()) {
      if ((int)q.size() + R.back().d <= (int)qmax.size()) {
        return;
      }
      q.push_back(R.back().i);
      vector<Vertex> T;
      for (auto v : R) {
        if (e[R.back().i][v.i]) {
          T.push_back({v.i});
        }
      }
      if ((int)T.size()) {
        if (S[lev]++ / ++pk < limit) {
          init(T);
        }
        int j = 0, mxk = 1, mnk = max((int)qmax.size() - (int)q.size() + 1, 1);
        C[1].clear(), C[2].clear();
        for (auto v : T) {
          int k = 1;
          auto f = [&](int i) { return e[v.i][i]; };
          while (any_of(C[k].begin(), C[k].end(), f)) {
            k++;
          }
          if (k > mxk) {
            mxk = k, C[mxk + 1].clear();
          }
          if (k < mnk) {
            T[j++].i = v.i;
          }
          C[k].push_back(v.i);
        }
        if (j > 0) {
          T[j - 1].d = 0;
        }
        for (int k = mnk; k <= mxk; k++) {
          for (int i : C[k]) {
            T[j].i = i, T[j++].d = k;
          }
        }
        expand(T, lev + 1);
      } else if ((int)q.size() > (int)qmax.size()) {
        qmax = q;
      }
      q.pop_back(), R.pop_back();
    }
  }
  Maxclique(vector<bitset<N>> g)
      : e(g), C((int)e.size() + 1), S((int)C.size()), old(S) {
    for (int i = 0; i < (int)e.size(); i++) {
      V.push_back({i});
    }
  }
  vector<int> solve() {
    init(V), expand(V);
    return qmax;
  }
};
array<int, 2> p[N];
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, d; cin >> n >> d;
  for (int i = 0; i < n; i++) {
    cin >> p[i][0] >> p[i][1];
  }
  vector<bitset<N>> g(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      int dis = (p[i][0] - p[j][0]) * (p[i][0] - p[j][0]) +
                (p[i][1] - p[j][1]) * (p[i][1] - p[j][1]);
      if (dis <= d * d) {
        g[i][j] = 1;
      }
    }
  }
  Maxclique M(g);
  vector<int> clique = M.solve();
  cout << clique.size() << '\n';
  for (auto i : clique) {
    cout << i + 1 << ' ';
  }
  cout << '\n';
  return 0;
}
// https://vjudge.net/problem/Kattis-senso
