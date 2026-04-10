#include <bits/stdc++.h>
using namespace std;

struct monotone_deque {
    deque<int> dq;
    void insert(int x) {
        while (!dq.empty() && dq.back() > x) dq.pop_back();
        dq.push_back(x);
    }
    void erase(int x) {
        if (dq.front() == x) dq.pop_front();
    }
    int getmin() { return dq.front(); }
};
int main() { return 0; }
