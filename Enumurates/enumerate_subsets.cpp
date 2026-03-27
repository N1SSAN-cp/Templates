#include <bits/stdc++.h>
using namespace std;

// All subsets of s via (t-1) & s trick, optionally including empty set
// Time: O(2^popcount(s))  Space: O(1)
// Overflow: safe as long as s fits in int (up to bit 30)
template <bool inc_empty, typename F>
inline void enumerate_all_subset(int s, F&& f) {
    for (int t = s; t; t = (t - 1) & s) f(t);
    if constexpr (inc_empty) f(0);
}

// All subsets of s with exactly k bits set
// Time: O(C(popcount(s), k) * k)  Space: O(popcount(s))
// Overflow: 1 << bits[i] safe if bits[i] <= 30 (int), use long long for bit 31+
template <typename F>
void enumerate_subsets_by_size(int s, int k, F f) {
    int total = __builtin_popcount(s);
    if (k > total || k < 0) return;  // guard
    vector<int> bits;
    for (int i = 0; i < 30; ++i) if (s >> i & 1) bits.push_back(i);
    vector<int> idx(k);
    iota(idx.begin(), idx.end(), 0);
    while (true) {
        int t = 0;
        for (int i : idx) t |= 1 << bits[i];
        f(t);
        int i = k - 1;
        while (i >= 0 && idx[i] == total - k + i) --i;
        if (i < 0) break;
        ++idx[i];
        for (int j = i + 1; j < k; ++j) idx[j] = idx[j - 1] + 1;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // all subsets of 0b1011 including empty
    cout << "subsets of 0b1011:\n";
    enumerate_all_subset<true>(0b1011, [](int t) {
        cout << bitset<4>(t) << "\n";
    });
    // Output: 1011 1010 1001 1000 0011 0010 0001 0000

    // subsets of 0b1111 with exactly 2 bits set
    cout << "\nsubsets of 0b1111 with 2 bits:\n";
    enumerate_subsets_by_size(0b1111, 2, [](int t) {
        cout << bitset<4>(t) << "\n";
    });
    // Output: 0011 0101 0110 1001 1010 1100
}
