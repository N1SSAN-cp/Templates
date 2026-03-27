#include <bits/stdc++.h>
using namespace std;

// All size-K subsets of [0, N) in lexicographic order
// Time: O(C(N,K) * K)  Space: O(K)
template <typename F>
void enumerate_combinations(int N, int K, F f) {
    if (K > N || K < 0) return;  // guard: invalid input
    vector<int> p(K);
    iota(p.begin(), p.end(), 0);
    while (true) {
        f(p);
        int i = K - 1;
        while (i >= 0 && p[i] == N - K + i) --i;
        if (i < 0) break;
        ++p[i];
        for (int j = i + 1; j < K; ++j) p[j] = p[j - 1] + 1;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // C(5,3): all 3-element subsets of {0,1,2,3,4}
    enumerate_combinations(5, 3, [](vector<int>& p) {
        for (int x : p) cout << x << " ";
        cout << "\n";
    });
}

// Output:
// 0 1 2
// 0 1 3
// 0 1 4
// 0 2 3
// 0 2 4
// 0 3 4
// 1 2 3
// 1 2 4
// 1 3 4
// 2 3 4
