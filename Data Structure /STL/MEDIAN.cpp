#include <bits/stdc++.h>
using namespace std;
struct MEDIAN {
    multiset<int> left, right;

    void balance() {
        while (left.size() > right.size() + 1) {
            right.insert(*prev(left.end()));
            left.erase(prev(left.end()));
        }
        while (right.size() > left.size()) {
            left.insert(*right.begin());
            right.erase(right.begin());
        }
    }

    void insert(int x) {
        if (left.empty() || x <= *left.rbegin()) {
            left.insert(x);
        } else {
            right.insert(x);
        }
        balance();
    }

    void remove(int x) {
        if (auto it = left.find(x); it != left.end()) {
            left.erase(it);
        } else if (auto it = right.find(x); it != right.end()) {
            right.erase(it);
        }
        balance();
    }

    int getMedian() { return *left.rbegin(); }

    void debug() {
        cerr << "Left: ";
        for (int x : left) cout << x << " ";
        cerr << "\nRight: ";
        for (int x : right) cout << x << " ";
        cerr << endl;
    }
};

int main() { return 0; }
