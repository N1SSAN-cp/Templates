#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    static const int base = 1000000000;
    static const int base_digits = 9;

    vector<int> a;
    int sign = 1;

    BigInt() = default;
    BigInt(long long v)     { *this = v; }
    BigInt(const string& s) { read(s); }
    BigInt(const char* s)   { read(string(s)); }

    void operator=(long long v) {
        sign = (v < 0) ? -1 : 1;
        v = abs(v);
        a.clear();
        for (; v > 0; v /= base) { a.push_back(v % base); }
    }

    void trim() {
        while (!a.empty() && !a.back()) { a.pop_back(); }
        if (a.empty()) { sign = 1; }
    }

    bool isZero() const { return a.empty(); }

    friend bool operator==(const BigInt& x, const BigInt& y) {
        return x.sign == y.sign && x.a == y.a;
    }
    friend bool operator<(const BigInt& x, const BigInt& y) {
        if (x.sign != y.sign) { return x.sign < y.sign; }
        if (x.a.size() != y.a.size()) {
            return x.sign == 1 ? x.a.size() < y.a.size()
                               : x.a.size() > y.a.size();
        }
        for (int i = x.a.size() - 1; i >= 0; i--) {
            if (x.a[i] != y.a[i]) {
                return x.sign == 1 ? x.a[i] < y.a[i] : x.a[i] > y.a[i];
            }
        }
        return false;
    }
    friend bool operator!=(const BigInt& x, const BigInt& y) { return !(x == y); }
    friend bool operator> (const BigInt& x, const BigInt& y) { return y < x; }
    friend bool operator<=(const BigInt& x, const BigInt& y) { return !(y < x); }
    friend bool operator>=(const BigInt& x, const BigInt& y) { return !(x < y); }

    BigInt operator-() const {
        BigInt res = *this;
        if (!isZero()) { res.sign = -sign; }
        return res;
    }

    friend BigInt abs(BigInt v)                              { v.sign = 1; return v; }
    friend BigInt min(const BigInt& x, const BigInt& y)     { return x < y ? x : y; }
    friend BigInt max(const BigInt& x, const BigInt& y)     { return x > y ? x : y; }

    friend BigInt operator+(BigInt x, const BigInt& y) {
        if (x.sign == y.sign) {
            int carry = 0;
            for (size_t i = 0; i < max(x.a.size(), y.a.size()) || carry; ++i) {
                if (i == x.a.size()) { x.a.push_back(0); }
                x.a[i] += carry + (i < y.a.size() ? y.a[i] : 0);
                carry = x.a[i] >= base;
                if (carry) { x.a[i] -= base; }
            }
            return x;
        }
        return x - (-y);
    }

    friend BigInt operator-(BigInt x, const BigInt& y) {
        if (y.isZero()) { return x; }
        if (x.sign == y.sign) {
            if (abs(x) >= abs(y)) {
                int carry = 0;
                for (size_t i = 0; i < y.a.size() || carry; ++i) {
                    x.a[i] -= carry + (i < y.a.size() ? y.a[i] : 0);
                    carry = x.a[i] < 0;
                    if (carry) { x.a[i] += base; }
                }
                x.trim();
                return x;
            }
            return -(y - x);
        }
        return x + (-y);
    }

    friend BigInt operator*(const BigInt& x, const BigInt& y) {
        BigInt res;
        res.sign = x.sign * y.sign;
        res.a.assign(x.a.size() + y.a.size(), 0);
        for (size_t i = 0; i < x.a.size(); ++i) {
            long long carry = 0;
            for (size_t j = 0; j < y.a.size() || carry; ++j) {
                long long cur = res.a[i + j]
                    + (long long)x.a[i] * (j < y.a.size() ? y.a[j] : 0)
                    + carry;
                res.a[i + j] = cur % base;
                carry = cur / base;
            }
        }
        res.trim();
        return res;
    }

    friend BigInt operator/(const BigInt& x, int v) {
        assert(v != 0);
        BigInt res = x;
        res.sign *= (v < 0 ? -1 : 1);
        v = abs(v);
        long long rem = 0;
        for (int i = res.a.size() - 1; i >= 0; i--) {
            long long cur = res.a[i] + rem * (long long)base;
            res.a[i] = cur / v;
            rem = cur % v;
        }
        res.trim();
        return res;
    }

    friend int operator%(const BigInt& x, int v) {
        assert(v != 0);
        if (v < 0) { v = -v; }
        long long m = 0;
        for (int i = x.a.size() - 1; i >= 0; i--) {
            m = (x.a[i] + m * (long long)base) % v;
        }
        return (int)(m * x.sign);
    }

    friend pair<BigInt, BigInt> divmod(const BigInt& x, const BigInt& y) {
        assert(!y.isZero());
        BigInt a = abs(x), b = abs(y);
        BigInt q, r;
        q.a.resize(a.a.size());
        for (int i = a.a.size() - 1; i >= 0; --i) {
            r.a.insert(r.a.begin(), a.a[i]);
            r.trim();
            int lo = 0, hi = base - 1;
            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (b * mid <= r) { lo = mid; }
                else { hi = mid - 1; }
            }
            q.a[i] = lo;
            r -= b * lo;
        }
        q.sign = x.sign * y.sign;
        r.sign = x.sign;
        q.trim();
        r.trim();
        return {q, r};
    }
    friend BigInt operator/(const BigInt& x, const BigInt& y) { return divmod(x, y).first;  }
    friend BigInt operator%(const BigInt& x, const BigInt& y) { return divmod(x, y).second; }

    BigInt& operator+=(const BigInt& y) { return *this = *this + y; }
    BigInt& operator-=(const BigInt& y) { return *this = *this - y; }
    BigInt& operator*=(const BigInt& y) { return *this = *this * y; }
    BigInt& operator/=(const BigInt& y) { return *this = *this / y; }
    BigInt& operator%=(const BigInt& y) { return *this = *this % y; }
    BigInt& operator/=(int v)           { return *this = *this / v; }

    friend BigInt gcd(BigInt x, BigInt y) {
        x = abs(x);
        y = abs(y);
        while (!y.isZero()) { x %= y; swap(x, y); }
        return x;
    }
    friend BigInt lcm(const BigInt& x, const BigInt& y) {
        return abs(x / gcd(x, y) * y);
    }

    void read(const string& s) {
        sign = 1;
        a.clear();
        int pos = 0;
        while (pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') { sign = -sign; }
            ++pos;
        }
        for (int i = s.size() - 1; i >= pos; i -= base_digits) {
            int x = 0;
            for (int j = max(pos, i - base_digits + 1); j <= i; j++) {
                x = x * 10 + (s[j] - '0');
            }
            a.push_back(x);
        }
        trim();
    }

    friend ostream& operator<<(ostream& out, const BigInt& v) {
        if (v.sign == -1 && !v.isZero()) { out << '-'; }
        out << (v.a.empty() ? 0 : v.a.back());
        for (int i = v.a.size() - 2; i >= 0; --i) {
            out << setw(base_digits) << setfill('0') << v.a[i];
        }
        return out;
    }
    friend istream& operator>>(istream& in, BigInt& v) {
        string s;
        in >> s;
        v.read(s);
        return in;
    }
};
