#include <iostream>
using namespace std;

#define ll long long
#define all(x) x.begin(), x.end()

struct XORTrie {
  static const int B = 63;

  struct node {
    node *nxt[2];
    ll sz;
    node() {
      nxt[0] = nxt[1] = nullptr;
      sz = 0;
    }
  } *root;

  XORTrie() { root = new node(); }

  void insert(ll val) {
    node *cur = root;
    cur->sz++;
    for (int i = B - 1; i >= 0; i--) {
      int b = (val >> i) & 1;
      if (!cur->nxt[b])
        cur->nxt[b] = new node();
      cur = cur->nxt[b];
      cur->sz++;
    }
  }

  void erase(ll val) {
    node *cur = root;
    cur->sz--;
    for (int i = B - 1; i >= 0; i--) {
      int b = (val >> i) & 1;
      cur = cur->nxt[b];
      cur->sz--;
    }
  }

  ll max_xor(ll x) {
    if (root->sz == 0)
      return 0;

    node *cur = root;
    ll ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (!cur)
        break;

      int b = (x >> i) & 1;
      if (cur->nxt[!b] && cur->nxt[!b]->sz > 0) {
        ans = (ans << 1) | 1;
        cur = cur->nxt[!b];
      } else if (cur->nxt[b] && cur->nxt[b]->sz > 0) {
        ans = ans << 1;
        cur = cur->nxt[b];
      } else {
        break;
      }
    }
    return ans;
  }

  ll min_xor(ll x) {
    if (root->sz == 0)
      return 0;

    node *cur = root;
    ll ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (!cur)
        break;

      int b = (x >> i) & 1;
      if (cur->nxt[b] && cur->nxt[b]->sz > 0) {
        ans = ans << 1;
        cur = cur->nxt[b];
      } else if (cur->nxt[!b] && cur->nxt[!b]->sz > 0) {
        ans = (ans << 1) | 1;
        cur = cur->nxt[!b];
      } else {
        break;
      }
    }
    return ans;
  }

  ll count_less_than(ll x, ll k) {
    if (root->sz == 0)
      return 0;

    node *cur = root;
    ll ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (!cur)
        break;

      int b1 = (x >> i) & 1;
      int b2 = (k >> i) & 1;

      if (b2 == 1) {
        if (cur->nxt[b1])
          ans += cur->nxt[b1]->sz;
        if (cur->nxt[!b1]) {
          cur = cur->nxt[!b1];
        } else {
          break;
        }
      } else {
        if (cur->nxt[b1]) {
          cur = cur->nxt[b1];
        } else {
          break;
        }
      }
    }
    return ans;
  }

  ll kth_xor(ll x, ll k) {
    if (root->sz == 0 || k <= 0 || k > root->sz)
      return -1;

    node *cur = root;
    ll ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (!cur)
        break;

      int b = (x >> i) & 1;
      ll left_sz = (cur->nxt[b] && cur->nxt[b]->sz > 0) ? cur->nxt[b]->sz : 0;

      if (k <= left_sz && cur->nxt[b]) {
        ans = ans << 1;
        cur = cur->nxt[b];
      } else if (cur->nxt[!b]) {
        k -= left_sz;
        ans = (ans << 1) | 1;
        cur = cur->nxt[!b];
      } else {
        break;
      }
    }
    return ans;
  }
};

void solve() {
  XORTrie ds;
  ll n;
  cin >> n;

  for (int j = 0; j < n; j++) {
    char ch;
    ll num;
    cin >> ch >> num;

    if (ch == '+') {
      ds.insert(num);
    } else if (ch == '-') {
      ds.erase(num);
    } else if (ch == '^') {
      cout << ds.max_xor(num) << '\n';
    } else if (ch == '?') {
      cout << ds.min_xor(num) << '\n';
    }
  }
}

signed main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int tc = 1;

  for (int i = 0; i < tc; i++) {
    solve();
  }

  return 0;
}
