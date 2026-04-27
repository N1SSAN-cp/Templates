#include <bits/stdc++.h>
using namespace std;

// Union-Find (path compression + union by size), O(alpha(N))
struct UnionFind {
    vector<int> par;
    UnionFind(int n) : par(n, -1) {}
    int find(int x) { return par[x] < 0 ? x : par[x] = find(par[x]); }
    bool merge(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) { return false; }
        if (par[x] > par[y]) { swap(x, y); }
        par[x] += par[y];
        par[y] = x;
        return true;
    }
    int operator[](int x) { return find(x); }
};

// Euler walk (Hierholzer's algorithm), O(N + M)
// edges[i] = {frm, to}, directed flag for directed/undirected
// Returns (vertex sequence, edge sequence) or ({}, {}) if no euler walk exists
pair<vector<int>, vector<int>> euler_walk(
    int N, const vector<array<int, 2>>& edges, bool directed, int s = -1
) {
    int M = (int)edges.size();
    assert(N > 0);
    assert(s == -1 || (0 <= s && s < N));

    vector<int> deg(N, 0);
    for (auto& e : edges) {
        if (directed) {
            deg[e[0]]++;
            deg[e[1]]--;
        } else {
            deg[e[0]]++;
            deg[e[1]]++;
        }
    }

    if (s == -1) {
        if (directed) {
            s = (int)(max_element(deg.begin(), deg.end()) - deg.begin());
            if (deg[s] == 0) { s = (M == 0 ? 0 : edges[0][0]); }
        } else {
            s = -1;
            for (int v = 0; v < N; v++) {
                if (deg[v] & 1) { s = v; break; }
            }
            if (s == -1) { s = (M == 0 ? 0 : edges[0][0]); }
        }
    }

    if (M == 0) { return {{s}, {}}; }

    // adj[u] = list of {to, edge_id}
    vector<vector<array<int, 2>>> adj(N);
    for (int i = 0; i < M; i++) {
        adj[edges[i][0]].push_back({edges[i][1], i});
        if (!directed) { adj[edges[i][1]].push_back({edges[i][0], i}); }
    }

    vector<int> its(N, 0), eu(M, 0), D(N, 0);
    vector<int> vs, es_out;
    // stack entry: {vertex, edge_id used to reach it} (-1 for start vertex)
    vector<array<int, 2>> st = {{s, -1}};
    D[s]++;

    while (!st.empty()) {
        int x = st.back()[0];
        int& it = its[x];
        if (it == (int)adj[x].size()) {
            vs.push_back(x);
            if (st.back()[1] != -1) { es_out.push_back(st.back()[1]); }
            st.pop_back();
            continue;
        }
        int y = adj[x][it][0], e = adj[x][it][1];
        it++;
        if (!eu[e]) {
            D[x]--;
            D[y]++;
            eu[e] = 1;
            st.push_back({y, e});
        }
    }

    for (int x : D) {
        if (x < 0) { return {{}, {}}; }
    }
    if ((int)vs.size() != M + 1) { return {{}, {}}; }
    reverse(vs.begin(), vs.end());
    reverse(es_out.begin(), es_out.end());
    return {vs, es_out};
}

// Check if Euler walk exists, O(N + M)
bool has_euler_walk(int N, const vector<array<int, 2>>& edges, bool directed, int s = -1) {
    int M = (int)edges.size();
    if (M == 0) { return true; }

    UnionFind uf(N);
    for (auto& e : edges) { uf.merge(e[0], e[1]); }

    vector<int> cnt_edge(N, 0);
    for (auto& e : edges) { cnt_edge[uf[e[0]]]++; }
    if (s != -1 && cnt_edge[uf[s]] == 0) { return false; }

    // Count components with edges
    int nc = 0;
    for (int v = 0; v < N; v++) {
        if (uf[v] == v && cnt_edge[v] >= 1) { nc++; }
    }
    if (nc > 1) { return false; }

    if (!directed) {
        vector<int> odd(N, 0);
        for (auto& e : edges) { odd[e[0]] ^= 1; odd[e[1]] ^= 1; }
        int n_odd = 0;
        for (int x : odd) { n_odd += x; }
        if (n_odd >= 4) { return false; }
        if (s != -1 && n_odd == 2 && !odd[s]) { return false; }
    } else {
        vector<int> in(N, 0), out(N, 0);
        for (auto& e : edges) { out[e[0]]++; in[e[1]]++; }
        int ng = 0;
        for (int v = 0; v < N; v++) { ng += abs(out[v] - in[v]); }
        if (ng >= 4) { return false; }
        if (s != -1 && ng == 2 && out[s] != in[s] + 1) { return false; }
    }

    return true;
}

int main() {
    // Undirected Euler circuit: 0-1-2-0
    {
        int N = 3;
        vector<array<int, 2>> edges = {{0,1}, {1,2}, {2,0}};
        if (has_euler_walk(N, edges, false)) {
            auto [vs, es] = euler_walk(N, edges, false);
            cout << "Undirected circuit vertices: ";
            for (int v : vs) { cout << v << " "; }
            cout << "\n";
            cout << "Undirected circuit edges:    ";
            for (int e : es) { cout << e << " "; }
            cout << "\n";
        }
    }

    // Undirected Euler path: 0-1-2-3 (start forced at 0)
    {
        int N = 4;
        vector<array<int, 2>> edges = {{0,1}, {1,2}, {2,3}};
        if (has_euler_walk(N, edges, false, 0)) {
            auto [vs, es] = euler_walk(N, edges, false, 0);
            cout << "Undirected path vertices: ";
            for (int v : vs) { cout << v << " "; }
            cout << "\n";
            cout << "Undirected path edges:    ";
            for (int e : es) { cout << e << " "; }
            cout << "\n";
        }
    }

    // Directed Euler circuit: 0->1->2->0
    {
        int N = 3;
        vector<array<int, 2>> edges = {{0,1}, {1,2}, {2,0}};
        if (has_euler_walk(N, edges, true)) {
            auto [vs, es] = euler_walk(N, edges, true);
            cout << "Directed circuit vertices: ";
            for (int v : vs) { cout << v << " "; }
            cout << "\n";
            cout << "Directed circuit edges:    ";
            for (int e : es) { cout << e << " "; }
            cout << "\n";
        }
    }

    // No Euler walk exists
    {
        int N = 3;
        vector<array<int, 2>> edges = {{0,1}, {0,2}};
        if (!has_euler_walk(N, edges, false)) {
            cout << "No Euler walk exists\n";
        }
    }

    return 0;
}
