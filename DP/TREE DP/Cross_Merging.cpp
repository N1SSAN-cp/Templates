#include <bits/stdc++.h>
using namespace std;

/*
PROBLEM: FIND a global property for all possible path of tree.
         Transformation: Solve the problem for all possible subarray of a array  THEN USE THIS FORMULATION
FOR PATH QUERY: USE HLD
*/


/*

    Think in terms of:
    - identity element
    - merge operation
    - extend operation

    Works for:
    - GCD paths
    - OR / AND paths
    - sets / maps
    - custom structs
*/

using Type = int; // can be anything (int, set, map, struct...)

int n;
vector<vector<int>> adj;
vector<Type> a;

vector<Type> dp;
Type ans;

/*
    IDENTITY:
    - neutral element for your operation
    Examples:
        sum  -> 0
        gcd  -> 0
        and  -> all_bits_set
        or   -> 0
*/
Type identity() {
    return 0;
}

/*
    addociative operation = order of merhing does not matter
    MERGE (associative operation)
    - combines two independent states
    Examples:
        sum  -> a + b
        gcd  -> gcd(a, b)
        or   -> a | b
*/
Type merge_op(Type x, Type y) {
    return std::gcd(x, y); // example
}

/*
    EXTEND:
    - attaches node u to a child path
*/
Type extend_op(int u, Type child) {
    return std::gcd(a[u], child); // example
}

/*
    ADD TO ANSWER:
    - how to use computed value
*/
void add_answer(Type val) {
    ans = merge_op(ans, val); // or store in map / count / etc.
}

void dfs(int u, int p) {

    // 1. BASE STATE
    dp[u] = a[u];
    add_answer(dp[u]);

    // accumulator for combining multiple children
    Type acc = identity();

    // 2. PROCESS CHILDREN
    for (int v : adj[u]) {
        if (v == p) continue;

        dfs(v, u);

        // 3. CROSS CONTRIBUTION (develop for two arrays (A.B))
        // combine previous children with current child
        Type cross = merge_op(acc, dp[v]);
        add_answer(cross);

        // 4. UPDATE ACCUMULATOR(OPT)
        acc = merge_op(acc, dp[v]);
    }

    // 5. MERGE V into U: EXTEND ALL CHILD CONTRIBUTIONS TO u ((develop for two arrays (A.B)))
    dp[u] = extend_op(u, acc);

    add_answer(dp[u]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    a.resize(n + 1);
    dp.resize(n + 1);
    adj.assign(n + 1, {});

    for (int i = 1; i <= n; i++) cin >> a[i];

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    ans = identity();

    dfs(1, 0);

    // print depending on problem
}
