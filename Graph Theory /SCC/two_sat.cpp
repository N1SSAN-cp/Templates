// SCC (Tarjan's): O(V + E) = O(n + m)
// n --> number of varibles and m --> number of clauses
// Overall: O(n + m)
#include <bits/stdc++.h>
using namespace std;

// ---- SCC (Tarjan) ----
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

  void init(int v) {
    V = v;
    adj.assign(V, {});
  }
  void init(const vector<vector<int>> &_adj) {
    adj = _adj;
    V = int(adj.size());
  }
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
      // node is the highest node in an SCC, which includes everything on the
      // stack up to it.
      components.emplace_back();
      vector<int> &comp = components.back();
      int x;
      do {
        x = stack.back();
        stack.pop_back();
        in_stack[x] = false;
        which_component[x] = int(components.size()) - 1;
        comp.push_back(x);
      } while (x != node);
    }
  }

  void build() {
    tour_index.assign(V, -1);
    low_link.resize(V);
    which_component.assign(V, -1);
    stack.clear();
    stack.reserve(V);
    in_stack.assign(V, false);
    tour = 0;
    components = {};
    // Note that Tarjan's algorithm provides the SCCs in reverse topological
    // order.
    for (int i = 0; i < V; i++) {
      if (tour_index[i] < 0) {
        dfs(i);
      }
    }
  }
};

// ---- 2-SAT ----
struct two_sat {
  int n = 0;
  vector<vector<int>> adj;
  vector<bool> assignment;
  SCC scc;

  two_sat(int _n = 0) { init(_n); }
  void init(int _n) {
    n = _n;
    adj.assign(2 * n, {});
  }

  int inv(int a) { return a ^ 1; }
  int new_var() {
    adj.emplace_back();
    adj.emplace_back();
    return 2 * n++;
  }

  void implies(int a, int b) {
    adj[a].push_back(b);
    adj[inv(b)].push_back(inv(a));
  }
  void either(int a, int b) {
    adj[inv(a)].push_back(b);
    adj[inv(b)].push_back(a);
  }
  void set_value(int a) { adj[inv(a)].push_back(a); }
  void equal(int a, int b) {
    implies(a, b);
    implies(inv(a), inv(b));
  }
  void unequal(int a, int b) {
    implies(a, inv(b));
    implies(inv(a), b);
  }

  // Warning: this only creates an implication in the negative direction. It is
  // still possible for a = b = true with and_var = false. In particular, it
  // does not work to call set_value on inv(and_var).
  int create_and(int a, int b) {
    if (a < 0 || b < 0) {
      return max(a, b);
    }
    int r = new_var();
    implies(r, a);
    implies(r, b);
    return r;
  }
  // Warning: this only creates an implication in the positive direction. It is
  // still possible for a = b = false with or_var = true. In particular, it does
  // not work to call set_value on or_var.
  int create_or(int a, int b) {
    if (a < 0 || b < 0) {
      return max(a, b);
    }
    int r = new_var();
    implies(a, r);
    implies(b, r);
    return r;
  }
  int create_at_most_one(int a, int b) {
    if (a < 0 || b < 0) {
      return max(a, b);
    }
    either(inv(a), inv(b));
    return create_or(a, b);
  }
  template <typename T> int create_at_most_one(const T &vars) {
    int aux = -1;
    for (int v : vars) {
      aux = create_at_most_one(aux, v);
    }
    return aux;
  }

  bool solve() {
    scc.init(adj);
    scc.build();
    for (int i = 0; i < n; i++) {
      if (scc.which_component[2 * i] == scc.which_component[2 * i + 1]) {
        return false;
      }
    }
    assignment.resize(2 * n);
    vector<bool> done(n, false);
    // Tarjan's algorithm provides the SCCs in reverse topological order.
    for (auto &comp : scc.components) {
      for (int x : comp) {
        assignment[x] = !done[x / 2];
        done[x / 2] = true;
      }
    }
    return true;
  }
};

// helpers
auto pos(int i) { return 2 * i; }
auto neg(int i) { return 2 * i + 1; }

// ============================================================
// EXAMPLE 1: Basic clauses
// Variables: x0, x1, x2
// Clauses:
//   (x0 OR x1)       at least one true
//   (!x0 OR x2)      if x0 then x2
//   (!x1 OR !x2)     x1 and x2 can't both be true
// ============================================================
void example_basic() {
  cout << "=== Example 1: Basic ===\n";
  two_sat ts(3);

  ts.either(pos(0), pos(1)); // x0 OR x1
  ts.either(neg(0), pos(2)); // !x0 OR x2  (equiv: x0 => x2)
  ts.either(neg(1), neg(2)); // !x1 OR !x2 (can't both be true)

  if (ts.solve()) {
    for (int i = 0; i < 3; i++) {
      cout << "x" << i << " = " << ts.assignment[pos(i)] << "\n";
    }
  } else {
    cout << "UNSAT\n";
  }
  cout << "\n";
}

// ============================================================
// EXAMPLE 2: Force values
// x0 must be true, x1 must be false
// ============================================================
void example_force() {
  cout << "=== Example 2: Force values ===\n";
  two_sat ts(2);

  ts.set_value(pos(0)); // x0 = true
  ts.set_value(neg(1)); // x1 = false

  if (ts.solve()) {
    for (int i = 0; i < 2; i++) {
      cout << "x" << i << " = " << ts.assignment[pos(i)] << "\n";
    }
  } else {
    cout << "UNSAT\n";
  }
  cout << "\n";
}

// ============================================================
// EXAMPLE 3: UNSAT — x0 must be both true and false
// ============================================================
void example_unsat() {
  cout << "=== Example 3: UNSAT ===\n";
  two_sat ts(1);

  ts.set_value(pos(0)); // x0 = true
  ts.set_value(neg(0)); // x0 = false  (contradiction)

  if (ts.solve()) {
    cout << "x0 = " << ts.assignment[pos(0)] << "\n";
  } else {
    cout << "UNSAT\n";
  }
  cout << "\n";
}

// ============================================================
// EXAMPLE 4: At most one (tournament-style)
// n people, each picks one of two time slots (0 = morning, 1 = evening)
// Constraint: person 0 and person 1 cannot both pick morning
// Constraint: person 1 and person 2 must pick different slots
// ============================================================
void example_scheduling() {
  cout << "=== Example 4: Scheduling ===\n";
  // variable i: true = morning, false = evening
  int n = 3;
  two_sat ts(n);

  // person 0 and 1 can't both be morning: !x0 OR !x1
  ts.either(neg(0), neg(1));

  // person 1 and 2 must differ: (x1 != x2)
  ts.unequal(pos(1), pos(2));

  if (ts.solve()) {
    for (int i = 0; i < n; i++) {
      cout << "person " << i << ": "
           << (ts.assignment[pos(i)] ? "morning" : "evening") << "\n";
    }
  } else {
    cout << "UNSAT\n";
  }
  cout << "\n";
}

// ============================================================
// EXAMPLE 5: Graph 2-coloring
// Can we color nodes with 2 colors such that no edge has same color?
// Nodes: 0,1,2,3   Edges: 0-1, 1-2, 2-3, 3-0  (a 4-cycle, bipartite → SAT)
// ============================================================
void example_2coloring() {
  cout << "=== Example 5: 2-Coloring (4-cycle, bipartite) ===\n";
  // variable i: true = red, false = blue
  // edge (u,v): u and v must differ → unequal
  int n = 4;
  two_sat ts(n);

  vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
  for (auto [u, v] : edges) {
    ts.unequal(pos(u), pos(v));
  }

  if (ts.solve()) {
    for (int i = 0; i < n; i++) {
      cout << "node " << i << ": " << (ts.assignment[pos(i)] ? "red" : "blue")
           << "\n";
    }
  } else {
    cout << "UNSAT\n";
  }
  cout << "\n";
}

int main() {
  example_basic();
  example_force();
  example_unsat();
  example_scheduling();
  example_2coloring();
}
