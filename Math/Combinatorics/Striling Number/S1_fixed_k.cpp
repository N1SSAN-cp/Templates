// Stirling Number of the First Kind - Fixed K
// [i, k] (unsigned) for all i in [0, n), given fixed k
// O(n log^2 n) | n, k <= 2e5, mod = 998244353
// Problem: For fixed k, count permutations of {1..i} with exactly k cycles,
//          for every i from 0 to n-1.
// Input:  n k
// Output: [0,k] [1,k] ... [n-1,k]
// Reference: https://codeforces.com/blog/entry/77468

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

// polynomial power: a^k truncated to first n terms, O(n log^2 n)
vector<int> power(vector<int> a, int k, int n) {
    assert((int)a.size() <= n);
    vector<int> ans({1});
    while (k) {
        if (k & 1) { ans = multiply(ans, a); ans.resize(n); }
        a = multiply(a, a);
        a.resize(n);
        k >>= 1;
    }
    return ans;
}

// Stirling Number of the First Kind - Fixed K, O(n log^2 n)
// returns [i, k] for i in [0, n)
vector<int> stirling(int n, int k) {
    assert(k <= n);
    vector<int> a(n);
    a[0] = 0;
    for (int i = 1; i < n; i++) { a[i] = POW(i, mod - 2); }
    auto ans = power(a, k, n);
    ans.resize(n);
    int f = 1;
    for (int i = 1; i <= k; i++) { f = 1LL * f * i % mod; }
    int cur = 1;
    for (int i = 0; i < n; i++) {
        ans[i] = 1LL * ans[i] * cur % mod;
        cur = 1LL * cur * (i + 1) % mod;
        ans[i] = 1LL * ans[i] * POW(f, mod - 2) % mod;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    auto ans = stirling(n, k); // ans[i] = [i, k]
    for (int i = 0; i < n; i++) {
        cout << ans[i] << " \n"[i == n - 1];
    }
    return 0;
}
