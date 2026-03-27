#include <bits/stdc++.h>
using namespace std;

// Digit DP — O(log N * K)
template <typename T = long long, int K = 10>
array<T, K> digit_frequency_vector(vector<int> A, bool include_N) {
    assert(A.size() > 0 && A[0] != '0');
    int n = (int)A.size();
    vector<T> pw(n + 1, T(1));
    for (int i = 0; i < n; i++) {
        pw[i + 1] = pw[i] * K;
    }
    using ARR = array<T, K>;
    ARR dp{}, same{};
    same[A[0]] = 1;
    for (int i = 1; i < A[0]; i++) {
        dp[i] = 1;
    }
    T lo_cnt = 0;
    for (int k = 0; k < K; k++) {
        lo_cnt += dp[k];
    }
    for (int i = 1; i < (int)A.size(); i++) {
        int a = A[i];
        ARR newdp{};
        for (int k = 0; k < K; k++) {
            newdp[k] = dp[k] * K + lo_cnt + same[k] * a + (k < a) + (k > 0);
        }
        lo_cnt = lo_cnt * K + a + (K - 1);
        swap(dp, newdp);
        same[a] += 1;
    }
    if (include_N) {
        for (int k = 0; k < K; k++) {
            dp[k] += same[k];
        }
    }
    return dp;
}

// Count of each digit's occurrences across all numbers in [L, R)
template <typename T = long long, int K = 10>
array<T, K> digit_frequency(uint64_t L, uint64_t R) {
    auto F = [&](uint64_t N) -> array<T, K> {
        vector<int> A;
        while (N) {
            A.push_back(N % K);
            N /= K;
        }
        reverse(A.begin(), A.end());
        return digit_frequency_vector<T, K>(A, false);
    };
    array<T, K> A = F(R);
    array<T, K> B = F(L);
    for (int k = 0; k < K; k++) {
        A[k] -= B[k];
    }
    return A;
}

int main() {
    // Example 1: digit frequency in [1, 100)
    {
        auto freq = digit_frequency(1, 100);
        cout << "Digit frequencies in [1, 100):\n";
        for (int k = 0; k < 10; k++) {
            cout << "  digit " << k << " -> " << freq[k] << "\n";
        }
        cout << "\n";
    }

    // Example 2: inclusive range [1, 20] -> use R = 21
    {
        auto freq = digit_frequency(1, 21);
        cout << "Digit frequencies in [1, 20] (inclusive):\n";
        cout << "  digit 1 -> " << freq[1] << "\n"; // 1,10,11,12,13,14,15,16,17,18,19 = 12
        cout << "\n";
    }

    // Example 3: [100, 200)
    {
        auto freq = digit_frequency(100, 200);
        cout << "Digit frequencies in [100, 200):\n";
        cout << "  digit 0 -> " << freq[0] << "\n"; // 100,101,...109 = 10 zeros
        cout << "\n";
    }

    // Example 4: single number 121 — how many times does each digit appear?
    // [121, 122) gives frequency only for the number 121
    {
        auto freq = digit_frequency(121, 122);
        cout << "Digit frequencies in just the number 121:\n";
        for (int k = 0; k < 10; k++) {
            if (freq[k]) {
                cout << "  digit " << k << " -> " << freq[k] << "\n";
            }
        }
    }

    return 0;
}
