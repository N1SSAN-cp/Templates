#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

template <typename X> struct Dice {
  using ARR = array<X, 6>;
  ARR A;

  Dice(ARR a) : A(a) {}

  X U() { return A[0]; }
  X F() { return A[1]; }
  X R() { return A[2]; }
  X L() { return A[3]; }
  X B() { return A[4]; }
  X D() { return A[5]; }

  // Specify FRLB where U moves to, or compass direction NWES
  void rotate(char c) {
    if (c == 'F' || c == 'S') {
      A = {A[4], A[0], A[2], A[3], A[5], A[1]};
    } else if (c == 'R' || c == 'E') {
      A = {A[3], A[1], A[0], A[5], A[4], A[2]};
    } else if (c == 'L' || c == 'W') {
      A = {A[2], A[1], A[5], A[0], A[4], A[3]};
    } else if (c == 'B' || c == 'N') {
      A = {A[1], A[5], A[2], A[3], A[0], A[4]};
    } else if (c == 'U') {
      A = {A[0], A[2], A[4], A[1], A[3], A[5]};
    } else if (c == 'D') {
      A = {A[0], A[3], A[1], A[4], A[2], A[5]};
    } else {
      assert(false);
    }
  }

  // Returns all 24 valid orientations
  vector<ARR> gen_all() {
    vector<ARR> res(24);
    vector<tuple<int, int, int>> tmp(24);
    tmp[0] = {0, 1, 2};
    tmp[1] = {0, 4, 3};
    tmp[2] = {5, 1, 3};
    tmp[3] = {5, 4, 2};
    for (int i = 0; i < 4; i++) {
      auto [a, b, c] = tmp[i];
      tmp[4 + i] = {b, c, a};
      tmp[8 + i] = {c, a, b};
    }
    for (int i = 0; i < 12; i++) {
      auto [a, b, c] = tmp[i];
      tmp[12 + i] = {5 - b, a, c};
    }
    for (int i = 0; i < 24; i++) {
      auto [a, b, c] = tmp[i];
      res[i] = {A[a], A[b], A[c], A[5 - c], A[5 - b], A[5 - a]};
    }
    return res;
  }

  // Adjacency list: 24 nodes, edge = reachable in 1 rotation
  vector<vector<int>> to_graph() {
    int N = 24;
    vector<vector<int>> G(N);
    auto dat = gen_all();
    for (int i = 0; i < N; i++) {
      array<X, 6> cur = dat[i];
      for (char ch : {'U', 'F', 'R', 'L', 'B', 'D'}) {
        Dice<X> d(cur);
        d.rotate(ch);
        int idx = -1;
        for (int j = 0; j < N; j++) {
          if (dat[j] == d.A) {
            idx = j;
          }
        }
        assert(idx != -1);
        if (i < idx) {
          G[i].push_back(idx);
          G[idx].push_back(i);
        }
      }
    }
    return G;
  }
};

vector<int> graph_bfs(vector<vector<int>> &G, int start_oid) {
  vector<int> dist(24, -1);
  queue<int> q;
  dist[start_oid] = 0;
  q.push(start_oid);
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    for (int nb : G[cur]) {
      if (dist[nb] == -1) {
        dist[nb] = dist[cur] + 1;
        q.push(nb);
      }
    }
  }
  return dist;
}

int main() {
  // EXAMPLE 1: roll dice and print top face after each move
  {
    Dice<int> d({1, 2, 3, 4, 5, 6});
    cout << "start top: " << d.U() << "\n";
    d.rotate('F');
    cout << "after roll F, top: " << d.U() << "\n";
    d.rotate('R');
    cout << "after roll R, top: " << d.U() << "\n";
  }

  // EXAMPLE 2: check if two dice are the same
  {
    Dice<int> d1({1, 2, 3, 4, 5, 6});
    Dice<int> d2({1, 5, 4, 2, 3, 6});
    auto all = d1.gen_all();
    bool same = false;
    for (auto &arr : all) {
      if (arr == d2.A) {
        same = true;
      }
    }
    cout << "dice same: " << (same ? "yes" : "no") << "\n";
  }

  // EXAMPLE 3: min rolls to go from orientation 0 to orientation 5
  {
    Dice<int> d({1, 2, 3, 4, 5, 6});
    auto G = d.to_graph();
    auto dist = graph_bfs(G, 0);
    cout << "min rolls orientation 0 -> 5: " << dist[5] << "\n";
  }

  // EXAMPLE 4: min rolls to match a target dice state
  {
    Dice<int> d({1, 2, 3, 4, 5, 6});
    auto ors = d.gen_all();
    map<array<int, 6>, int> to_id;
    for (int i = 0; i < 24; i++) {
      to_id[ors[i]] = i;
    }

    array<int, 6> target = {2, 1, 3, 4, 6, 5};
    int target_oid = to_id[target];

    auto G = d.to_graph();
    auto dist = graph_bfs(G, 0);
    cout << "min rolls to reach target state: " << dist[target_oid] << "\n";
  }

  // EXAMPLE 5: count all orientations reachable (expect 24)
  {
    Dice<int> d({1, 2, 3, 4, 5, 6});
    auto G = d.to_graph();
    auto dist = graph_bfs(G, 0);
    int count = 0;
    for (int i = 0; i < 24; i++) {
      if (dist[i] != -1) {
        count++;
      }
    }
    cout << "reachable orientations (expect 24): " << count << "\n";
  }

  return 0;
}
