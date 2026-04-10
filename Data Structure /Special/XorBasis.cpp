#include <bits/stdc++.h>
using namespace std;

// XOR Basis — related to XOR of subsets
// TC: O(LOG_N) per insert

struct XorBasis {
    static const int LOG_N = 31;
    int basis[LOG_N], sz, cnt;
    XorBasis() {
        memset(basis, 0, sizeof(basis));
        sz = 0;
        cnt = 0;
    }

    // insert mask into basis using Gaussian elimination
    void insert(int mask) {
        cnt++;
        for (int i = LOG_N - 1; i >= 0; i--) {
            if (((mask >> i) & 1) == 0) {
                continue;
            }
            if (!basis[i]) {
              basis[i] = mask; sz++; return;
            }
            mask ^= basis[i];
        }
    }

    // maximum XOR achievable by any subset
    int max_xor() const {
        int ans = 0;
        for (int i = LOG_N - 1; i >= 0; i--) {
            if ((ans ^ basis[i]) > ans) {
                ans ^= basis[i];
            }
        }
        return ans;
    }

    // minimum non-zero XOR achievable by any subset
    int min_xor() const {
        for (int i = 0; i < LOG_N; i++) {
            if (basis[i]) {
                return basis[i];
            }
        }
        return 0;
    }

    // check if k is achievable as XOR of some subset
    bool can(int k) const {
        for (int i = LOG_N - 1; i >= 0; i--) {
            if (k & (1 << i)) {
                k ^= basis[i];
            }
        }
        return k == 0;
    }

    // number of distinct XOR values achievable
    long long count_Distinct_XOR_Sum() const { return 1LL << sz; }

    // number of subsets that produce each distinct XOR value
    long long count_Each_XOR_Sum() const { return 1LL << (cnt - sz); }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<int> arr(n);
    XorBasis xb;

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        xb.insert(arr[i]);
    }

    int q; cin >> q;
    while (q--) {
        int type; cin >> type;
        if (type == 1) {
            cout << xb.max_xor() << "\n";
        } else if (type == 2) {
            cout << xb.min_xor() << "\n";
        } else if (type == 3) {
            int k; cin >> k;
            cout << (xb.can(k) ? "YES" : "NO") << "\n";
        } else if (type == 4) {
            cout << xb.count_Distinct_XOR_Sum() << "\n";
        } else if (type == 5) {
            cout << xb.count_Each_XOR_Sum() << "\n";
        }
    }

    return 0;
}
