#include <bits/stdc++.h>
using namespace std;

// Sliding Window Block Decomposition (generalized)
// Time Complexity: O(n) build, O(1) per query


template <typename T>
struct SlidingWindowBlock {
    int n, k;
    vector<T> arr, prefix, suffix;
    function<T(T, T)> merge;

    SlidingWindowBlock(vector<T>& a, int k, function<T(T, T)> merge_fn)
        : n(a.size()), k(k), arr(a), prefix(a.size()), suffix(a.size()), merge(merge_fn) {
        build();
    }

    void build() {
        prefix[0] = arr[0];
        for (int i = 1; i < n; i++) {
            if (i % k == 0) {
                prefix[i] = arr[i];
            } else {
                prefix[i] = merge(prefix[i - 1], arr[i]);
            }
        }

        suffix[n - 1] = arr[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            if (i % k == k - 1) {
                suffix[i] = arr[i];
            } else {
                suffix[i] = merge(suffix[i + 1], arr[i]);
            }
        }
    }

    // Query window [l, r] where r - l + 1 == k
    T query(int l, int r) {
        if (r % k == k - 1) {
            return prefix[r];
        } else {
            return merge(prefix[r], suffix[l]);
        }
    }
};

void solve() {
    int n, k;
    cin >> n >> k;
    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    vector<long long> arr(n);
    arr[0] = x;
    for (int i = 1; i < n; i++) {
        arr[i] = (a * arr[i - 1] + b) % c;
    }

    SlidingWindowBlock<long long> swb(arr, k, [](long long p, long long q) {
        return p | q;  // merge: change as needed
    });

    long long ans = 0;
    for (int l = 0, r = k - 1; r < n; l++, r++) {
        ans ^= swb.query(l, r);  // aggregation: change as needed
    }

    cout << ans << endl;
}

int main() {
    solve();
    return 0;
}

/*
 * SLIDING WINDOW SUM WITH BLOCK DECOMPOSITION
 *
 * Algorithm: Fixed-size block decomposition with prefix/suffix arrays
 * Time Complexity: O(n)
 * Space Complexity: O(n)

 * Finding xor sum of all k sized-window or
 * Changes from original template:
 * 1) merge
 * 2) final solution


 * To adapt for different operations:
 * Change merge() function to:
 * - Bitwise OR: return a | b;
 * - Bitwise AND: return a & b;
 * - GCD: return __gcd(a, b);
 * - Max: return max(a, b);
 * - Min: return min(a, b);
 * - Sum: return a + b;

 
 * And change final aggregation:
 * - XOR all results: ans ^= window_result;
 * - Sum all results: ans += window_result;
 * - Max of all: ans = max(ans, window_result);
 * - Min of all: ans = min(ans, window_result);
 */
