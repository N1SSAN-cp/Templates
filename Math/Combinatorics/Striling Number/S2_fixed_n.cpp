// Stirling Number of the Second Kind - Fixed N
// S(n, k) for all k in [0, n], given fixed n
// O(n log n) | n <= 2e5, mod = 998244353
// Problem: Given n labeled balls, count ways to partition into exactly k non-empty
//          indistinguishable groups, for every k from 0 to n.
// Input:  n
// Output: S(n,0) S(n,1) ... S(n,n)

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 18;
const int mod = 998244353;
const int root = 3;
int lim, rev[N], w[N], wn[N], inv_lim;

void reduce(int &x) { x = (x + mod) % mod; }

int POW(int x, int y, int ans = 1) {
    for (; y; y >>= 1, x = (long long)x * x % mod) {
        if (y & 1) { ans = (long long)ans * x % mod; }
    }
    return ans;
}

void precompute(int len) {
    lim = wn[0] = 1; int s = -1;
    while (lim < len) { lim <<= 1; ++s; }
    for (int i = 0; i < lim; ++i) { rev[i] = rev[i >> 1] >> 1 | (i & 1) << s; }
    const int g = POW(root, (mod - 1) / lim);
    inv_lim = POW(lim, mod - 2);
    for (int i = 1; i < lim; ++i) { wn[i] = (long long)wn[i - 1] * g % mod; }
}

void ntt(vector<int> &a, int typ) {
    for (int i = 0; i < lim; ++i) { if (i < rev[i]) { swap(a[i], a[rev[i]]); } }
    for (int i = 1; i < lim; i <<= 1) {
        for (int j = 0, t = lim / i / 2; j < i; ++j) { w[j] = wn[j * t]; }
        for (int j = 0; j < lim; j += i << 1) {
            for (int k = 0; k < i; ++k) {
                const int x = a[k + j], y = (long long)a[k + j + i] * w[k] % mod;
                reduce(a[k + j] += y - mod); reduce(a[k + j + i] = x - y);
            }
        }
    }
    if (!typ) {
        reverse(a.begin() + 1, a.begin() + lim);
        for (int i = 0; i < lim; ++i) { a[i] = (long long)a[i] * inv_lim % mod; }
    }
}

vector<int> multiply(vector<int> &f, vector<int> &g) {
    int n = (int)f.size() + (int)g.size() - 1;
    precompute(n);
    vector<int> a = f, b = g;
    a.resize(lim); b.resize(lim);
    ntt(a, 1); ntt(b, 1);
    for (int i = 0; i < lim; ++i) { a[i] = (long long)a[i] * b[i] % mod; }
    ntt(a, 0);
    return a;
}

template <const int32_t MOD>
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
    modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) { y *= x; } x *= x; } return y; }
    modint<MOD> inv() const { return pow(MOD - 2); }
    inline modint<MOD> operator/(modint<MOD> o) const { return *this * o.inv(); }
    inline modint<MOD> &operator/=(modint<MOD> o) { return *this *= o.inv(); }
    inline bool operator==(modint<MOD> o) const { return value == o.value; }
    inline bool operator!=(modint<MOD> o) const { return value != o.value; }
};
template <int32_t MOD> modint<MOD> operator*(int64_t val, modint<MOD> n) { return modint<MOD>(val) * n; }
template <int32_t MOD> modint<MOD> operator*(int32_t val, modint<MOD> n) { return modint<MOD>(val % MOD) * n; }
template <int32_t MOD> istream &operator>>(istream &in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD> ostream &operator<<(ostream &out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

mint f[N];

// Stirling Number of the Second Kind, O(n log n)
// returns S(n, k) for k in [0, n]
vector<int> stirling(int n) {
    f[0] = 1;
    for (int i = 1; i <= n; i++) { f[i] = f[i - 1] * i; }
    vector<int> a(n + 1), b(n + 1);
    for (int i = 0; i <= n; i++) {
        a[i] = (f[i].inv() * (i & 1 ? mod - 1 : 1)).value;
    }
    for (int i = 0; i <= n; i++) {
        b[i] = (mint(i).pow(n) / f[i]).value;
    }
    auto ans = multiply(a, b);
    ans.resize(n + 1);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    auto ans = stirling(n); // ans[k] = S(n, k)
    for (int i = 0; i <= n; i++) {
        cout << ans[i] << " \n"[i == n];
    }
    return 0;
}
