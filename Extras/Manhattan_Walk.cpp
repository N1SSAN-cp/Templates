// Find A such that: A[0]=s, A[N]=t, dist(A[i],A[i+1])==D[i]. (integer points)
// Empty if it does not exist. https://atcoder.jp/contests/abc135/tasks/abc135_e
// https://codeforces.com/contest/2068/problem/H Algorithm: Manhattan walk —
// greedy reachable-range propagation — O(N)
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;

vector<pair<ll, ll>> manhattan_path(pair<ll, ll> s, pair<ll, ll> t, vector<ll> D) {
  int N = (int)D.size();
  ll S = 0;
  for (int i = 0; i < N; i++) {
    S += D[i];
  }
  ll a = t.first - s.first, b = t.second - s.second;
  if ((S + a + b) % 2 != 0) {
    return {};
  }
  auto nxt_range = [&](ll l, ll r, ll d) -> pair<ll, ll> {
    if (r <= d) {
      return {d - r, d + r};
    } else if (d <= l) {
      return {l - d, r + d};
    }
    ll lo = 0, hi = r + d;
    if ((hi - lo) & 1) {
      ++lo;
    }
    return {lo, hi};
  };
  vector<ll> L(N + 1), R(N + 1);
  L[0] = R[0] = abs(a) + abs(b);
  for (int i = 0; i < N; i++) {
    auto [lo, hi] = nxt_range(L[i], R[i], D[i]);
    L[i + 1] = lo;
    R[i + 1] = hi;
  }
  if (L[N] != 0) {
    return {};
  }
  vector<ll> A(N + 1);
  for (int i = N - 1; i >= 0; i--) {
    auto [lo, hi] = nxt_range(A[i + 1], A[i + 1], D[i]);
    A[i] = max(L[i], lo);
  }
  auto step = [&](auto &step, ll a, ll b, ll X, ll Y) -> pair<ll, ll> {
    if (a < 0) {
      auto [dx, dy] = step(step, -a, b, X, Y);
      return {-dx, dy};
    }
    if (b < 0) {
      auto [dx, dy] = step(step, a, -b, X, Y);
      return {dx, -dy};
    }
    assert(a >= 0 && b >= 0);
    if (a > b) {
      auto [dx, dy] = step(step, b, a, X, Y);
      return {dy, dx};
    }
    ll k = (a + b + X - Y) / 2;
    if (k <= b) {
      ll dy = -k;
      ll dx = X - abs(dy);
      return {dx, dy};
    }
    assert(k <= a + b);
    ll dx = b - k;
    ll dy = -(X - abs(dx));
    return {dx, dy};
  };
  vector<pair<ll, ll>> points;
  points.emplace_back(0, 0);
  for (int i = 0; i < N; i++) {
    auto [x, y] = points[i];
    ll d = D[i];
    auto [dx, dy] = step(step, x - a, y - b, d, A[i + 1]);
    x += dx;
    y += dy;
    points.emplace_back(x, y);
  }
  for (auto &[x, y] : points) {
    x += s.first;
    y += s.second;
  }
  return points;
}

// Verify: consecutive Manhattan distances match D, endpoints match s and t.
bool verify(const vector<pair<ll, ll>> &pts, pair<ll, ll> s, pair<ll, ll> t,
            const vector<ll> &D) {
  int N = (int)D.size();
  if ((int)pts.size() != N + 1) {
    return false;
  }
  if (pts.front() != s || pts.back() != t) {
    return false;
  }
  for (int i = 0; i < N; i++) {
    ll dist = abs(pts[i + 1].first - pts[i].first) +
              abs(pts[i + 1].second - pts[i].second);
    if (dist != D[i]) {
      return false;
    }
  }
  return true;
}

void print_result(const string &label, pair<ll, ll> s, pair<ll, ll> t,
                  const vector<ll> &D, const vector<pair<ll, ll>> &res) {
  cout << label << "\n";
  if (res.empty()) {
    cout << "  No solution\n";
  } else {
    for (int i = 0; i < (int)res.size(); i++) {
      cout << "  P[" << i << "] = (" << res[i].first << ", " << res[i].second
           << ")\n";
    }
    cout << "  Valid: " << (verify(res, s, t, D) ? "YES" : "NO") << "\n";
  }
  cout << "\n";
}

int main() {
  // s=(0,0) -> t=(3,3), steps {3,4,3}: sum=10, |dx|+|dy|=6, parity ok.
  {
    pair<ll, ll> s = {0, 0}, t = {3, 3};
    vector<ll> D = {3, 4, 3};
    auto res = manhattan_path(s, t, D);
    print_result("s=(0,0) t=(3,3) D={3,4,3}", s, t, D, res);
  }

  // s=(0,0) -> t=(0,0), single step D={4}: must return to origin.
  {
    pair<ll, ll> s = {0, 0}, t = {0, 0};
    vector<ll> D = {4};
    auto res = manhattan_path(s, t, D);
    print_result("s=(0,0) t=(0,0) D={4}", s, t, D, res);
  }

  // s=(0,0) -> t=(10,0), single step D={3}: |dx|+|dy|=10 > 3, impossible.
  {
    pair<ll, ll> s = {0, 0}, t = {10, 0};
    vector<ll> D = {3};
    auto res = manhattan_path(s, t, D);
    print_result("s=(0,0) t=(10,0) D={3} [impossible]", s, t, D, res);
  }

  // s=(1,2) -> t=(4,6), steps {5,2,4}: sum=11, |dx|+|dy|=7, parity ok.
  {
    pair<ll, ll> s = {1, 2}, t = {4, 6};
    vector<ll> D = {5, 2, 4};
    auto res = manhattan_path(s, t, D);
    print_result("s=(1,2) t=(4,6) D={5,2,4}", s, t, D, res);
  }

  // parity failure: s=(0,0) t=(1,0), D={2}: sum=2, |dx|+|dy|=1, parity
  // mismatch.
  {
    pair<ll, ll> s = {0, 0}, t = {1, 0};
    vector<ll> D = {2};
    auto res = manhattan_path(s, t, D);
    print_result("s=(0,0) t=(1,0) D={2} [parity fail]", s, t, D, res);
  }

  return 0;
}
