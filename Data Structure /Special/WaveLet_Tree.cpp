#include <bits/stdc++.h>
using namespace std;

// Wavelet Tree
// Build: O(n log n), Query: O(log n)

struct wavelet_tree {
  int lo, hi;
  wavelet_tree *l, *r;
  int *b, *c, bsz, csz;

  wavelet_tree() {
    lo = 1, hi = 0, bsz = 0, csz = 0;
    l = nullptr, r = nullptr;
    b = nullptr, c = nullptr;
  }

  void init(int *from, int *to, int x, int y) {
    lo = x;
    hi = y;
    if (from >= to) {
      return;
    }
    int mid = (lo + hi) >> 1;
    auto f = [mid](int v) { return v <= mid; };
    int sz = (int)(to - from) + 2;
    b = new int[sz];
    c = new int[sz];
    bsz = csz = 0;
    b[bsz++] = 0;
    c[csz++] = 0;
    for (auto it = from; it != to; it++) {
      b[bsz] = b[bsz - 1] + f(*it);
      c[csz] = c[csz - 1] + (*it);
      bsz++;
      csz++;
    }
    if (hi == lo) {
      return;
    }
    auto pivot = stable_partition(from, to, f);
    l = new wavelet_tree();
    l->init(from, pivot, lo, mid);
    r = new wavelet_tree();
    r->init(pivot, to, mid + 1, hi);
  }

  void init(vector<int> &a, int x, int y) {
    init(a.data(), a.data() + a.size(), x, y);
  }

  // kth smallest in [l, r], 0-indexed range, 1-indexed k
  int kth(int l, int r, int k) {
    if (l > r) {
      return 0;
    }
    if (lo == hi) {
      return lo;
    }
    int lb = b[l], rb = b[r + 1];
    int inLeft = rb - lb;
    if (k <= inLeft) {
      return this->l->kth(lb, rb - 1, k);
    }
    return this->r->kth(l - lb, r - rb, k - inLeft);
  }

  // lower median of [l, r], 0-indexed range
  int median_low(int l, int r) { return kth(l, r, (r - l + 2) / 2); }

  // upper median of [l, r], 0-indexed range
  int median_high(int l, int r) { return kth(l, r, (r - l + 3) / 2); }

  // count of elements <= k in [l, r], 0-indexed range
  int LTE(int l, int r, int k) {
    if (l > r || k < lo) {
      return 0;
    }
    if (hi <= k) {
      return r - l + 1;
    }
    int lb = b[l], rb = b[r + 1];
    return this->l->LTE(lb, rb - 1, k) + this->r->LTE(l - lb, r - rb, k);
  }

  // count of elements == k in [l, r], 0-indexed range
  int count(int l, int r, int k) {
    if (l > r || k < lo || k > hi) {
      return 0;
    }
    if (lo == hi) {
      return r - l + 1;
    }
    int lb = b[l], rb = b[r + 1];
    int mid = (lo + hi) >> 1;
    if (k <= mid) {
      return this->l->count(lb, rb - 1, k);
    }
    return this->r->count(l - lb, r - rb, k);
  }

  // sum of elements <= k in [l, r], 0-indexed range
  int sum(int l, int r, int k) {
    if (l > r || k < lo) {
      return 0;
    }
    if (hi <= k) {
      return c[r + 1] - c[l];
    }
    int lb = b[l], rb = b[r + 1];
    return this->l->sum(lb, rb - 1, k) + this->r->sum(l - lb, r - rb, k);
  }

  ~wavelet_tree() {
    delete[] b;
    delete[] c;
    delete l;
    delete r;
  }
};

int main() {
  // a = [3, 1, 4, 1, 5, 9, 2, 6]
  // index:  0  1  2  3  4  5  6  7
  vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};
  int lo = *min_element(a.begin(), a.end()); // 1
  int hi = *max_element(a.begin(), a.end()); // 9

  wavelet_tree wt;
  wt.init(a, lo, hi);

  // kth: 3rd smallest in a[1..5] = {1,4,1,5,9} -> sorted {1,1,4,5,9} -> 4
  cout << "kth(1,5,3)       = " << wt.kth(1, 5, 3) << "\n"; // 4

  // median_low: lower median of a[0..7] = {1,1,2,3,4,5,6,9} -> 3
  cout << "median_low(0,7)  = " << wt.median_low(0, 7) << "\n"; // 3

  // median_high: upper median of a[0..7] = {1,1,2,3,4,5,6,9} -> 4
  cout << "median_high(0,7) = " << wt.median_high(0, 7) << "\n"; // 4

  // LTE: count elements <= 4 in a[0..4] = {3,1,4,1,5} -> 4 (3,1,4,1)
  cout << "LTE(0,4,4)       = " << wt.LTE(0, 4, 4) << "\n"; // 4

  // count: count of 1s in a[0..4] = {3,1,4,1,5} -> 2
  cout << "count(0,4,1)     = " << wt.count(0, 4, 1) << "\n"; // 2

  // sum: sum of elements <= 4 in a[0..4] = {3,1,4,1,5} -> 3+1+4+1 = 9
  cout << "sum(0,4,4)       = " << wt.sum(0, 4, 4) << "\n"; // 9

  return 0;
}
