#include <bits/stdc++.h>
using namespace std;

// Enumerates all tuples (x0, x1, ...) where 0 <= xi < A[i]
// Time: O(A[0] * A[1] * ... * A[K-1])  Space: O(K) recursion depth
// Overflow: none — all values stay within A[i] bounds
template <typename F>
void enumerate_product(vector<int> A, F query) {
    int N = A.size();
    auto dfs = [&](auto& dfs, vector<int>& p) -> void {
        int n = p.size();
        if (n == N) return query(p);
        for (int x = 0; x < A[n]; ++x) {
            p.push_back(x);
            dfs(dfs, p);
            p.pop_back();
        }
    };
    vector<int> p;
    dfs(dfs, p);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // All tuples in [0,2) x [0,3)
    enumerate_product({2, 3}, [](vector<int>& p) {
        for (int x : p) cout << x << " ";
        cout << "\n";
    });
}

// Output:
// 0 0
// 0 1
// 0 2
// 1 0
// 1 1
// 1 2
