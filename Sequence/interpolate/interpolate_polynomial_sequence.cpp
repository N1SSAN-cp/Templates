#include <bits/stdc++.h>
using namespace std;

// Finite Difference Test: O(m^2)
int polynomialDegree(vector<long long> a) {
  for (int d = 0; d < (int)a.size() - 1; d++) {
    bool constant = true;
    for (int i = 0; i < (int)a.size() - 1; i++) {
      a[i] = a[i + 1] - a[i];
      if (a[i] != a[0]) {
        constant = false;
      }
    }
    a.pop_back();
    if (constant) {
      return d + 1;
    }
  }
  return -1; // not polynomial within given points
}

const int N = 1e6 + 9, mod = 1e6 + 3;
struct base {
  double x, y;
  base() { x = y = 0; }
  base(double x, double y) : x(x), y(y) {}
};
inline base operator+(base a, base b) { return base(a.x + b.x, a.y + b.y); }
inline base operator-(base a, base b) { return base(a.x - b.x, a.y - b.y); }
inline base operator*(base a, base b) {
  return base(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}
inline base conj(base a) { return base(a.x, -a.y); }

int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};
const double PI = acosl(-1.0);

void ensure_base(int p) {
  if (p <= lim) {
    return;
  }
  rev.resize(1 << p);
  for (int i = 0; i < (1 << p); i++) {
    rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (p - 1));
  }
  roots.resize(1 << p);
  while (lim < p) {
    double angle = 2 * PI / (1 << (lim + 1));
    for (int i = 1 << (lim - 1); i < (1 << lim); i++) {
      roots[i << 1] = roots[i];
      double ai = angle * (2 * i + 1 - (1 << lim));
      roots[(i << 1) + 1] = base(cos(ai), sin(ai));
    }
    lim++;
  }
}

void fft(vector<base> &a, int n = -1) {
  if (n == -1) {
    n = a.size();
  }
  assert((n & (n - 1)) == 0);
  int zeros = __builtin_ctz(n);
  ensure_base(zeros);
  int shift = lim - zeros;
  for (int i = 0; i < n; i++) {
    if (i < (rev[i] >> shift)) {
      swap(a[i], a[rev[i] >> shift]);
    }
  }
  for (int k = 1; k < n; k <<= 1) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; j++) {
        base z = a[i + j + k] * roots[j + k];
        a[i + j + k] = a[i + j] - z;
        a[i + j] = a[i + j] + z;
      }
    }
  }
}

// eq=0: 4 FFTs, eq=1: 3 FFTs (squaring)
vector<int> multiply(vector<int> &a, vector<int> &b, int eq = 0) {
  int need = a.size() + b.size() - 1;
  int p = 0;
  while ((1 << p) < need) {
    p++;
  }
  ensure_base(p);
  int sz = 1 << p;
  vector<base> A(sz), B(sz);
  for (int i = 0; i < (int)a.size(); i++) {
    int x = (a[i] % mod + mod) % mod;
    A[i] = base(x & ((1 << 15) - 1), x >> 15);
  }
  fft(A, sz);
  if (eq) {
    copy(A.begin(), A.begin() + sz, B.begin());
  } else {
    for (int i = 0; i < (int)b.size(); i++) {
      int x = (b[i] % mod + mod) % mod;
      B[i] = base(x & ((1 << 15) - 1), x >> 15);
    }
    fft(B, sz);
  }
  double ratio = 0.25 / sz;
  base r2(0, -1), r3(ratio, 0), r4(0, -ratio), r5(0, 1);
  for (int i = 0; i <= (sz >> 1); i++) {
    int j = (sz - i) & (sz - 1);
    base a1 = (A[i] + conj(A[j])), a2 = (A[i] - conj(A[j])) * r2;
    base b1 = (B[i] + conj(B[j])) * r3, b2 = (B[i] - conj(B[j])) * r4;
    if (i != j) {
      base c1 = (A[j] + conj(A[i])), c2 = (A[j] - conj(A[i])) * r2;
      base d1 = (B[j] + conj(B[i])) * r3, d2 = (B[j] - conj(B[i])) * r4;
      A[i] = c1 * d1 + c2 * d2 * r5;
      B[i] = c1 * d2 + c2 * d1;
    }
    A[j] = a1 * b1 + a2 * b2 * r5;
    B[j] = a1 * b2 + a2 * b1;
  }
  fft(A, sz);
  fft(B, sz);
  vector<int> res(need);
  for (int i = 0; i < need; i++) {
    long long aa = A[i].x + 0.5, bb = B[i].x + 0.5, cc = A[i].y + 0.5;
    res[i] = (aa + (bb % mod << 15) + (cc % mod << 30)) % mod;
  }
  return res;
}

template <int MOD, int RT> struct ModInt {
  static_assert(MOD > 1 && MOD < (1 << 30));
  static constexpr int mod = MOD;
  static constexpr ModInt rt() { return RT; }

  unsigned int v;

  ModInt() : v(0) {}
  template <typename T> ModInt(T _v) : v(((long long)_v % MOD + MOD) % MOD) {}

  static ModInt raw(int _v) {
    ModInt x;
    x.v = _v;
    return x;
  }

  explicit operator int() const { return v; }
  explicit operator long long() const { return v; }

  bool operator==(ModInt o) const { return v == o.v; }
  bool operator!=(ModInt o) const { return v != o.v; }

  ModInt &operator+=(ModInt o) {
    if ((v += o.v) >= (unsigned)MOD) {
      v -= MOD;
    }
    return *this;
  }
  ModInt &operator-=(ModInt o) {
    if ((int)(v -= o.v) < 0) {
      v += MOD;
    }
    return *this;
  }
  ModInt &operator*=(ModInt o) {
    v = (long long)v * o.v % MOD;
    return *this;
  }
  ModInt &operator/=(ModInt o) { return *this *= inv(o); }

  friend ModInt operator+(ModInt a, ModInt b) { return a += b; }
  friend ModInt operator-(ModInt a, ModInt b) { return a -= b; }
  friend ModInt operator*(ModInt a, ModInt b) { return a *= b; }
  friend ModInt operator/(ModInt a, ModInt b) { return a /= b; }
  friend ModInt operator-(ModInt a) { return ModInt(MOD - a.v); }

  friend ModInt pow(ModInt a, long long p) {
    ModInt res = 1;
    for (; p > 0; p >>= 1, a *= a) {
      if (p & 1) {
        res *= a;
      }
    }
    return res;
  }
  friend ModInt inv(ModInt a) {
    assert(a.v != 0);
    int x = 1, y = 0, m = MOD, n = a.v;
    while (n > 1) {
      int q = m / n;
      m -= q * n;
      swap(m, n);
      y -= q * x;
      swap(x, y);
    }
    return ModInt(x);
  }

  struct Hash {
    size_t operator()(ModInt a) const { return hash<int>{}(a.v); }
  };

  friend ostream &operator<<(ostream &os, ModInt a) { return os << a.v; }
  friend istream &operator>>(istream &is, ModInt &a) {
    long long _v;
    is >> _v;
    a = ModInt(_v);
    return is;
  }
};

using mint = ModInt<mod, 3>;

// Combinatorics
struct combi {
  int n;
  vector<mint> facts, finvs, invs;
  combi(int _n) : n(_n), facts(_n), finvs(_n), invs(_n) {
    facts[0] = finvs[0] = 1;
    invs[1] = 1;
    for (int i = 2; i < n; i++) {
      invs[i] = invs[mod % i] * (-mod / i);
    }
    for (int i = 1; i < n; i++) {
      facts[i] = facts[i - 1] * i;
      finvs[i] = finvs[i - 1] * invs[i];
    }
  }
  mint fact(int n) { return facts[n]; }
  mint finv(int n) { return finvs[n]; }
  mint inv_val(int n) { return invs[n]; }
  mint ncr(int n, int k) { return facts[n] * finvs[k] * finvs[n - k]; }
} C(N);

// Polynomial helpers
vector<mint> multiply_mint(vector<mint> &a, vector<mint> &b) {
  vector<int> A, B;
  for (auto x : a) {
    A.push_back(x.v);
  }
  for (auto x : b) {
    B.push_back(x.v);
  }
  auto res = multiply(A, B, A == B);
  vector<mint> ans;
  for (auto x : res) {
    ans.push_back(mint(x));
  }
  while (ans.size() && ans.back() == 0) {
    ans.pop_back();
  }
  return ans;
}

vector<mint> add(vector<mint> a, vector<mint> b) {
  int n = max(a.size(), b.size());
  vector<mint> ans(n);
  for (int i = 0; i < n; i++) {
    ans[i] = (i < (int)a.size() ? a[i] : 0) + (i < (int)b.size() ? b[i] : 0);
  }
  while (ans.size() && ans.back() == 0) {
    ans.pop_back();
  }
  return ans;
}

vector<mint> t[N * 4];

void build(int id, int l, int r) {
  if (l == r) {
    t[id] = {-l, 1};
    return;
  }
  int mid = (l + r) >> 1;
  build(id << 1, l, mid);
  build(id << 1 | 1, mid + 1, r);
  t[id] = multiply_mint(t[id << 1], t[id << 1 | 1]);
}

vector<mint> yo(int id, int l, int r, vector<mint> &p) {
  if (l == r) {
    int n = (int)p.size() - 1;
    mint tmp = ((n - l) & 1 ? -1 : 1);
    return {p[l] * tmp * C.finv(l) * C.finv(n - l)};
  }
  int mid = (l + r) >> 1;
  auto L = yo(id << 1, l, mid, p);
  auto R = yo(id << 1 | 1, mid + 1, r, p);
  return add(multiply_mint(L, t[id << 1 | 1]), multiply_mint(t[id << 1], R));
}

vector<mint> Lagrange(vector<mint> &p) {
  int n = p.size();
  build(1, 0, n - 1);
  auto ans = yo(1, 0, n - 1, p);
  ans.resize(n);
  return ans;
}

// Polynomial_Sequence
struct Polynomial_Sequence {
  vector<mint> coeffs;
  Polynomial_Sequence(vector<mint> pts) { coeffs = Lagrange(pts); }
  mint query(mint x) {
    mint ans = 0, xpow = 1;
    for (auto c : coeffs) {
      ans += c * xpow;
      xpow *= x;
    }
    return ans;
  }
};

int main() {

  int n, k; cin >> n >> k;
  vector<mint> pts;
  mint sum = 0;
  pts.push_back(0);
  for (int i = 1; i <= k + 1; i++) {
    sum += pow(mint(i), k);
    pts.push_back(sum);
  }
  Polynomial_Sequence pes(pts);
  cout << pes.query(n) << '\n';
  return 0;
}
