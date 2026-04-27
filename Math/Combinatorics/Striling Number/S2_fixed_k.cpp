// Stirling Number of the Second Kind - Fixed K
// S(i, k) for all i in [0, n], given fixed k
// O(n log^2 n) | n, k <= 3e5, mod = 1e9+7
// Problem: Given fixed k non-empty indistinguishable groups, count ways to partition
//          i labeled elements into exactly k groups, for every i from 0 to n.
// Input:  n k
// Output: S(0,k) S(1,k) ... S(n,k)
// Reference: https://codeforces.com/contest/961/problem/G

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

struct base {
    double x, y;
    base() { x = y = 0; }
    base(double x, double y) : x(x), y(y) {}
};
inline base operator+(base a, base b) { return base(a.x + b.x, a.y + b.y); }
inline base operator-(base a, base b) { return base(a.x - b.x, a.y - b.y); }
inline base operator*(base a, base b) { return base(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline base conj(base a) { return base(a.x, -a.y); }

int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};
const double PI = acosl(-1.0);

void ensure_base(int p) {
    if (p <= lim) { return; }
    rev.resize(1 << p);
    for (int i = 0; i < (1 << p); i++) { rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (p - 1)); }
    roots.resize(1 << p);
    while (lim < p) {
        double angle = 2 * PI / (1 << (lim + 1));
        for (int i = 1 << (lim - 1); i < (1 << lim); i++) {
            roots[i << 1] = roots[i];
            double angle_i = angle * (2 * i + 1 - (1 << lim));
            roots[(i << 1) + 1] = base(cos(angle_i), sin(angle_i));
        }
        lim++;
    }
}

void fft(vector<base> &a, int n = -1) {
    if (n == -1) { n = a.size(); }
    assert((n & (n - 1)) == 0);
    int zeros = __builtin_ctz(n);
    ensure_base(zeros);
    int shift = lim - zeros;
    for (int i = 0; i < n; i++) { if (i < (rev[i] >> shift)) { swap(a[i], a[rev[i] >> shift]); } }
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

// eq=0: 4 FFTs, eq=1: 3 FFTs
vector<int> multiply(vector<int> &a, vector<int> &b, int eq = 0) {
    int need = a.size() + b.size() - 1;
    int p = 0;
    while ((1 << p) < need) { p++; }
    ensure_base(p);
    int sz = 1 << p;
    vector<base> A, B;
    if (sz > (int)A.size()) { A.resize(sz); }
    for (int i = 0; i < (int)a.size(); i++) {
        int x = (a[i] % mod + mod) % mod;
        A[i] = base(x & ((1 << 15) - 1), x >> 15);
    }
    fill(A.begin() + a.size(), A.begin() + sz, base{0, 0});
    fft(A, sz);
    if (sz > (int)B.size()) { B.resize(sz); }
    if (eq) {
        copy(A.begin(), A.begin() + sz, B.begin());
    } else {
        for (int i = 0; i < (int)b.size(); i++) {
            int x = (b[i] % mod + mod) % mod;
            B[i] = base(x & ((1 << 15) - 1), x >> 15);
        }
        fill(B.begin() + b.size(), B.begin() + sz, base{0, 0});
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
    fft(A, sz); fft(B, sz);
    vector<int> res(need);
    for (int i = 0; i < need; i++) {
        long long aa = A[i].x + 0.5;
        long long bb = B[i].x + 0.5;
        long long cc = A[i].y + 0.5;
        res[i] = (aa + ((bb % mod) << 15) + ((cc % mod) << 30)) % mod;
    }
    return res;
}

template <int32_t MOD>
struct modint {
    int32_t value;
    modint() = default;
    modint(int32_t value_) : value(value_) {}
    inline modint<MOD> operator+(modint<MOD> o) const { int32_t c = value + o.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
    inline modint<MOD> operator-(modint<MOD> o) const { int32_t c = value - o.value; return modint<MOD>(c < 0 ? c + MOD : c); }
    inline modint<MOD> operator*(modint<MOD> o) const { int32_t c = (int64_t)value * o.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
    inline modint<MOD> &operator+=(modint<MOD> o) { value += o.value; if (value >= MOD) { value -= MOD; } return *this; }
    inline modint<MOD> &operator-=(modint<MOD> o) { value -= o.value; if (value < 0) { value += MOD; } return *this; }
    inline modint<MOD> &operator*=(modint<MOD> o) { value = (int64_t)value * o.value % MOD; if (value < 0) { value += MOD; } return *this; }
    inline modint<MOD> operator-() const { return modint<MOD>(value ? MOD - value : 0); }
    modint<MOD> pow(uint64_t k) const {
        modint<MOD> x = *this, y = 1;
        for (; k; k >>= 1) { if (k & 1) { y *= x; } x *= x; }
        return y;
    }
    modint sqrt() const {
        if (value == 0) { return 0; }
        if (MOD == 2) { return 1; }
        if (pow((MOD - 1) >> 1) == MOD - 1) { return 0; }
        unsigned int Q = MOD - 1, M = 0, i;
        modint zQ;
        while (!(Q & 1)) { Q >>= 1; M++; }
        for (int z = 1;; z++) {
            if (modint(z).pow((MOD - 1) >> 1) == MOD - 1) { zQ = modint(z).pow(Q); break; }
        }
        modint t = pow(Q), R = pow((Q + 1) >> 1), r;
        while (true) {
            if (t == 1) { r = R; break; }
            for (i = 1; modint(t).pow(1 << i) != 1; i++) {}
            modint b = modint(zQ).pow(1 << (M - 1 - i));
            M = i; zQ = b * b; t = t * zQ; R = R * b;
        }
        return min(r, -r + MOD);
    }
    modint<MOD> inv() const { return pow(MOD - 2); }
    inline modint<MOD> operator/(modint<MOD> o) const { return *this * o.inv(); }
    inline modint<MOD> &operator/=(modint<MOD> o) { return *this *= o.inv(); }
    inline bool operator==(modint<MOD> o) const { return value == o.value; }
    inline bool operator!=(modint<MOD> o) const { return value != o.value; }
    inline bool operator<(modint<MOD> o) const { return value < o.value; }
    inline bool operator>(modint<MOD> o) const { return value > o.value; }
};
template <int32_t MOD> modint<MOD> operator*(int64_t val, modint<MOD> n) { return modint<MOD>(val) * n; }
template <int32_t MOD> modint<MOD> operator*(int32_t val, modint<MOD> n) { return modint<MOD>(val % MOD) * n; }
template <int32_t MOD> ostream &operator<<(ostream &out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

struct poly {
    vector<mint> a;
    inline void normalize() { while ((int)a.size() && a.back() == 0) { a.pop_back(); } }
    template <class... Args> poly(Args... args) : a(args...) {}
    poly(const initializer_list<mint> &x) : a(x.begin(), x.end()) {}
    int size() const { return (int)a.size(); }
    inline mint coef(const int i) const { return (i < (int)a.size() && i >= 0) ? a[i] : mint(0); }
    mint operator[](const int i) const { return (i < (int)a.size() && i >= 0) ? a[i] : mint(0); }
    bool is_zero() const { for (int i = 0; i < size(); i++) { if (a[i] != 0) { return 0; } } return 1; }
    poly operator+(const poly &x) const {
        int n = max(size(), x.size()); vector<mint> ans(n);
        for (int i = 0; i < n; i++) { ans[i] = coef(i) + x.coef(i); }
        while ((int)ans.size() && ans.back() == 0) { ans.pop_back(); }
        return ans;
    }
    poly operator-(const poly &x) const {
        int n = max(size(), x.size()); vector<mint> ans(n);
        for (int i = 0; i < n; i++) { ans[i] = coef(i) - x.coef(i); }
        while ((int)ans.size() && ans.back() == 0) { ans.pop_back(); }
        return ans;
    }
    poly operator*(const poly &b) const {
        if (is_zero() || b.is_zero()) { return {}; }
        vector<int> A, B;
        for (auto x : a) { A.push_back(x.value); }
        for (auto x : b.a) { B.push_back(x.value); }
        auto res = multiply(A, B, (A == B));
        vector<mint> ans;
        for (auto x : res) { ans.push_back(mint(x)); }
        while ((int)ans.size() && ans.back() == 0) { ans.pop_back(); }
        return ans;
    }
    poly operator*(const mint &x) const {
        int n = size(); vector<mint> ans(n);
        for (int i = 0; i < n; i++) { ans[i] = a[i] * x; }
        return ans;
    }
    poly operator/(const mint &x) const { return (*this) * x.inv(); }
    poly &operator+=(const poly &x) { return *this = (*this) + x; }
    poly &operator-=(const poly &x) { return *this = (*this) - x; }
    poly &operator*=(const poly &x) { return *this = (*this) * x; }
    poly &operator*=(const mint &x) { return *this = (*this) * x; }
    poly &operator/=(const mint &x) { return *this = (*this) / x; }
    poly mod_xk(int k) const { return {a.begin(), a.begin() + min(k, size())}; }
    poly mul_xk(int k) const { poly ans(*this); ans.a.insert(ans.a.begin(), k, 0); return ans; }
    poly div_xk(int k) const { return vector<mint>(a.begin() + min(k, (int)a.size()), a.end()); }
    poly inverse(int n) const {
        assert(!is_zero()); assert(a[0] != 0);
        poly ans{mint(1) / a[0]};
        for (int i = 1; i < n; i *= 2) {
            ans = (ans * mint(2) - ans * ans * mod_xk(2 * i)).mod_xk(2 * i);
        }
        return ans.mod_xk(n);
    }
};

struct Combi {
    int n; vector<mint> facts, finvs, invs;
    Combi(int _n) : n(_n), facts(_n), finvs(_n), invs(_n) {
        facts[0] = finvs[0] = 1;
        invs[1] = 1;
        for (int i = 2; i < n; i++) { invs[i] = invs[mod % i] * (-mod / i); }
        for (int i = 1; i < n; i++) { facts[i] = facts[i - 1] * i; finvs[i] = finvs[i - 1] * invs[i]; }
    }
    inline mint ncr(int n, int k) { return n < k ? 0 : facts[n] * finvs[k] * finvs[n - k]; }
};
Combi C(N);

// builds product (1-x)(1-2x)...(1-kx) via divide and conquer
void build(int l, int r, poly &ans) {
    if (l == r) { ans = poly({1, mod - l}); return; }
    int mid = (l + r) >> 1;
    poly a, b;
    build(l, mid, a);
    build(mid + 1, r, b);
    ans = a * b;
}

// Stirling Number of the Second Kind - Fixed K, O(n log^2 n)
// returns S(i, k) for i in [0, n]
vector<mint> stirling(int n, int k) {
    poly p;
    build(1, k, p);
    p = p.inverse(n + 1);
    auto ans = p.a;
    ans.insert(ans.begin(), k, 0);
    ans.resize(n + 1);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    auto ans = stirling(n, k); // ans[i] = S(i, k)
    for (int i = 0; i <= n; i++) {
        cout << ans[i] << " \n"[i == n];
    }
    return 0;
}
