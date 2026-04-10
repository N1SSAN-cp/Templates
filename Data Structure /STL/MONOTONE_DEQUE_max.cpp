#include <bits/stdc++.h>
using namespace std;

// Monotone Deque: sliding window maximum
// insert/erase/getmax: O(1) amortized
// Problem: given array a[], find max in every contiguous subarray of size k
struct MonotoneDeque {
    deque<int> dq;
    void insert(int x) {
        while (!dq.empty() && dq.back() < x) {
            dq.pop_back();
        }
        dq.push_back(x);
    }
    void erase(int x) {
        if (!dq.empty() && dq.front() == x) {
            dq.pop_front();
        }
    }
    int getmax() {
        return dq.front();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}
