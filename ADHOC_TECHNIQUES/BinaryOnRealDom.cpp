#include <bits/stdc++.h>
using namespace std;

// Binary search on answer (real-valued), O(n * 100)
const int N = 1e5 + 5;

int main() {
    int n, m, a[N]; cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    auto ok = [&](long double x) {
        long long c = 0;
        for (int i = 0; i < n; ++i) {
            c += (long long)(a[i] / x);
        }
        return c >= m;
    };

    long double l = 0, r = 1;
    while (ok(r)) {
        r *= 2;
    }

    for (int i = 0; i < 100; ++i) {
        long double mid = (l + r) / 2;
        if (ok(mid)) {
            l = mid;
        } else {
            r = mid;
        }
    }

    cout << fixed << setprecision(10) << l << "\n";
    return 0;
}
