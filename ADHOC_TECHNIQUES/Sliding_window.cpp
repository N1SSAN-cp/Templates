#include <bits/stdc++.h>
using namespace std;

// Sliding Window — Longest subarray with all distinct elements
// O(n)

struct DS {
    map<int, int> freq;

    bool check(int x) {
        return freq.find(x) == freq.end();
    }

    void add(int x) {
        freq[x]++;
    }

    void rem(int x) {
        freq[x]--;
        if (freq[x] == 0) {
            freq.erase(x);
        }
    }

    void clear() {
        freq.clear();
    }

    int size() {
        return freq.size();
    }
};

void solve() {
    int n; cin >> n;
    vector<int> arr(n);
    for (auto& x : arr) {
        cin >> x;
    }
    DS Window;
    int tail = 0, head = -1, ans = 0;

    while (tail < n) {
        while (head + 1 < n && Window.check(arr[head + 1])) {
            head++;
            Window.add(arr[head]);
        }
        ans = max(ans, head - tail + 1);
        if (tail > head) {
            head = ++tail - 1;
            Window.clear();
        } else {
            Window.rem(arr[tail]);
            tail++;
        }
    }

    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
