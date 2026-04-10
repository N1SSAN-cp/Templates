#include <bits/stdc++.h>
using namespace std;

// nCr precomputed with mint — mod handled by mint automatically
// Precompute: O(n), Query: O(1),
// Requires prime MOD
using namespace std;

template <int MOD, int RT>
struct mint {
    static_assert(MOD > 1 && MOD < (1 << 30));
    static constexpr int mod = MOD;
    static constexpr mint rt() { return RT; }
    unsigned int v;
    mint() : v(0) {}
    template <typename T>
    mint(T _v) : v(((long long)_v % MOD + MOD) % MOD) {}

    static mint raw(int _v) {
        mint x;
        x.v = _v;
        return x;
    }
    explicit operator int() const { return v; }
    // explicit operator long long() const { return v; }
    bool operator==(mint o) const { return v == o.v; }
    bool operator!=(mint o) const { return v != o.v; }
    mint& operator+=(mint o) {
        if ((v += o.v) >= (unsigned)MOD) v -= MOD;
        return *this;
    }
    mint& operator-=(mint o) {
        if ((int)(v -= o.v) < 0) v += MOD;
        return *this;
    }
    mint& operator*=(mint o) {
        v = (long long)v * o.v % MOD;
        return *this;
    }
    mint& operator/=(mint o) { return *this *= inv(o); }

    friend mint operator+(mint a, mint b) { return a += b; }
    friend mint operator-(mint a, mint b) { return a -= b; }
    friend mint operator*(mint a, mint b) { return a *= b; }
    friend mint operator/(mint a, mint b) { return a /= b; }
    friend mint operator-(mint a) { return mint(MOD - a.v); }
    friend mint pow(mint a, long long p) {
        mint res = 1;
        for (; p > 0; p >>= 1, a *= a)
            if (p & 1) res *= a;
        return res;
    }
    friend mint inv(mint a) {
        assert(a.v != 0);
        int x = 1, y = 0, m = MOD, n = a.v;
        while (n > 1) {
            int q = m / n;
            m -= q * n, std::swap(m, n);
            y -= q * x, std::swap(x, y);
        }
        return mint(x);
    }

    struct Hash {
        size_t operator()(mint a) const { return std::hash<int>{}(a.v); }
    };

    friend std::ostream& operator<<(std::ostream& os, mint a) { return os << a.v; }
    friend std::istream& operator>>(std::istream& is, mint& a) {
        long long _v;
        is >> _v, a = mint(_v);
        return is;
    }
};

using mi = mint<(int)1e9 + 7, 3>;
const int maxN = 1e6;

template <typename mint>
struct combo {
    static constexpr int maxN = 1e6;
    vector<mint> fact_m, inv_fact_m;

    combo() {
        fact_m.resize(maxN + 1);
        inv_fact_m.resize(maxN + 1);
        fact_m[0] = 1;
        for (int i = 1; i <= maxN; i++) {
            fact_m[i] = fact_m[i - 1] * i;
        }
        inv_fact_m[maxN] = inv(fact_m[maxN]);
        for (int i = maxN - 1; i >= 0; i--) {
            inv_fact_m[i] = inv_fact_m[i + 1] * (i + 1);
        }
    }

    mint nCr(int n, int r) {
        assert(n >= 0 && n <= maxN);
        assert(r >= 0 && r <= n);
        if (r < 0 || r > n) {
            return 0;
        }
        return fact_m[n] * inv_fact_m[r] * inv_fact_m[n - r];
    }
};

int main() {
    combo<mi> ncr;
    cout << ncr.nCr(10, 3);
}
