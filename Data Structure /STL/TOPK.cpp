#include <bits/stdc++.h>
using namespace std;

// TopK — maintains top K elements with their sum
// insert/remove: O(log n)

template <typename T, typename Comp = less<T>>
struct TopK {
    int K{};
    multiset<T, Comp> in{}, out{};
    T sumK{};
    Comp comp{};

    TopK(int _K, Comp c = Comp{}) : K{_K}, sumK{}, comp{c} {}

    void clear() {
        in.clear();
        out.clear();
        sumK = T{};
    }

    void insert(const T& x) {
        in.insert(x);
        sumK += x;
        if ((int)in.size() > K) {
            auto it = prev(in.end());
            sumK -= *it;
            out.insert(*it);
            in.erase(it);
        }
    }

    void remove(const T& x) {
        auto it = in.find(x);
        if (it != in.end()) {
            sumK -= *it;
            in.erase(it);
        } else {
            it = out.find(x);
            if (it == out.end()) {
                return;
            }
            out.erase(it);
        }
        if ((int)in.size() < K && !out.empty()) {
            auto it2 = out.begin();
            sumK += *it2;
            in.insert(*it2);
            out.erase(it2);
        }
    }

    T getSum() const {
        return ((int)in.size() == K) ? sumK : T{};
    }
};
// Example usage
int main() {
    // top 3 smallest (default less<int> keeps smallest in 'in')
    TopK<int> tk(3);

    tk.insert(5);
    tk.insert(2);
    tk.insert(8);
    tk.insert(1);  // pushes 8 out

    int s = tk.getSum();  // 1+2+5 = 8

    tk.remove(2);
    s = tk.getSum();  // not enough elements, returns 0

    // top 2 largest using greater<int>
    TopK<int, greater<int>> tk2(2);
    tk2.insert(3);
    tk2.insert(7);
    tk2.insert(5);  // pushes 3 out

    s = tk2.getSum();  // 7+5 = 12

    return 0;
}
