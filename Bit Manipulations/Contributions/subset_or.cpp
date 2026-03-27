#include <bits/stdc++.h>
using namespace std;

// Sum of OR of all non-empty subsets
// Time: O(n * B)

using i64 = long long;
const int B = 30;

i64 subsetOR(vector<int> &a) {
  int n = a.size();
  i64 ans = 0;
  for (int b = 0; b < B; b++) {
    i64 c0 = 0;
    for (int i = 0; i < n; i++) {
      if (!((a[i] >> b) & 1)) {
        c0++;
      }
    }
    ans += (1LL << b) * ((1LL << n) - (1LL << c0));
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
  cout << subsetOR(a) << "\n";
  return 0;
}
