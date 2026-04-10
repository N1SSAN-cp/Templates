#include <bits/stdc++.h>
using namespace std;

using T = __int128;
// Extended Euclidean Algorithm — O(log(min(a, b)))
// ax + by = gcd(a, b), returns gcd(a, b)
T extended_euclid(T a, T b, T &x, T &y) {
    T xx = y = 0;
    T yy = x = 1;
    while (b) {
        T q = a / b;
        T t = b; b = a % b; a = t;
        t = xx; xx = x - q * xx; x = t;
        t = yy; yy = y - q * yy; y = t;
    }
    return a;
}

// Chinese Remainder Theorem (pair) — O(log(min(m1, m2)))
// finds x such that x % m1 = a1, x % m2 = a2, m1 and m2 may not be coprime
// x is unique modulo m = lcm(m1, m2). returns (x, m). on failure, m = -1.
pair<T, T> CRT(T a1, T m1, T a2, T m2) {
    T p, q;
    T g = extended_euclid(m1, m2, p, q);
    if (a1 % g != a2 % g) { return {0, -1}; }
    T m = m1 / g * m2;
    p = (p % m + m) % m;
    q = (q % m + m) % m;
    return {(p * a2 % m * (m1 / g) % m + q * a1 % m * (m2 / g) % m) % m, m};
}

// Chinese Remainder Theorem (general) — O(n log(max(m)))
// given a[] and m[], finds x such that x % m[i] = a[i] for all i
// returns (x, m) where m = lcm of all m[i]. on failure, m = -1.
pair<T, T> CRT(vector<T> a, vector<T> m) {
    T a1 = a[0], m1 = m[0];
    for (int i = 1; i < (int)a.size(); i++) {
        auto [a2, m2] = CRT(a1, m1, a[i], m[i]);
        if (m2 == -1) { return {0, -1}; }
        a1 = a2;
        m1 = m2;
    }
    return {a1, m1};
}
