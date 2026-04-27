#include <bits/stdc++.h>
using namespace std;

// Pollard's Rho Factorization
// Preprocess: O(P), Per query: O(n^(1/4) * log(n))

namespace PollardRho {
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
const int P = 1e6 + 9;
long long seq[P]; int primes[P], spf[P];

inline long long add_mod(long long x, long long y, long long m) {
  return (x += y) < m ? x : x - m;
}
inline long long mul_mod(long long x, long long y, long long m) {
  return __int128(x) * y % m;
}
inline long long pow_mod(long long x, long long n, long long m) {
  long long res = 1 % m;
  for (; n; n >>= 1) {
    if (n & 1) {
      res = mul_mod(res, x, m);
    }
    x = mul_mod(x, x, m);
  }
  return res;
}

// Miller-Rabin primality test: O(k * log^2(n)), k <= 11 bases (primes < 32),
// deterministic for n < 3.3e24 is_prime(n): call miller_rabin(n) -> true if
// prime, valid for n <= 9.2e18
inline bool miller_rabin(long long n) {
  if (n <= 2 || (n & 1 ^ 1)) {
    return (n == 2);
  }
  if (n < P) {
    return spf[n] == n;
  }
  long long c, d, s = 0, r = n - 1;
  for (; !(r & 1); r >>= 1, s++) {
  }
  for (int i = 0; primes[i] < n && primes[i] < 32; i++) {
    c = pow_mod(primes[i], r, n);
    for (int j = 0; j < s; j++) {
      d = mul_mod(c, c, n);
      if (d == 1 && c != 1 && c != (n - 1)) {
        return false;
      }
      c = d;
    }
    if (c != 1) {
      return false;
    }
  }
  return true;
}

// Linear sieve: O(n)
void init() {
  int cnt = 0;
  for (int i = 2; i < P; i++) {
    if (!spf[i]) {
      primes[cnt++] = spf[i] = i;
    }
    for (int j = 0, k; (k = i * primes[j]) < P; j++) {
      spf[k] = primes[j];
      if (spf[i] == spf[k]) {
        break;
      }
    }
  }
}

// Pollard's Rho: O(n^(1/4))
long long pollard_rho(long long n) {
  while (1) {
    long long x = rnd() % n, y = x, c = rnd() % n, u = 1, v, t = 0;
    long long *px = seq, *py = seq;
    while (1) {
      *py++ = y = add_mod(mul_mod(y, y, n), c, n);
      *py++ = y = add_mod(mul_mod(y, y, n), c, n);
      if ((x = *px++) == y) {
        break;
      }
      v = u;
      u = mul_mod(u, abs(y - x), n);
      if (!u) {
        return __gcd(v, n);
      }
      if (++t == 32) {
        t = 0;
        if ((u = __gcd(u, n)) > 1 && u < n) {
          return u;
        }
      }
    }
    if (t && (u = __gcd(u, n)) > 1 && u < n) {
      return u;
    }
  }
}

// Factorization: O(n^(1/4) * log(n))
vector<long long> factorize(long long n) {
  if (n == 1) {
    return vector<long long>();
  }
  if (miller_rabin(n)) {
    return vector<long long>{n};
  }
  vector<long long> v, w;
  while (n > 1 && n < P) {
    v.push_back(spf[n]);
    n /= spf[n];
  }
  if (n >= P) {
    long long x = pollard_rho(n);
    v = factorize(x);
    w = factorize(n / x);
    v.insert(v.end(), w.begin(), w.end());
  }
  return v;
}
} // namespace PollardRho

int main() {
  PollardRho::init();

  long long n; cin >> n;
  auto f = PollardRho::factorize(n);
  sort(f.begin(), f.end());
  cout << f.size() << ' ';
  for (auto x : f) {
    cout << x << ' ';
  }
  cout << '\n';
  return 0;
}
