#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using ll = long long;

u64 rng64() {
  static u64 x =
      (u64)chrono::high_resolution_clock::now().time_since_epoch().count() *
      10150724397891781847ULL;
  x ^= x << 7;
  return x ^= x >> 9;
}

struct modint61 {
  static constexpr u64 mod = (1ULL << 61) - 1;
  u64 val;
  constexpr modint61() : val(0) {}
  constexpr modint61(u64 x) : val(x % mod) {}
  constexpr modint61(ll x) : val((x %= (ll)mod) < 0 ? x + (ll)mod : x) {}
  constexpr modint61(int x) : val(x < 0 ? (ll)x + (ll)mod : (u64)x) {}
  constexpr modint61(char x) : val((u64)(unsigned char)x) {}
  static constexpr u64 get_mod() { return mod; }
  modint61 &operator+=(const modint61 &a) {
    val = (val += a.val) >= mod ? val - mod : val;
    return *this;
  }
  modint61 &operator-=(const modint61 &a) {
    val = (val -= a.val) >= mod ? val + mod : val;
    return *this;
  }
  modint61 &operator*=(const modint61 &a) {
    unsigned __int128 y = (unsigned __int128)val * a.val;
    val = (y >> 61) + (y & mod);
    val = val >= mod ? val - mod : val;
    return *this;
  }
  modint61 operator+(const modint61 &p) const { return modint61(*this) += p; }
  modint61 operator-(const modint61 &p) const { return modint61(*this) -= p; }
  modint61 operator*(const modint61 &p) const { return modint61(*this) *= p; }
  bool operator==(const modint61 &p) const { return val == p.val; }
  bool operator!=(const modint61 &p) const { return val != p.val; }
};

struct RollingHash {
  using mint = modint61;
  static constexpr u64 mod = mint::get_mod();

  const mint base;
  vector<mint> power{mint((u64)1)};

  RollingHash(mint base = mint((u64)(rng64() % mod))) : base(base) {}

  void expand(int sz) {
    while ((int)power.size() <= sz) {
      power.push_back(power.back() * base);
    }
  }

  template <typename STRING> vector<mint> build(const STRING &s) {
    int n = (int)s.size();
    vector<mint> h(n + 1, mint((u64)0));
    for (int i = 0; i < n; i++) {
      h[i + 1] = h[i] * base + mint(s[i]);
    }
    return h;
  }

  template <typename STRING> mint eval(const STRING &s) {
    mint x((u64)0);
    for (auto c : s) {
      x = base * x + mint(c);
    }
    return x;
  }

  mint query(const vector<mint> &h, int l, int r) {
    expand(r - l);
    return h[r] - h[l] * power[r - l];
  }

  mint combine(mint h1, mint h2, int h2len) {
    expand(h2len);
    return h1 * power[h2len] + h2;
  }

  mint add_char(mint h, int x) { return h * base + mint(x); }

  int lcp(const vector<mint> &a, int l1, int r1, const vector<mint> &b, int l2,
          int r2) {
    int len = min(r1 - l1, r2 - l2), lo = 0, hi = len + 1;
    while (hi - lo > 1) {
      int mid = (lo + hi) / 2;
      if (query(a, l1, l1 + mid) == query(b, l2, l2 + mid)) {
        lo = mid;
      } else {
        hi = mid;
      }
    }
    return lo;
  }
};
int main() {
  string s1 = "abracadabra";
  string s2 = "acadabra";

  // Initialize rolling hash (uses your RNG)
  RollingHash rh;

  // Build prefix hashes
  auto h1 = rh.build(s1);
  auto h2 = rh.build(s2);

  // Example 1: Compare substrings s1[3..7) and s2[0..4)
  int l1 = 3, r1 = 7;
  int l2 = 0, r2 = 4;

  if (rh.query(h1, l1, r1) == rh.query(h2, l2, r2)) {
    cout << "Substrings are equal!\n";
  } else {
    cout << "Substrings are different!\n";
  }

  // Example 2: Full string hash
  auto hash1 = rh.eval(s1);
  auto hash2 = rh.eval(s2);
  cout << "Hash of s1: " << hash1.val << "\n";
  cout << "Hash of s2: " << hash2.val << "\n";

  // Example 3: Longest Common Prefix (LCP)
  int lcp_len = rh.lcp(h1, 0, s1.size(), h2, 0, s2.size());
  cout << "Longest Common Prefix length: " << lcp_len << "\n";
  cout << "LCP string: " << s1.substr(0, lcp_len) << "\n";

  // Example 4: Combine hashes (s1[0..3) + s2[0..4))
  auto combined = rh.combine(rh.query(h1, 0, 3), rh.query(h2, 0, 4), 4);
  cout << "Combined hash: " << combined.val << "\n";

  return 0;
}

