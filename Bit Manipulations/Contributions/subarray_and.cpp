#include <bits/stdc++.h>
using namespace std;

// Sum of AND of all subarrays
// Time: O(n * B)

using i64 = long long;
const int B = 30;

i64 subarrayAND(vector<int>& a) {
    int n = a.size();
    i64 ans = 0;
    for (int b = 0; b < B; b++) {
        i64 run = 0;
        for (int i = 0; i < n; i++) {
            if ((a[i] >> b) & 1) { run++; ans += (1LL << b) * run; }
            else { run = 0; }
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i]; }
    cout << subarrayAND(a) << "\n";
    return 0;
}
