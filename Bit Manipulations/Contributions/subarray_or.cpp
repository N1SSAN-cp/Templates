#include <bits/stdc++.h>
using namespace std;

// Sum of OR of all subarrays
// Time: O(n * B)

using i64 = long long;
const int B = 30;

i64 subarrayOR(vector<int> &a) {
  int n = a.size();
  i64 total = (i64)n * (n + 1) / 2;
  i64 ans = 0;
  for (int b = 0; b < B; b++) {
    i64 no = 0, run = 0;
    for (int i = 0; i < n; i++) {
      if ((a[i] >> b) & 1) {
        no += run * (run + 1) / 2;
        run = 0;
      } else {
        run++;
      }
    }
    no += run * (run + 1) / 2;
    ans += (1LL << b) * (total - no);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  cout << subarrayOR(a) << "\n";
  return 0;
}
