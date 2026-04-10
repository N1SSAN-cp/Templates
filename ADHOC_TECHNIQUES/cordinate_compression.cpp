#include <bits/stdc++.h>
using namespace std;

// Coordinate Compression
// Maps arbitrary values to compact indices [0, n)
// add() + build(): O(n log n) | getComp/getOrg/getPrev/getNext: O(log n)

struct CoordinateCompression {
    map<long long, long long> mpp;
    vector<long long> rev;

    void add(long long x) { mpp[x]; }

    void build() {
        long long ct = 0;
        for (auto [n, _] : mpp) {
            rev.push_back(n);
            mpp[n] = ct++;
        }
    }

    long long getComp(long long x) { return mpp[x]; }
    long long getOrg(long long x) { return rev[x]; }

    long long getPrev(long long x) {
        auto it = mpp.upper_bound(x);
        it--;
        return it->second;
    }

    long long getNext(long long x) {
        auto it = mpp.lower_bound(x);
        return it->second;
    }
};

void solve() {
    CoordinateCompression cc;

    vector<long long> vals = {10, 100, 1000, 50, 10};
    for (long long v : vals) {
        cc.add(v);
    }
    cc.build();
    // compressed indices: 10->0, 50->1, 100->2, 1000->3

    cout << cc.getComp(100) << "\n";  // 2
    cout << cc.getOrg(2) << "\n";     // 100

    // largest compressed index with original value <= 60
    cout << cc.getPrev(60) << "\n";  // 1  (which is 50)

    // smallest compressed index with original value >= 60
    cout << cc.getNext(60) << "\n";  // 2  (which is 100)
}

int main() { solve(); }
