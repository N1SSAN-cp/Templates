#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(x) x.begin(), x.end()

template <typename F> struct reverse_comparator_t {
  F f;
  template <typename Arg1, typename Arg2>
  constexpr bool operator()(Arg1 &&arg1, Arg2 &&arg2) & {
    return f(forward<Arg2>(arg2), forward<Arg1>(arg1));
  }
  template <typename Arg1, typename Arg2>
  constexpr bool operator()(Arg1 &&arg1, Arg2 &&arg2) const & {
    return f(forward<Arg2>(arg2), forward<Arg1>(arg1));
  }
  template <typename Arg1, typename Arg2>
  constexpr bool operator()(Arg1 &&arg1, Arg2 &&arg2) && {
    return std::move(f)(forward<Arg2>(arg2), forward<Arg1>(arg1));
  }
  template <typename Arg1, typename Arg2>
  constexpr bool operator()(Arg1 &&arg1, Arg2 &&arg2) const && {
    return std::move(f)(forward<Arg2>(arg2), forward<Arg1>(arg1));
  }
};

template <typename F>
constexpr reverse_comparator_t<decay_t<F>> reverse_comparator(F &&f) {
  return {forward<F>(f)};
}

struct CartesianTree {
  struct Node {
    int l, m, r; // inclusive ranges
    array<int, 2> c;
  };
  vector<Node> nodes;
  int root = -1;

  CartesianTree() {}

  CartesianTree(vector<Node> &&nodes_, int root_)
      : nodes(std::move(nodes_)), root(root_) {}

  Node &operator[](int idx) { return nodes[idx]; }
  const Node &operator[](int idx) const { return nodes[idx]; }

  int size() const { return int(nodes.size()); }

  // min-cartesian-tree, with earlier cells tiebroken earlier
  template <typename T, typename Comp = less<T>>
  static CartesianTree build_min_tree(const vector<T> &v, Comp comp = Comp()) {
    vector<Node> nodes(v.size() * 2 + 1);
    vector<int> stk;
    stk.reserve(v.size());
    int root = -1;
    for (int i = 0; i <= int(v.size()); i++) {
      int cur = 2 * i;
      nodes[cur].l = i;
      nodes[cur].r = i - 1;
      nodes[cur].m = i - 1;
      nodes[cur].c = {-1, -1};
      while (!stk.empty() &&
             (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
        int nxt = stk.back();
        stk.pop_back();
        nodes[nxt].c[1] = cur;
        nodes[nxt].r = nodes[cur].r;
        cur = nxt;
      }
      if (i == int(v.size())) {
        root = cur;
        break;
      } else {
        nodes[2 * i + 1].l = nodes[cur].l;
        nodes[2 * i + 1].m = i;
        nodes[2 * i + 1].c[0] = cur;
        stk.push_back(2 * i + 1);
      }
    }
    return {std::move(nodes), root};
  }

  // max-cartesian-tree, with earlier cells tiebroken earlier
  template <typename T, typename Comp = less<T>>
  static CartesianTree build_max_tree(const vector<T> &v, Comp comp = Comp()) {
    return build_min_tree(v, reverse_comparator(comp));
  }

  vector<array<int, 3>> ranges() const {
    vector<array<int, 3>> res;
    for (int i = 0; i < size(); i++) {
      if (nodes[i].m >= nodes[i].l && nodes[i].m <= nodes[i].r) {
        res.push_back({nodes[i].l, nodes[i].m, nodes[i].r});
      }
    }
    return res;
  }
};

void solve() {
  int n;
  cin >> n;
  vector<int> arr(n);
  for (auto &x : arr) {
    cin >> x;
  }
}

signed main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int tc = 1;
  cin >> tc;
  for (int i = 0; i < tc; i++) {
    solve();
  }
  return 0;
}
