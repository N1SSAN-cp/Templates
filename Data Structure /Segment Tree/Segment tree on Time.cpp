// Technique: Offline Deletion via Segment Tree on Time
// If a DS supports addition in O(T(n)), this gives deletion in O(T(n) log n)
// offline.
//
// Knapsack pattern:
//   Item type  → using Item = ?
//   apply()    → your DP transition, rb.save() before every write
//   query()    → what to return as answer
//   dp[]       → define what dp[i] means for your problem
//   Everything else → copy paste as is

#include <bits/stdc++.h>
using namespace std;

// ---- Rollback framework ----
struct Rollback {
  vector<pair<int *, int>> history;
  void save(int &field) { history.push_back({&field, field}); }
  int snapshot() { return history.size(); }
  void rollback(int checkpoint) {
    while ((int)history.size() > checkpoint) {
      *history.back().first = history.back().second;
      history.pop_back();
    }
  }
} rb;

// ---- YOUR DS HERE ----
// example: DP array
int K;
const int MOD = 998244353;
const int MAXK = 5005;
int dp[MAXK];
// dp[i]= number of different subsets whose subset sum = i

void apply(int x) {
  // change this per problem
  // before changing ds save the state
  for (int i = K; i >= x; i--) {
    rb.save(dp[i]);
    dp[i] += dp[i - x];
    dp[i] %= MOD;
  }
}

int query() {
  // change chis per problem
  return dp[K];
}

// Item = the unit object that we are storing in segment tree
using Item = int; // change this per problem
// Segtree is maintaing the objects  of [L,R]  currently active stored on the
// node covering time range  [L,R]
vector<Item> SegTree[400005];

void add(int node, int l, int r, int lq, int rq, int v) {
  if (lq > r || l > rq) {
    return;
  }
  if (lq <= l && r <= rq) {
    SegTree[node].push_back(v); return;
  }
  int mid = (l + r) >> 1;
  add(node << 1, l, mid, lq, rq, v);
  add(node << 1 | 1, mid + 1, r, lq, rq, v);
}

vector<int> answers;
// iterate on segment tree from top to down
void dfs(int node, int l, int r) {
  int checkpoint = rb.snapshot();
  // add current values covering this node
  for (auto item : SegTree[node]) {
    apply(item);
  }
  // leaf node at time l
  if (l == r) {
    answers[l] = query(); // may be need to be changed
  } else {
    int mid = (l + r) >> 1;
    dfs(node << 1, l, mid);
    dfs(node << 1 | 1, mid + 1, r);
  }
  rb.rollback(checkpoint);
}
int main() {
  int q; cin >> q >> K;
  dp[0] = 1;
  answers.resize(q);

  vector<pair<char, int>> queries(q);
  for (int i = 0; i < q; i++) {
    cin >> queries[i].first >> queries[i].second;
  }
  map<Item, stack<int>> active;
  // map[x] is actived at points of x
  vector<tuple<Item, int, int>> intervals;

  for (int i = 0; i < q; i++) {
    auto [s, x] = queries[i];
    if (s == '+') {
      active[x].push(i);
    } else {
      int L = active[x].top();
      active[x].pop();
      intervals.push_back({x, L, i - 1});
    }
  }
  for (auto [x, times] : active) {
    while (!times.empty()) {
      intervals.push_back({x, times.top(), q - 1});
      times.pop();
    }
  }
  // add to SegTree
  for (auto [x, l, r] : intervals) {
    add(1, 0, q - 1, l, r, x);
  }
  dfs(1, 0, q - 1);
  // print answer
  for (int i = 0; i < q; i++) {
    cout << answers[i] << " \n"[i + 1 == q];
  }
}
