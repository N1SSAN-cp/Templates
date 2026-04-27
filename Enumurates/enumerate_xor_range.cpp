#include <bits/stdc++.h>
using namespace std;

// Enumerates intervals [L, R) such that (x ^ a) is constant for x in [L, R)
// Covers full range [lo, hi)
// Time: O(log(hi))  Space: O(1)
template <typename F>
void enumerate_xor_range(long long a, long long lo, long long hi, F query) {
    for (int k = 0; k < 64; ++k) {
        if (lo >= hi) break;
        long long b = 1LL << k;
        if (lo & b) { query(lo ^ a, (lo ^ a) + b); lo += b; }
        if (hi & b) { query((hi - b) ^ a, ((hi - b) ^ a) + b); hi -= b; }
        if (a & b) a ^= b;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // Find mex of {n^x : x in [0, m]}
    int n = 5, m = 7;
    vector<array<long long, 2>> ranges;
    enumerate_xor_range(n, 0, m + 1, [&](long long L, long long R) {
        ranges.push_back({L, R});
    });
    sort(ranges.begin(), ranges.end());
    long long mex = 0;
    for (auto [L, R] : ranges) {
        if (L > mex) break;
        mex = max(mex, R);
    }
    cout << "mex of {5^x : x in [0,7]} = " << mex << "\n";
}

// Output:
// mex of {5^x : x in [0,7]} = 8
