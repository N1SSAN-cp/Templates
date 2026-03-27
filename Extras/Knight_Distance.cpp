#include <bits/stdc++.h>
using namespace std;

//  Knight Distance | O(1)
// Returns the minimum number of knight moves to reach (x, y) from (0, 0)
int knight_distance(int x, int y) {
  x = abs(x);
  y = abs(y);

  if (x + y == 0) {
    return 0;
  }
  if (x + y == 1) {
    return 3;
  }
  if (x == 2 && y == 2) {
    return 4;
  }

  int step = (max(x, y) + 1) / 2;
  if ((x + y + 2) / 3 > step) {
    step = (x + y + 2) / 3;
  }
  step += (step ^ x ^ y) & 1;

  return step;
}

