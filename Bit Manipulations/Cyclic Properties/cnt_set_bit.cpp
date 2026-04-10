#include <bits/stdc++.h>
using namespace std;

auto count_set_bit = [](int L, int R, int i) {
  auto f = [&](int n) {
    int block = 1ll << (i + 1);
    int half = 1ll << i;
    int full = (n + 1) / block;
    int rem = (n + 1) % block;
    return full * half + max(0, rem - half);
  };
  return f(R) - f(L - 1);
};
