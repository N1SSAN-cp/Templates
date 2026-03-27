#include <bits/stdc++.h>
using namespace std;

// Greedy coin counterexample finder
// O(N^3)
// https://codeforces.com/problemset/problem/10/E
// Theory: https://graal.ens-lyon.fr/~abenoit/algo09/coins2.pdf
int greedy_coin_counter_example(vector<int> A) {
  sort(A.begin(), A.end());
  A.erase(unique(A.begin(), A.end()), A.end());
  reverse(A.begin(), A.end()); // descending order
  int N = (int)A.size();
  assert(A.back() == 1);
  assert(A.back() < (1 << 30));

  auto greedy_cost = [&](int x) -> int {
    int coins = 0;
    for (int i = 0; i < N; i++) {
      coins += x / A[i];
      x %= A[i];
    }
    return coins;
  };

  int ans = -1;
  vector<int> S(N);

  // Enumerate all (i, j) pairs to generate counterexample candidates.
  for (int i = 1; i < N; i++) {
    // Decompose c[i-1]-1 greedily using only coins c[i..N-1].
    int x = A[i - 1] - 1;
    for (int j = i; j < N; j++) {
      S[j] = x / A[j];
      x %= A[j];
    }
    // Accumulate prefix: sm = value, M = coin count for coins c[i..j].
    int sm = 0, M = 0;
    for (int j = i; j < N; j++) {
      sm += A[j] * S[j];
      M += S[j];
      // Candidate w = sm + A[j].
      // Optimal cost  = M + 1  (current coins plus one more c[j]).
      // If greedy does worse, w is a counterexample.
      int w = sm + A[j];
      if (M + 1 < greedy_cost(w)) {
        ans = (ans == -1 ? w : min(ans, w));
      }
    }
  }
  return ans;
}

int main() {
  // Standard coins {1,5,10,25}: greedy is always optimal -> -1
  {
    vector<int> coins = {1, 5, 10, 25};
    int res = greedy_coin_counter_example(coins);
    cout << "Coins {1,5,10,25}: " << res << "\n"; // Expected: -1
  }

  // Coins {1,3,4}: greedy picks 4+1+1=3 coins for 6,
  // but optimal is 3+3=2 coins -> counterexample at 6
  {
    vector<int> coins = {1, 3, 4};
    int res = greedy_coin_counter_example(coins);
    cout << "Coins {1,3,4}: " << res << "\n"; // Expected: 6
  }

  // Coins {1,6,10}: greedy picks 10+1+1+1=4 coins for 12,
  // but optimal is 6+6=2 coins -> counterexample at 12
  {
    vector<int> coins = {1, 6, 10};
    int res = greedy_coin_counter_example(coins);
    cout << "Coins {1,6,10}: " << res << "\n"; // Expected: 12
  }

  return 0;
}
