#include <bits/stdc++.h>
using namespace std;

// Minimum terms = len(prefix) + 2 * p
// Z-algorithm: O(n)
template <typename STRING>
vector<int> zalgorithm(const STRING& s) {
    int n = int(s.size());
    if (n == 0) { return {}; }
    vector<int> z(n);
    z[0] = 0;
    for (int i = 1, j = 0; i < n; i++) {
        int& k = z[i];
        k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]);
        while (i + k < n && s[k] == s[i + k]) { k++; }
        if (j + z[j] < i + z[i]) { j = i; }
    }
    z[0] = n;
    return z;
}

// Sequence like: 012[345][345][345]
template <typename T>
struct Interpolate_Periodic_Sequence {
    vector<T> dat;
    int p;

    Interpolate_Periodic_Sequence(vector<T> A) : dat(A) {
        reverse(A.begin(), A.end());
        auto Z = zalgorithm(A);
        Z[0] = 0;
        p = max_element(Z.begin(), Z.end()) - Z.begin();
    }

    T operator[](long long n) {
        if (n < (int)dat.size()) { return dat[n]; }
        long long k = (n - ((int)dat.size() - 1) + p - 1) / p;
        n -= k * p;
        return dat[n];
    }
};

// Sequence whose differences are like: 012[345][345][345]
// Interpolate_Difference_Periodic_Sequenceprefix + 2*p + 1
template <typename T>
struct Interpolate_Difference_Periodic_Sequence {
    vector<T> dat;
    T d;
    int p;

    Interpolate_Difference_Periodic_Sequence(vector<T> A) : dat(A) {
        vector<T> diff;
        for (int i = 0; i < (int)A.size() - 1; i++) { diff.push_back(A[i + 1] - A[i]); }
        reverse(diff.begin(), diff.end());
        auto Z = zalgorithm(diff);
        Z[0] = 0;
        p = max_element(Z.begin(), Z.end()) - Z.begin();
        long long n = (int)A.size();
        d = A[n - 1] - A[n - p - 1];
    }

    T operator[](long long n) {
        if (n < (int)dat.size()) { return dat[n]; }
        long long k = (n - ((int)dat.size() - 1) + p - 1) / p;
        n -= k * p;
        return dat[n] + k * d;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> A = {0, 1, 2, 3, 4, 5, 3, 4, 5};
    Interpolate_Periodic_Sequence<int> seq(A);

    cout << seq[0] << "\n";
    cout << seq[4] << "\n";
    cout << seq[9] << "\n";
    cout << seq[12] << "\n";

    vector<int> B = {0, 1, 2, 5, 8, 11, 14};
    Interpolate_Difference_Periodic_Sequence<int> dseq(B);

    cout << dseq[3] << "\n";
    cout << dseq[6] << "\n";
    cout << dseq[7] << "\n";
    cout << dseq[10] << "\n";

    return 0;
}
