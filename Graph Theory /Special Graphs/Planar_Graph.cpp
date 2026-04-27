#include <bits/stdc++.h>
using namespace std;

/*
Planar Grpah facts:
    1) maximum chormatic Number  <= 4
    2) V - E + F = 2 [connected]
    3) Contains atleast a node DEG(u) <= 5
*/

template <typename XY> struct Point {
  XY x, y;
  Point(XY x = 0, XY y = 0) : x(x), y(y) {}
  Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
  bool operator<(const Point &p) const { return x != p.x ? x < p.x : y < p.y; }
};

template <typename XY> XY cross(Point<XY> a, Point<XY> b) {
  return a.x * b.y - a.y * b.x;
}

// Shoelace formula: returns 2 * signed area (positive = CCW), O(N)
template <typename XY> XY polygon_area2(vector<Point<XY>> &pts) {
  XY res = 0;
  int n = (int)pts.size();
  for (int i = 0; i < n; i++) {
    res += cross(pts[i], pts[(i + 1) % n]);
  }
  return res;
}

template <typename XY> int half(Point<XY> p) {
  return (p.y != 0) ? (p.y > 0 ? 1 : 0) : (p.x > 0 ? 1 : 0);
}

// Angle sort: returns indices sorted by angle in [0, 2pi)
template <typename XY> vector<int> angle_sort(vector<Point<XY>> &dir) {
  int n = (int)dir.size();
  vector<int> idx(n);
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(), [&](int i, int j) {
    int hi = half(dir[i]), hj = half(dir[j]);
    if (hi != hj) {
      return hi < hj;
    }
    return cross(dir[i], dir[j]) > 0;
  });
  return idx;
}

struct Edge {
  int frm, to, id;
};

/*
 * Planar Graph (Face Enumeration)
 * - No guarantee on behavior when not a connected planar graph
 * - Does not handle N = 1
 * - Face 0 is the outer face
 * - Degree-1 vertices seem to be OK
 */
template <typename XY> struct Planar_Graph {
  using P = Point<XY>;
  int NV, NE, NF;

  vector<vector<int>> adj;
  vector<Edge> edges;

  vector<P> point;
  vector<int> left_face;  // face to the left of directed edge
  vector<int> nxt_edge;   // next edge when traversing face CCW
  vector<int> first_edge; // one edge per face

  Planar_Graph(int N, vector<P> point) : NV(N), adj(N), point(point) {
    assert(N > 1);
  }

  void add(int a, int b) {
    int id = (int)edges.size();
    edges.push_back({a, b, id});
    edges.push_back({b, a, id + 1});
    adj[a].push_back(id);
    adj[b].push_back(id + 1);
  }

  void build() {
    int M = (int)edges.size();
    NE = M / 2;
    nxt_edge.assign(M, -1);
    left_face.assign(M, -1);

    int v0 = 0, e0 = 0;
    for (int v = 0; v < NV; v++) {
      if (point[v] < point[v0]) {
        v0 = v;
      }
      vector<int> eid; vector<P> dir;
      for (int eid_val : adj[v]) {
        eid.push_back(eid_val);
        dir.push_back(point[edges[eid_val].to] - point[v]);
      }
      vector<int> I = angle_sort(dir);
      assert(!I.empty());
      for (int k = 0; k < (int)I.size(); k++) {
        int i = (k == 0 ? I.back() : I[k - 1]);
        int j = I[k];
        i = eid[i];
        j = eid[j];
        nxt_edge[j ^ 1] = i;
      }
      if (v == v0) {
        e0 = eid[I[0]] ^ 1;
      }
    }
    for (auto &x : nxt_edge) {
      assert(x != -1);
    }

    // Build faces by following nxt_edge cycles
    auto make_face = [&](int e) -> void {
      int p = (int)first_edge.size();
      first_edge.push_back(e);
      while (left_face[e] == -1) {
        left_face[e] = p;
        e = nxt_edge[e];
      }
    };

    make_face(e0);
    for (int e = 0; e < 2 * NE; e++) {
      if (left_face[e] == -1) {
        make_face(e);
      }
    }
    NF = (int)first_edge.size();
    assert(NV - NE + NF == 2); // Euler's formula
  }

  // Returns {vs, es}: vs = [v0, v1, ..., v0], es = [e0, e1, ...]
  pair<vector<int>, vector<int>> get_face_data(int fid) {
    vector<int> eid = {first_edge[fid]};
    while (true) {
      int e = nxt_edge[eid.back()];
      if (e == first_edge[fid]) {
        break;
      }
      eid.push_back(e);
    }
    vector<int> vid;
    for (int e : eid) {
      vid.push_back(edges[e].frm);
    }
    vid.push_back(vid[0]);
    return {vid, eid};
  }

  // Returns 2 * signed area of face (positive = CCW inner face, negative = CW
  // outer face)
  XY face_area2(int fid) {
    auto [vid, eid] = get_face_data(fid);
    vector<P> pts;
    for (int v : vid) {
      pts.push_back(point[v]);
    }
    pts.pop_back(); // remove duplicate closing vertex
    return polygon_area2(pts);
  }

  // Dual graph: one node per face, one edge per original edge
  // dual_adj[f] = list of {neighbor_face, edge_id}
  // Edge 2k and 2k+1 correspond to dual edge between left_face[2k] and
  // left_face[2k+1]
  vector<vector<pair<int, int>>> dual_graph() {
    vector<vector<pair<int, int>>> dual(NF);
    for (int e = 0; e < 2 * NE; e += 2) {
      int f0 = left_face[e];
      int f1 = left_face[e ^ 1];
      dual[f0].push_back({f1, e});
      dual[f1].push_back({f0, e ^ 1});
    }
    return dual;
  }
};

int main() {
  // Square with a diagonal: 4 vertices, 5 edges
  // (0,0) - (1,0) - (1,1) - (0,1), plus diagonal (0,0)-(1,1)
  vector<Point<long long>> pts = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

  Planar_Graph<long long> G(4, pts);
  G.add(0, 1); G.add(1, 2); G.add(2, 3);
  G.add(3, 0);
  G.add(0, 2); // diagonal

  G.build();

  cout << "NV=" << G.NV << " NE=" << G.NE << " NF=" << G.NF << "\n";

  for (int f = 0; f < G.NF; f++) {
    auto [vid, eid] = G.get_face_data(f);
    cout << "face " << f << ": vertices = ";
    for (int v : vid) {
      cout << v << " ";
    }
    cout << "  area2 = " << G.face_area2(f) << "\n";
  }

  auto dual = G.dual_graph();
  cout << "\ndual graph:\n";
  for (int f = 0; f < G.NF; f++) {
    cout << "face " << f << " neighbors: ";
    for (auto [nf, eid] : dual[f]) {
      cout << nf << "(via edge " << eid << ") ";
    }
    cout << "\n";
  }

  return 0;
}
