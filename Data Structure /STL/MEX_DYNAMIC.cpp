#include <bits/stdc++.h>
using namespace std;


//keep sizes slightly UP
struct MEX {
    vector<int> freq; set<int> np;
    MEX(int sz) {
        freq.assign(sz, 0);
        for (int i = 0; i < sz; i++) {
            np.insert(i);
        }
    }
    void insert(int x) {
        if (++freq[x] == 1) {
            np.erase(x);
        }
    }
    void remove(int x) {
        if (--freq[x] == 0) {
            np.insert(x);
        }
    }
    int getMex() { return *np.begin(); }
};

int main() {
    MEX mm(3);
    mm.insert(1);
    mm.insert(0);
    mm.insert(1);
    mm.remove(0);
    cout << mm.getMex() << endl;
}
