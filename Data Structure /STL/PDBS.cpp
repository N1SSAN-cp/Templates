#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;

template <class T>
struct PBDSMultiset {
    using P = pair<T, int>;
    tree<P, null_type, less<P>, rb_tree_tag, tree_order_statistics_node_update> t;

    int id = 0;

    // insert
    void insert(T x) {
        t.insert({x, id++});
    }

    // erase ONE occurrence
    void erase(T x) {
        auto it = t.lower_bound({x, -1});
        if (it != t.end() && it->first == x) {
            t.erase(it);
        }
    }

    // count < x
    int count_less(T x) {
        return t.order_of_key({x, -1});
    }

    // count <= x
    int count_less_equal(T x) {
        return t.order_of_key({x, INT_MAX});
    }

    // count > x
    int count_greater(T x) {
        return size() - count_less_equal(x);
    }

    // count >= x
    int count_greater_equal(T x) {
        return size() - count_less(x);
    }

    // kth (0-based)
    T kth(int k) {
        return t.find_by_order(k)->first;
    }

    // size
    int size() {
        return (int)t.size();
    }

    // lower_bound (>= x)
    T lower_bound(T x) {
        auto it = t.lower_bound({x, -1});
        if (it == t.end()) throw;
        return it->first;
    }

    // upper_bound (> x)
    T upper_bound(T x) {
        auto it = t.upper_bound({x, INT_MAX});
        if (it == t.end()) throw;
        return it->first;
    }
};
