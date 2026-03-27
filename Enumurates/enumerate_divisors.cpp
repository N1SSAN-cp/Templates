#include <bits/stdc++.h>
using namespace std;

// All divisors of N in sorted order
// Time: O(sqrt(N))  Space: O(sqrt(N)) for hi list
// Overflow: i*i overflows int for N > ~2e9, use long long cast
template <typename F>
void enumerate_divisors(int N, F f) {
    vector<int> hi;
    for (long long i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            f((int)i);
            if (i != N / i) hi.push_back(N / i);
        }
    }
    for (int i = hi.size() - 1; i >= 0; --i) f(hi[i]);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cout << "divisors of 36: ";
    enumerate_divisors(36, [](int d) { cout << d << " "; });
    cout << "\n";
}

// Output:
// divisors of 36: 1 2 3 4 6 9 12 18 36
