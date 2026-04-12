#include <bits/stdc++.h>
using namespace std;

// Berlekamp-Massey + Kitamasa Linear Recurrence | O(n^2 log k)
const int MOD = 998244353;

template <int MOD, int RT> struct mint {
  static const int mod = MOD;
  static constexpr mint rt() { return RT; }
  int v;
  explicit operator int() const { return v; }
  mint() : v(0) {}
  mint(long long val) : v(int(val % MOD)) { v += (v < 0) * MOD; }
  mint &operator+=(mint o) {
    if ((v += o.v) >= MOD) {
      v -= MOD;
    }
    return *this;
  }
  mint &operator-=(mint o) {
    if ((v -= o.v) < 0) {
      v += MOD;
    }
    return *this;
  }
  mint &operator*=(mint o) {
    v = int((long long)v * o.v % MOD);
    return *this;
  }
  friend mint pow(mint a, long long p) {
    assert(p >= 0);
    return p == 0 ? 1 : pow(a * a, p / 2) * (p & 1 ? a : 1);
  }
  friend mint inv(mint a) {
    assert(a.v != 0);
    return pow(a, MOD - 2);
  }
  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a -= b; }
  friend mint operator*(mint a, mint b) { return a *= b; }
};
using mi = mint<MOD, 2>;

struct LinearRec {
  vector<mi> seq, rec;

  // Berlekamp-Massey: find shortest linear recurrence | O(n^2)
  // Need at least 2*L terms where L is the recurrence order (unknown upfront)
  // Safe default: provide >= 2 * (expected order) terms; more is fine
  void build(vector<mi> s) {
    seq = s;
    int n = s.size();
    vector<mi> C(n), B(n), tmp;
    C[0] = B[0] = 1; int L = 0, m = 0;
    mi b = 1;
    for (int i = 0; i < n; i++) {
      ++m;
      mi d = s[i];
      for (int j = 1; j <= L; j++) {
        d += C[j] * s[i - j];
      }
      if ((int)d == 0) {
        continue;
      }
      mi coef = d * inv(b);
      tmp = C;
      for (int j = m; j < n; j++) {
        C[j] -= coef * B[j - m];
      }
      if (2 * L > i) {
        continue;
      }
      L = i + 1 - L; B = tmp;
      b = d;
      m = 0;
    }
    C.resize(L + 1); C.erase(C.begin());
    for (auto &x : C) {
      x = mi(0) - x;
    }
    rec = C;
  }

  vector<mi> combine(int n, vector<mi> &a, vector<mi> &b) {
    vector<mi> res(n * 2 + 1);
    for (int i = 0; i <= n; i++) {
      for (int j = 0; j <= n; j++) {
        res[i + j] += a[i] * b[j];
      }
    }
    for (int i = 2 * n; i > n; --i) {
      for (int j = 0; j < n; j++) {
        res[i - 1 - j] += res[i] * rec[j];
      }
    }
    res.resize(n + 1);
    return res;
  }

  // Kitamasa: compute k-th term | O(n^2 log k)
  mi query(long long k) {
    int n = rec.size();
    if (k < (long long)n) {
      return seq[k];
    }
    vector<mi> pol(n + 1), e(n + 1);
    pol[0] = 1;
    e[1] = 1;
    for (++k; k; k >>= 1) {
      if (k & 1) {
        pol = combine(n, pol, e);
      }
      e = combine(n, e, e);
    }
    mi res = 0;
    for (int i = 0; i < n; i++) {
      res += pol[i + 1] * seq[i];
    }
    return res;
  }
};

int main() {
  // Fibonacci has order 2, so need >= 2*2 = 4 terms; using 10 to be safe
  vector<mi> s(10);
  s[0] = 0;
  s[1] = 1;
  for (int i = 2; i < 10; i++) {
    s[i] = s[i - 1] + s[i - 2];
  }

  LinearRec lr;
  lr.build(s);

  long long n; cin >> n;

  cout << int(lr.query(n)) << "\n";
  return 0;
}
