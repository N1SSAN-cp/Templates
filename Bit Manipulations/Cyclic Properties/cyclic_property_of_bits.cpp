#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

// convert x to binary string, strip leading zeros
string get_bit_string(i64 x) {
    string cur = bitset<64>(x).to_string();
    cur = cur.substr(cur.find('1'));
    return cur;
}

// count of set bits in [0, x]
i64 sum_of_bits(i64 x) {
    i64 ans = 0, tot = x + 1;
    for (int i = 0; i < 60; i++) {
        i64 full  = tot / (1LL << (i + 1));  // complete cycles
        i64 left  = tot % (1LL << (i + 1));  // remainder
        i64 extra = max(left - (1LL << i), 0LL);  // partial cycle ones
        ans += (full * (1LL << i)) + extra;
    }
    return ans;
}

// binary search: find number whose prefix sum of ones >= k
i64 find_kth_one(i64 k) {
    i64 lo = 0, hi = k, ans = -1;
    while (lo <= hi) {
        i64 mid = (lo + hi) / 2;
        if (sum_of_bits(mid) >= k) { ans = mid; hi = mid - 1; }
        else { lo = mid + 1; }
    }
    return ans;
}

// position (0-indexed) of k-th one in binary rep of x
i64 find_kth_one_pos_in_num(i64 x, i64 k) {
    i64 cnt = 0;
    string num = get_bit_string(x);
    int i;
    for (i = 0; i < (int)num.size(); i++) {
        if (num[i] == '1') { cnt++; }
        if (cnt == k) { break; }  // found k-th one
    }
    return i;
}

// total bits written when writing 1,2,...,x in binary
i64 total_bits_till(i64 x) {
    i64 ans = 0, len = 1, num = 1;
    while (num <= x) {
        i64 sametill = (num * 2) - 1;  // last number with same bit-length
        if (sametill <= x) {
            ans += len * (sametill - num + 1);  // full group
            num = sametill + 1;
            len++;
        } else {
            ans += len * (x - num + 1);  // partial group
            break;
        }
    }
    return ans + 1;  // 1-indexed
}

int main() {
    int q; cin >> q;
    while (q--) {
        i64 k; cin >> k;
        i64 num = find_kth_one(k);                       // number containing k-th one
        i64 pos_in_num = k - sum_of_bits(num - 1);       // local rank within num
        i64 index = find_kth_one_pos_in_num(num, pos_in_num);  // bit position
        cout << total_bits_till(num - 1) + index << "\n";
    }
    return 0;
}
