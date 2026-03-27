#include <bits/stdc++.h>
using namespace std;

// Sum of XOR of all subarrays
// Time: O(n * BITS)

using i64 = long long;
const int B = 30;

i64 subarrayXOR(vector<int> &a) {
  int n = a.size();
  i64 ans = 0;
  for (int b = 0; b < B; b++) {
    int cnt0 = 1, cnt1 = 0, pre = 0;
    for (int i = 0; i < n; i++) {
      pre ^= a[i];
      if ((pre >> b) & 1) {
        cnt1++;
      } else {
        cnt0++;
      }
    }
    ans += (1LL << b) * cnt0 * cnt1;
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
  cout << subarrayXOR(a) << "\n";
  return 0;
}
