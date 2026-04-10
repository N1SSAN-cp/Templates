#include <bits/stdc++.h>
using namespace std;

// Mode :  returns smallest mode
// insert/remove/mode: O(log n)
struct Mode {
    map<int, int> freq;
    multiset<pair<int, int>> freq_order;

    void insert(int x) {
        if (freq[x] > 0) {
            freq_order.erase(freq_order.find({-freq[x], x}));
        }
        freq[x]++;
        freq_order.insert({-freq[x], x});
    }

    void remove(int x) {
        if (freq[x] == 0) {
            return;
        }
        freq_order.erase(freq_order.find({-freq[x], x}));
        freq[x]--;
        if (freq[x] > 0) {
            freq_order.insert({-freq[x], x});
        }
    }

    // returns smallest element among those with highest frequency
    int mode() {
        if (freq_order.empty()) {
            return -1;
        }
        return freq_order.begin()->second;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Mode mt;

    mt.insert(3);
    mt.insert(1);
    mt.insert(3);
    mt.insert(2);
    mt.insert(2);
    mt.insert(2);

    cout << mt.mode() << "\n";  // 2  (freq: 1->1, 2->3, 3->2)

    mt.remove(2);
    mt.remove(2);
    mt.remove(2);

    cout << mt.mode() << "\n";  // 3  (freq: 1->1, 3->2)

    return 0;
}
