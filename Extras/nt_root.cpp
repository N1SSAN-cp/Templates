auto nth_root = [&](long long k, int n) -> long long {
    if (n <= 0) return -1;
    long long lo{1}, hi{1'000'000'000'000'000'000LL};
    while (lo < hi) {
        long long mid{(lo + hi + 1) / 2};
        __int128 prod{1};
        bool overflow{false};
        for (int i{0}; i < n; i++) {
            prod *= mid;
            if (prod > k) { overflow = true; break; }
        }
        if (overflow) hi = mid - 1;
        else          lo = mid;
    }
    return lo;
};
