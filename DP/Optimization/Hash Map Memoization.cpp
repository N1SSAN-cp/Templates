#include <bits/stdc++.h>
using namespace std;

/*
 * Time complexity of DP:
 *   = (#distinct states visited) * (cost of one transition)
 *
 * When to use map as memo table:
 *   - State value range is too large to allocate an array (e.g. n <= 1e18)
 *   - BUT the number of distinct states actually visited by recursion is small
 *   - map stores only visited states -> memory is O(#visited states)
 *
 * Key insight:
 *   n <= 1e18 is the VALUE RANGE, not the STATE COUNT.
 *   Transitions that divide/factorize collapse states fast -> few states
 * visited. Transitions that go +1/-1 sequentially -> O(n) states -> map won't
 * help.
 *
 * 1D map  : one state variable is large/sparse
 * 2D map  : two state variables are large/sparse (use map<pair<int, long long>, int>)
 */

// ---------------------------------------------------------------------
// Problem: Minimum steps to reduce n to 1
//   Each step: n-1, or n/2 (if even), or n/3 (if divisible by 3)
//   n <= 1e18
//
// #distinct states visited ~ O(log^2 n) -> few hundred for n = 1e18
// array[1e18] impossible; map stores only visited states
//
// Time:  O(log^2 n * log(log^2 n))  [map lookup per state]
// Space: O(log^2 n)
// ---------------------------------------------------------------------
unordered_map<int, int> memo;

int solve(int n) {
  // basecase
  if (n == 1) {
    return 0;
  }
  // cache checking
  if (memo.count(n)) {
    return memo[n];
  }
  // compute
  int res = solve(n - 1) + 1;
  // transition
  if (n % 2 == 0) {
    res = min(res, solve(n / 2) + 1);
  }
  if (n % 3 == 0) {
    res = min(res, solve(n / 3) + 1);
  }
  // save and return
  return memo[n] = res;
}

int main() {
  int n; cin >> n;
  cout << solve(n) << "\n";
}
