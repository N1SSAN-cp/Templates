#include <bits/stdc++.h>
using namespace std;

// argsort: returns indices sorted by value ascending
// Time: O(n log n)
vector<int> argsort(vector<int> &a) {
  int N = (int)a.size();
  vector<int> idx(N);
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(), [&](int i, int j) { return a[i] < a[j]; });
  return idx;
}

// SCC decomposition of tournament via indegree sort
// comp[v] = SCC id in topological order (0 = source SCC)
// comp[u] < comp[v] => u is definitely ranked above v
// Time: O(n log n)
pair<int, vector<int>> scc_tournament(vector<int> &indeg) {
  int N = (int)indeg.size();
  vector<int> I = argsort(indeg);
  vector<int> ANS(N);
  long long sm = 0;
  int nxt = 0;
  for (int i = 0; i < N; i++) {
    int v = I[i];
    ANS[v] = nxt;
    sm += indeg[v];
    long long TS = sm - (((long long)(i + 1) * i) / 2);
    if (TS == 0) {
      ++nxt;
    }
  }
  return {nxt, ANS};
}

// ─────────────────────────────────────────────────────────────
// PROBLEM 1: Round-robin ranking ambiguity
// n players, each pair plays exactly once.
// Query: is u definitely above v, v above u, or ambiguous?
// Input: n M (match count), M lines "u v" (u beat v), Q queries
// Output per query: who is above or "ambiguous"
// ─────────────────────────────────────────────────────────────
/*
Sample input:
4 6
3 0
3 1
3 2
0 1
1 2
2 0
3
0 1
3 0
0 2

Sample output:
ambiguous
3 is definitely above 0
ambiguous
*/
void problem1() {
  int N, M;
  cin >> N >> M;
  vector<int> indeg(N, 0);
  for (int i = 0; i < M; i++) {
    int u, v;
    cin >> u >> v;
    indeg[v]++;
  }
  auto [num_scc, comp] = scc_tournament(indeg);
  int Q;
  cin >> Q;
  while (Q--) {
    int u, v;
    cin >> u >> v;
    if (comp[u] < comp[v]) {
      cout << u << " is definitely above " << v << "\n";
    } else if (comp[u] > comp[v]) {
      cout << v << " is definitely above " << u << "\n";
    } else {
      cout << "ambiguous\n";
    }
  }
}

// ─────────────────────────────────────────────────────────────
// PROBLEM 2: Count decisive players
// A player is decisive if their rank is uniquely determined
// (their SCC has size 1).
// Input: n, n*(n-1)/2 lines "u v" (u beat v)
// Output: decisive count, per-player rank info
// ─────────────────────────────────────────────────────────────
/*
Sample input:
4
3 0
3 1
3 2
0 1
1 2
2 0

Sample output:
Decisive players: 1
Player 0 -> SCC 1 (ambiguous, size=3)
Player 1 -> SCC 1 (ambiguous, size=3)
Player 2 -> SCC 1 (ambiguous, size=3)
Player 3 -> rank 0 (decisive)
*/
void problem2() {
  int N;
  cin >> N;
  vector<int> indeg(N, 0);
  int total = N * (N - 1) / 2;
  for (int i = 0; i < total; i++) {
    int u, v;
    cin >> u >> v;
    indeg[v]++;
  }
  auto [num_scc, comp] = scc_tournament(indeg);
  vector<int> scc_size(num_scc, 0);
  for (int v = 0; v < N; v++) {
    scc_size[comp[v]]++;
  }
  int decisive = count(scc_size.begin(), scc_size.end(), 1);
  cout << "Decisive players: " << decisive << "\n";
  for (int v = 0; v < N; v++) {
    if (scc_size[comp[v]] == 1) {
      cout << "Player " << v << " -> rank " << comp[v] << " (decisive)\n";
    } else {
      cout << "Player " << v << " -> SCC " << comp[v]
           << " (ambiguous, size=" << scc_size[comp[v]] << ")\n";
    }
  }
}

// ─────────────────────────────────────────────────────────────
// PROBLEM 3: Is tournament transitive?
// Transitive = all indegrees distinct = num_scc == n
// Input: n, indegree sequence
// Output: "Transitive" + order, or "Not transitive" + tiers
// ─────────────────────────────────────────────────────────────
/*
Sample input:
4
0 1 2 3

Sample output:
Transitive - total order exists
0 -> 1 -> 2 -> 3
*/
void problem3() {
  int N;
  cin >> N;
  vector<int> indeg(N);
  for (int i = 0; i < N; i++) {
    cin >> indeg[i];
  }
  auto [num_scc, comp] = scc_tournament(indeg);
  if (num_scc == N) {
    cout << "Transitive - total order exists\n";
    vector<int> order(N);
    for (int v = 0; v < N; v++) {
      order[comp[v]] = v;
    }
    for (int i = 0; i < N; i++) {
      cout << order[i];
      if (i + 1 < N) {
        cout << " -> ";
      }
    }
    cout << "\n";
  } else {
    cout << "Not transitive - " << num_scc << " SCCs\n";
    vector<vector<int>> tiers(num_scc);
    for (int v = 0; v < N; v++) {
      tiers[comp[v]].push_back(v);
    }
    for (int i = 0; i < num_scc; i++) {
      cout << "Tier " << i << ": ";
      for (int v : tiers[i]) {
        cout << v << " ";
      }
      cout << "\n";
    }
  }
}

// ─────────────────────────────────────────────────────────────
// PROBLEM 4: Min and max possible rank of a player
// Within its SCC a player can be ranked anywhere.
// min rank = comp[X], max rank = comp[X] + scc_size - 1
// Input: n, indegree sequence, player X
// Output: guaranteed/best/worst rank
// ─────────────────────────────────────────────────────────────
/*
Sample input:
5
1 2 2 2 3
2

Sample output:
Player 2:
  SCC id (guaranteed rank): 1
  Best possible rank: 1
  Worst possible rank: 3
*/
void problem4() {
  int N;
  cin >> N;
  vector<int> indeg(N);
  for (int i = 0; i < N; i++) {
    cin >> indeg[i];
  }
  int X;
  cin >> X;
  auto [num_scc, comp] = scc_tournament(indeg);
  vector<int> scc_size(num_scc, 0);
  for (int v = 0; v < N; v++) {
    scc_size[comp[v]]++;
  }
  cout << "Player " << X << ":\n";
  cout << "  SCC id (guaranteed rank): " << comp[X] << "\n";
  cout << "  Best possible rank: " << comp[X] << "\n";
  cout << "  Worst possible rank: " << comp[X] + scc_size[comp[X]] - 1 << "\n";
}

// ─────────────────────────────────────────────────────────────
// PROBLEM 5: Does a definite champion exist?
// A definite champion exists iff SCC0 has size 1.
// Input: n, indegree sequence
// Output: champion node, or "no definite champion"
// ─────────────────────────────────────────────────────────────
/*
Sample input:
4
0 2 2 2

Sample output:
No definite champion - top SCC has 3 players
*/
void problem5() {
  int N;
  cin >> N;
  vector<int> indeg(N);
  for (int i = 0; i < N; i++) {
    cin >> indeg[i];
  }
  auto [num_scc, comp] = scc_tournament(indeg);
  vector<int> scc_size(num_scc, 0);
  for (int v = 0; v < N; v++) {
    scc_size[comp[v]]++;
  }
  if (scc_size[0] == 1) {
    for (int v = 0; v < N; v++) {
      if (comp[v] == 0) {
        cout << "Definite champion: player " << v << "\n";
      }
    }
  } else {
    cout << "No definite champion - top SCC has " << scc_size[0]
         << " players\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  // uncomment the problem you want to run:
  // problem1();
  // problem2();
  // problem3();
  // problem4();
  problem5();

  return 0;
}
