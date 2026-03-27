#include <bits/stdc++.h>
using namespace std;

template <typename T> struct SegTree {
  int n;
  T identity;
  vector<T> Tree;
  function<T(const T &, const T &)> merge;

  SegTree(int n, T identity, function<T(const T &, const T &)> merge)
      : n(n), identity(identity), merge(merge), Tree(4 * n, identity) {}

  SegTree(vector<T> &arr, T identity, function<T(const T &, const T &)> merge)
      : n(arr.size()), identity(identity), merge(merge), Tree(4 * n, identity) {
    build(1, 0, n - 1, arr);
  }

  void update(int pos, T val) { update(1, 0, n - 1, pos, val); }
  T query(int lq, int rq) { return query(1, 0, n - 1, lq, rq); }

private:
  void build(int id, int l, int r, vector<T> &arr) {
    if (l == r) {
      Tree[id] = arr[l];
      return;
    }
    int mid = (l + r) >> 1;
    build(id << 1, l, mid, arr);
    build(id << 1 | 1, mid + 1, r, arr);
    Tree[id] = merge(Tree[id << 1], Tree[id << 1 | 1]);
  }

  void update(int id, int l, int r, int pos, T val) {
    if (pos > r || pos < l) {
      return;
    }
    if (l == r) {
      Tree[id] = val;
      return;
    }
    int mid = (l + r) >> 1;
    update(id << 1, l, mid, pos, val);
    update(id << 1 | 1, mid + 1, r, pos, val);
    Tree[id] = merge(Tree[id << 1], Tree[id << 1 | 1]);
  }

  T query(int id, int l, int r, int lq, int rq) {
    if (lq > r || l > rq) {
      return identity;
    }
    if (lq <= l && r <= rq) {
      return Tree[id];
    }
    int mid = (l + r) >> 1;
    T left = query(id << 1, l, mid, lq, rq);
    T right = query(id << 1 | 1, mid + 1, r, lq, rq);
    return merge(left, right);
  }
};

// ---- node definitions ----

struct NodeMaxSub {
  int sum, pref, suf, ans;
  NodeMaxSub() : sum(0), pref(0), suf(0), ans(0) {}
  NodeMaxSub(int v) : sum(v), pref(max(0, v)), suf(max(0, v)), ans(max(0, v)) {}
};

struct NodeMinSub {
  int sum, pref, suf, ans;
  NodeMinSub() : sum(0), pref(0), suf(0), ans(0) {}
  NodeMinSub(int v) : sum(v), pref(min(0, v)), suf(min(0, v)), ans(min(0, v)) {}
};

struct NodeMaxIdx {
  int val, idx;
  NodeMaxIdx() : val(INT_MIN), idx(-1) {}
  NodeMaxIdx(int v, int i) : val(v), idx(i) {}
};

struct NodeMinIdx {
  int val, idx;
  NodeMinIdx() : val(INT_MAX), idx(-1) {}
  NodeMinIdx(int v, int i) : val(v), idx(i) {}
};

struct NodeSorted {
  int mn, mx;
  bool ok;
  NodeSorted() : mn(INT_MAX), mx(INT_MIN), ok(true) {}
  NodeSorted(int v) : mn(v), mx(v), ok(true) {}
};

struct NodePref {
  int sum, pref;
  NodePref() : sum(0), pref(0) {}
  NodePref(int v) : sum(v), pref(max(0, v)) {}
};

struct NodeSuf {
  int sum, suf;
  NodeSuf() : sum(0), suf(0) {}
  NodeSuf(int v) : sum(v), suf(max(0, v)) {}
};

int main() {
  int n;
  cin >> n;
  vector<int> arr(n);
  for (auto &x : arr) {
    cin >> x;
  }

  // primitives
  SegTree<int> sumST(arr, 0, [](int a, int b) { return a + b; });
  SegTree<int> minST(arr, INT_MAX, [](int a, int b) { return min(a, b); });
  SegTree<int> maxST(arr, INT_MIN, [](int a, int b) { return max(a, b); });
  SegTree<int> gcdST(arr, 0, [](int a, int b) { return __gcd(a, b); });
  SegTree<int> xorST(arr, 0, [](int a, int b) { return a ^ b; });
  SegTree<int> orST(arr, 0, [](int a, int b) { return a | b; });
  SegTree<int> andST(arr, ~0, [](int a, int b) { return a & b; });
  SegTree<int> prodST(arr, 1, [](int a, int b) { return a * b; });

  // max subarray sum
  vector<NodeMaxSub> arrMaxSub(n);
  for (int i = 0; i < n; i++) {
    arrMaxSub[i] = NodeMaxSub(arr[i]);
  }
  SegTree<NodeMaxSub> maxSubST(arrMaxSub, NodeMaxSub(),
                               [](NodeMaxSub a, NodeMaxSub b) {
                                 NodeMaxSub r;
                                 r.sum = a.sum + b.sum;
                                 r.pref = max(a.pref, a.sum + b.pref);
                                 r.suf = max(b.suf, b.sum + a.suf);
                                 r.ans = max({a.ans, b.ans, a.suf + b.pref});
                                 return r;
                               });

  // min subarray sum
  vector<NodeMinSub> arrMinSub(n);
  for (int i = 0; i < n; i++) {
    arrMinSub[i] = NodeMinSub(arr[i]);
  }
  SegTree<NodeMinSub> minSubST(arrMinSub, NodeMinSub(),
                               [](NodeMinSub a, NodeMinSub b) {
                                 NodeMinSub r;
                                 r.sum = a.sum + b.sum;
                                 r.pref = min(a.pref, a.sum + b.pref);
                                 r.suf = min(b.suf, b.sum + a.suf);
                                 r.ans = min({a.ans, b.ans, a.suf + b.pref});
                                 return r;
                               });

  // max with index
  vector<NodeMaxIdx> arrMaxIdx(n);
  for (int i = 0; i < n; i++) {
    arrMaxIdx[i] = NodeMaxIdx(arr[i], i);
  }
  SegTree<NodeMaxIdx> maxIdxST(
      arrMaxIdx, NodeMaxIdx(),
      [](NodeMaxIdx a, NodeMaxIdx b) { return a.val >= b.val ? a : b; });

  // min with index
  vector<NodeMinIdx> arrMinIdx(n);
  for (int i = 0; i < n; i++) {
    arrMinIdx[i] = NodeMinIdx(arr[i], i);
  }
  SegTree<NodeMinIdx> minIdxST(
      arrMinIdx, NodeMinIdx(),
      [](NodeMinIdx a, NodeMinIdx b) { return a.val <= b.val ? a : b; });

  // sorted ascending
  vector<NodeSorted> arrSortedInc(n);
  for (int i = 0; i < n; i++) {
    arrSortedInc[i] = NodeSorted(arr[i]);
  }
  SegTree<NodeSorted> sortedIncST(arrSortedInc, NodeSorted(),
                                  [](NodeSorted a, NodeSorted b) {
                                    NodeSorted r;
                                    r.mn = min(a.mn, b.mn);
                                    r.mx = max(a.mx, b.mx);
                                    r.ok = a.ok && b.ok && (a.mx <= b.mn);
                                    return r;
                                  });

  // sorted descending
  vector<NodeSorted> arrSortedDec(n);
  for (int i = 0; i < n; i++) {
    arrSortedDec[i] = NodeSorted(arr[i]);
  }
  SegTree<NodeSorted> sortedDecST(arrSortedDec, NodeSorted(),
                                  [](NodeSorted a, NodeSorted b) {
                                    NodeSorted r;
                                    r.mn = min(a.mn, b.mn);
                                    r.mx = max(a.mx, b.mx);
                                    r.ok = a.ok && b.ok && (a.mn >= b.mx);
                                    return r;
                                  });

  // max prefix sum
  vector<NodePref> arrPref(n);
  for (int i = 0; i < n; i++) {
    arrPref[i] = NodePref(arr[i]);
  }
  SegTree<NodePref> prefST(arrPref, NodePref(), [](NodePref a, NodePref b) {
    NodePref r;
    r.sum = a.sum + b.sum;
    r.pref = max(a.pref, a.sum + b.pref);
    return r;
  });

  // max suffix sum
  vector<NodeSuf> arrSuf(n);
  for (int i = 0; i < n; i++) {
    arrSuf[i] = NodeSuf(arr[i]);
  }
  SegTree<NodeSuf> sufST(arrSuf, NodeSuf(), [](NodeSuf a, NodeSuf b) {
    NodeSuf r;
    r.sum = a.sum + b.sum;
    r.suf = max(b.suf, b.sum + a.suf);
    return r;
  });

  // example queries
  cout << sumST.query(0, n - 1) << "\n";          // range sum
  cout << minST.query(0, n - 1) << "\n";          // range min
  cout << maxST.query(0, n - 1) << "\n";          // range max
  cout << gcdST.query(0, n - 1) << "\n";          // range gcd
  cout << xorST.query(0, n - 1) << "\n";          // range xor
  cout << orST.query(0, n - 1) << "\n";           // range or
  cout << andST.query(0, n - 1) << "\n";          // range and
  cout << prodST.query(0, n - 1) << "\n";         // range product
  cout << maxSubST.query(0, n - 1).ans << "\n";   // max subarray sum
  cout << minSubST.query(0, n - 1).ans << "\n";   // min subarray sum
  cout << maxIdxST.query(0, n - 1).idx << "\n";   // index of max
  cout << minIdxST.query(0, n - 1).idx << "\n";   // index of min
  cout << sortedIncST.query(0, n - 1).ok << "\n"; // is sorted asc
  cout << sortedDecST.query(0, n - 1).ok << "\n"; // is sorted desc
  cout << prefST.query(0, n - 1).pref << "\n";    // max prefix sum
  cout << sufST.query(0, n - 1).suf << "\n";      // max suffix sum

  // example point update
  sumST.update(2, 10);

  return 0;
}
