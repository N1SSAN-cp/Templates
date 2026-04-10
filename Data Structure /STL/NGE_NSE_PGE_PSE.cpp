#include <bits/stdc++.h>
using namespace std;

int main() {
    int n; cin >> n;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> NGE(n);
    NGE[n - 1] = n;
    for (int i = n - 2; i >= 0; i--) {
        NGE[i] = i + 1;
        while (NGE[i] < n && arr[i] >= arr[NGE[i]]) {
            NGE[i] = NGE[NGE[i]];
        }
    }

    vector<int> NSE(n);
    NSE[n - 1] = n;
    for (int i = n - 2; i >= 0; i--) {
        NSE[i] = i + 1;
        while (NSE[i] < n && arr[i] <= arr[NSE[i]]) {
            NSE[i] = NSE[NSE[i]];
        }
    }

    vector<int> PGE(n);
    PGE[0] = -1;
    for (int i = 1; i < n; i++) {
        PGE[i] = i - 1;
        while (PGE[i] >= 0 && arr[i] >= arr[PGE[i]]) {
            PGE[i] = PGE[PGE[i]];
        }
    }

    vector<int> PSE(n);
    PSE[0] = -1;
    for (int i = 1; i < n; i++) {
        PSE[i] = i - 1;
        while (PSE[i] >= 0 && arr[i] <= arr[PSE[i]]) {
            PSE[i] = PSE[PSE[i]];
        }
    }

    return 0;
}
