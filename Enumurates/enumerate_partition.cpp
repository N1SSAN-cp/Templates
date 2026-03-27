#include <bits/stdc++.h>
using namespace std;

/*
  Enumerates integer partitions of N in lexicographically descending order.
  Time: O(p(N)) where p(N) is the partition count  Space: O(N) recursion depth
  Overflow: none — all values <= N <= INT_MAX
  N=10,20,30,40: 42, 527, 5604, 37338
  N=50 (204226)  : 12 ms
  N=60 (966467)  : 60 ms
  N=70 (4087968) : 270 ms
  N=80 (15796476): 1100 ms
*/
template <typename F>
void enumerate_partition(int N, F query, int LIM_len = -1, int LIM_val = -1) {
    assert(N >= 0);
    auto dfs = [&](auto& dfs, vector<int>& p, int sum) -> void {
        if (sum == N) { query(p); return; }
        if (LIM_len != -1 && (int)p.size() == LIM_len) return;
        int nxt = (p.empty() ? N : p.back());
        if (LIM_val != -1) nxt = min(nxt, LIM_val);
        nxt = min(nxt, N - sum);
        p.push_back(0);
        for (int x = nxt; x >= 1; --x) {
            p.back() = x;
            dfs(dfs, p, sum + x);
        }
        p.pop_back();
    };
    vector<int> p;
    dfs(dfs, p, 0);
}

// Enumerates all ordered ways to write N as sum of K non-negative integers
// Time: O(C(N+K-1, K-1))  Space: O(K)
// Overflow: none — all values <= N
template <typename F>
void enumerate_compositions(int N, int K, F query) {
    vector<int> A(K);
    auto dfs = [&](auto& dfs, int p, int s) -> void {
        if (p == K) {
            if (s == 0) query(A);
            return;
        }
        for (int x = 0; x <= s; ++x) {
            A[p] = x;
            dfs(dfs, p + 1, s - x);
        }
    };
    dfs(dfs, 0, N);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // integer partitions of 6
    cout << "partitions of 6:\n";
    enumerate_partition(6, [](vector<int>& p) {
        for (int x : p) cout << x << " ";
        cout << "\n";
    });

    // partitions of 6 into at most 2 parts
    cout << "\npartitions of 6 (at most 2 parts):\n";
    enumerate_partition(6, [](vector<int>& p) {
        for (int x : p) cout << x << " ";
        cout << "\n";
    }, 2);
    // Output: 6 / 5 1 / 4 2 / 3 3

    // ordered compositions of 4 into 3 parts
    cout << "\ncompositions of 4 into 3 parts:\n";
    enumerate_compositions(4, 3, [](vector<int>& A) {
        for (int x : A) cout << x << " ";
        cout << "\n";
    });
}
