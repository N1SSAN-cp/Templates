#include <bits/stdc++.h>
using namespace std;

struct IntervalUnion {
    map<int, int> mp;  // disjoint intervals [l,r) (always sorted, non-overlapping)

    // find first interval that may overlap L
    auto lb(int L) {
        auto it = mp.upper_bound(L);
        // step back if previous interval still overlaps L
        if (it != mp.begin() && prev(it)->second > L) {
            --it;
        }
        return it;
    }

    // add interval [L,R)
    void add(int L, int R) {
        if (L >= R) return;

        auto it = lb(L);

        // merge all overlapping intervals
        while (it != mp.end() && it->first < R) {
            L = min(L, it->first);
            R = max(R, it->second);
            it = mp.erase(it);
        }
        mp[L] = R;  // insert merged interval
    }

    // remove interval [L,R)
    void remove(int L, int R) {
        if (L >= R) return;

        auto it = lb(L);

        while (it != mp.end() && it->first < R) {
            auto [a, b] = *it;
            it = mp.erase(it);

            // keep left split if exists
            if (a < L) mp[a] = L;
            // keep right split if exists
            if (b > R) mp[R] = b;
        }
    }

    // flip coverage in [L,R)
    void flip(int L, int R) {
        if (L >= R) return;

        auto it = lb(L); int cur = L;
        vector<pair<int, int>> addv;  // uncovered segments to become covered

        while (it != mp.end() && it->first < R) {
            auto [a, b] = *it;
            it = mp.erase(it);

            // restore split parts outside range
            if (a < L) mp[a] = L;
            if (b > R) mp[R] = b;

            // covered segment inside range becomes uncovered
            int s = max(a, L);
            if (cur < s) addv.push_back({cur, s});

            cur = min(b, R);
        }

        if (cur < R) addv.push_back({cur, R});
        for (auto& p : addv) mp[p.first] = p.second;
    }

    // point query
    bool contains(int x) {
        auto it = mp.upper_bound(x);
        return it != mp.begin() && prev(it)->second > x;
    }

    // any overlap with [L,R)
    bool any(int L, int R) {
        auto it = lb(L);
        return it != mp.end() && it->first < R;
    }

    // full cover check for [L,R)
    bool all(int L, int R) {
        auto it = mp.upper_bound(L);
        return it != mp.begin() &&
               prev(it)->first <= L &&
               prev(it)->second >= R;
    }

    // first covered position >= x
    int nextCovered(int x) {
        auto it = lb(x);
        if (it == mp.end()) return INT_MAX;
        return (it->first <= x ? x : it->first);
    }

    // first uncovered position >= x
    int nextUncovered(int x) {
        auto it = lb(x);
        if (it == mp.end() || it->first > x) return x;
        return it->second;
    }

    // total covered length
    int size() {
        int s = 0;
        for (auto& [l, r] : mp) s += r - l;
        return s;
    }

    // number of disjoint intervals
    int count() { return (int)mp.size(); }

    // get intervals
    vector<pair<int, int>> get() {
        return {mp.begin(), mp.end()};
    }
};

// NOTE:
// - All operations are HALF-OPEN [L,R)
// - For closed [L,R], convert to [L,R+1)
// - Structure always maintains MERGED disjoint intervals

void solve() {
    IntervalUnion u;

    u.add(1, 6);
    u.add(3, 9);

    u.remove(4, 7);

    u.flip(2, 9);

    cout << u.contains(3) << "\n";
    cout << u.any(3, 7) << "\n";
    cout << u.all(4, 7) << "\n";
    cout << u.nextCovered(5) << "\n";
    cout << u.nextUncovered(4) << "\n";
    cout << u.size() << "\n";
    cout << u.count() << "\n";

    for (auto [l, r] : u.get()) {
        cout << "[" << l << "," << r - 1 << "] ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int tc;
    cin >> tc;
    while (tc--) solve();
}
