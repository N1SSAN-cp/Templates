#include <bits/stdc++.h>
using namespace std;

// Erdos-Gallai Theorem — O(N)
bool check_degree_sequence(vector<int> deg) {
  int N = (int)deg.size();
  if (N == 0) {
    return true;
  }

  long long sm = 0;
  vector<int> freq(N, 0);
  for (int i = 0; i < N; i++) {
    int d = deg[i];
    if (d < 0 || d >= N) {
      return false;
    }
    freq[d]++;
    sm += d;
  }
  if (sm & 1) {
    return false;
  }

  // sort descending via freq
  int p = 0;
  for (int x = N - 1; x >= 0; x--) {
    for (int j = 0; j < freq[x]; j++) {
      deg[p++] = x;
    }
  }

  // prefix sum of sorted deg, size N+1
  vector<long long> S(N + 1, 0);
  for (int i = 0; i < N; i++) {
    S[i + 1] = S[i] + deg[i];
  }

  long long m = 0;
  for (int k = N; k >= 0; k--) {
    while (m < N && deg[m] >= k) {
      m++;
    }
    long long lhs = S[k];
    long long rhs = (long long)k * (k - 1);
    if (m < k) {
      rhs += S[N] - S[k];
    } else {
      rhs += (m - k) * k;
      rhs += S[N] - S[m];
    }
    if (lhs > rhs) {
      return false;
    }
  }
  return true;
}

// Erdos-Gallai construction — O(N + M)
pair<bool, vector<pair<int, int>>>
construct_from_degree_sequence(vector<int> deg) {
  if (!check_degree_sequence(deg)) {
    return {false, {}};
  }
  int N = (int)deg.size();

  // bucket[d] = list of vertices with degree d (replaces CSR)
  vector<vector<int>> bucket(N);
  for (int v = 0; v < N; v++) {
    bucket[deg[v]].push_back(v);
  }

  // flatten into parallel arrays D (degree) and V (vertex), sorted by degree
  // ascending
  vector<int> cnt(N, 0), D(N), V(N);
  int p = 0;
  for (int x = 0; x < N; x++) {
    for (int v : bucket[x]) {
      cnt[x]++;
      D[p] = x; V[p] = v;
      p++;
    }
  }
  assert(p == N);

  vector<pair<int, int>> ans;
  vector<pair<int, int>> tmp;

  for (int idx = N - 1; idx >= 0; idx--) {
    int v = V[idx], n = D[idx];
    cnt[D[idx]]--;
    D[idx] = 0; int cur = idx;
    while (n > 0) {
      int d = D[cur - 1];
      int l = cur - cnt[d];
      int m = min(n, cnt[d]);
      for (int i = l; i < l + m; i++) {
        ans.emplace_back(V[i], v);
        D[i]--;
      }
      tmp.emplace_back(d, m);
      n -= m;
      cur = l;
    }
    for (auto &[d, m] : tmp) {
      cnt[d] -= m;
      cnt[d - 1] += m;
    }
    tmp.clear();
  }
  return {true, ans};
}

// Prufer-sequence-based tree construction — O(N)
pair<bool, vector<pair<int, int>>>
construct_from_degree_sequence_tree(vector<int> deg) {
  int N = (int)deg.size(); long long sm = 0;
  for (int d : deg) {
    sm += d;
  }
  if (sm != 2LL * N - 2) {
    return {false, {}};
  }
  if (N == 1) {
    return {true, {}};
  }
  for (int d : deg) {
    if (d == 0) {
      return {false, {}};
    }
  }

  vector<pair<int, int>> E; vector<int> X, Y;
  for (int v = 0; v < N; v++) {
    (deg[v] == 1 ? X : Y).push_back(v);
  }

  for (int step = 0; step < N - 1; step++) {
    while (!Y.empty() && deg[Y.back()] <= 1) {
      X.push_back(Y.back()); Y.pop_back();
    }
    while (!X.empty() && deg[X.back()] != 1) {
      X.pop_back();
    }
    int v = X.back(); X.pop_back();
    while (!X.empty() && deg[X.back()] != 1) {
      X.pop_back();
    }
    if (step == N - 2) {
      E.emplace_back(v, X.back());
      X.pop_back();
    } else {
      int w = Y.back(); Y.pop_back();
      E.emplace_back(v, w);
      deg[w]--;
      Y.push_back(w);
    }
  }
  return {true, E};
}

int main() {
  // --- check only ---
  vector<int> deg = {3, 3, 2, 2, 1, 1};
  cout << check_degree_sequence(deg) << "\n"; // 1

  // --- construct graph ---
  auto [ok, edges] = construct_from_degree_sequence(deg);
  if (ok) {
    for (auto [u, v] : edges) {
      cout << u << " " << v << "\n";
    }
  }

  // --- construct tree ---
  vector<int> tdeg = {3, 2, 1, 1, 1}; // N=5, sum=8=2*5-2
  auto [tok, tedges] = construct_from_degree_sequence_tree(tdeg);
  if (tok) {
    for (auto [u, v] : tedges) {
      cout << u << " " << v << "\n";
    }
  }
  return 0;
}
