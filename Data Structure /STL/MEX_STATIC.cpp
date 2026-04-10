#include <bits/stdc++.h>
using namespace std;

const int maxN = 2e5;
struct ds {
    int sz;
    vector<int> vis;
    int mex;
    ds(int sz) {
        this->sz = sz;
        vis.resize(sz + 1, 0);
        mex = 0;
    }
    void insert(int x) {
        if (x > sz) return;
        vis[x]++;
        while (mex <= sz && vis[mex] > 0) {
            mex++;
        }
    }
    void remove(int x) {
        if (x > sz || vis[x] == 0) return;
        vis[x]--;
        if (vis[x] == 0 && x < mex) {
            mex = x;
        }
    }
    int getMex() { return mex; }
};

int main() { return 0; }
