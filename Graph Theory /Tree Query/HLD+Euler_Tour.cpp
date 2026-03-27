#include <bits/stdc++.h>
using namespace std;
template <typename Weight = long long>
struct Tree {
    struct Edge {
        int to, id;
        Weight cost;
    };
    int N;
    vector<vector<Edge>> T;
    vector<pair<int, int>> edges;

    vector<int> lid, rid, head, order, parent, v_to_edge;
    vector<int> depth;
    vector<Weight> depth_weight;

    vector<int> memo_tail;

    Tree(int n = 0) { init(n); }

    void init(int n) {
        N = n;
        T.assign(N, {});
        edges.clear();
    }

    void add_edge(int u, int v, Weight w = 1) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        T[u].push_back({v, id, w});
        T[v].push_back({u, id, w});
    }

    void build(int root = 0, bool hld = true) {
        lid.assign(N, -1);
        rid.assign(N, -1);
        head.assign(N, root);
        order.assign(N, -1);
        parent.assign(N, -1);
        v_to_edge.assign(N, -1);
        depth.assign(N, -1);
        depth_weight.assign(N, 0);
        memo_tail.clear();

        int timer = 0;
        dfs_size(root, -1, hld);
        dfs_hld(root, timer);
    }

    void dfs_size(int v, int p, bool hld) {
        parent[v] = p;
        depth[v] = (p == -1 ? 0 : depth[p] + 1);
        rid[v] = 1;

        int heavy_size = 0;

        for (auto& e : T[v]) {
            if (e.to == p) {
                continue;
            }

            depth_weight[e.to] = depth_weight[v] + e.cost;
            v_to_edge[e.to] = e.id;

            dfs_size(e.to, v, hld);

            rid[v] += rid[e.to];

            if (hld && rid[e.to] > heavy_size) {
                heavy_size = rid[e.to];
                swap(e, T[v][0]);
            }
        }
    }

    void dfs_hld(int v, int& timer) {
        lid[v] = timer;
        order[timer] = v;
        timer++;

        bool heavy = true;

        for (auto& e : T[v]) {
            int to = e.to;
            if (to == parent[v]) {
                continue;
            }

            head[to] = heavy ? head[v] : to;
            heavy = false;

            dfs_hld(to, timer);
        }

        rid[v] += lid[v];
    }

    vector<int> heavy_path_at(int v) {
        vector<int> path;
        path.push_back(v);

        while (true) {
            int a = path.back();
            bool extended = false;

            for (auto& e : T[a]) {
                if (e.to != parent[a] && head[e.to] == v) {
                    path.push_back(e.to);
                    extended = true;
                    break;
                }
            }

            if (!extended) {
                break;
            }
        }

        return path;
    }

    int heavy_child(int v) {
        int k = lid[v] + 1;
        if (k == N) {
            return -1;
        }
        int w = order[k];
        return (parent[w] == v) ? w : -1;
    }

    int tail(int v) {
        if (memo_tail.empty()) {
            memo_tail.assign(N, -1);
            for (int i = N - 1; i >= 0; i--) {
                int x = order[i];
                int w = heavy_child(x);
                memo_tail[x] = (w == -1) ? x : memo_tail[w];
            }
        }
        return memo_tail[v];
    }

    int e_to_v(int eid) {
        auto [u, v] = edges[eid];
        return (parent[u] == v) ? u : v;
    }

    int v_to_e(int v) { return v_to_edge[v]; }

    int get_eid(int u, int v) {
        if (parent[u] != v) {
            swap(u, v);
        }
        return v_to_edge[u];
    }

    int ELID(int v) { return 2 * lid[v] - depth[v]; }
    int ERID(int v) { return 2 * rid[v] - depth[v] - 1; }

    int kth_ancestor(int v, int k) {
        while (true) {
            int h = head[v];
            if (lid[v] - k >= lid[h]) {
                return order[lid[v] - k];
            }
            k -= lid[v] - lid[h] + 1;
            v = parent[h];
        }
    }

    int la(int v, int k) { return kth_ancestor(v, k); }

    int lca(int a, int b) {
        while (head[a] != head[b]) {
            if (lid[a] > lid[b]) {
                swap(a, b);
            }
            b = parent[head[b]];
        }
        return (lid[a] < lid[b]) ? a : b;
    }

    int meet(int a, int b, int c) { return lca(a, b) ^ lca(a, c) ^ lca(b, c); }

    int dist(int a, int b) {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }

    Weight dist_weighted(int a, int b) {
        int c = lca(a, b);
        return depth_weight[a] + depth_weight[b] - 2 * depth_weight[c];
    }

    bool in_subtree(int a, int b) {
        return lid[b] <= lid[a] && lid[a] < rid[b];
    }

    int subtree_size(int v) { return rid[v] - lid[v]; }

    int jump(int a, int b, long long k) {
        int c = lca(a, b);
        int da = depth[a] - depth[c];
        int db = depth[b] - depth[c];

        if (k > da + db) {
            return -1;
        }
        if (k <= da) {
            return kth_ancestor(a, k);
        }
        return kth_ancestor(b, da + db - k);
    }

    vector<int> cointect_child(int v) {
        vector<int> res;
        for (auto& e : T[v]) {
            if (e.to != parent[v]) {
                res.push_back(e.to);
            }
        }
        return res;
    }

    vector<int> cointect_subtree(int v) {
        return vector<int>(order.begin() + lid[v], order.begin() + rid[v]);
    }

    vector<int> cointect_light(int v) {
        vector<int> res;
        bool skip = true;
        for (auto& e : T[v]) {
            if (e.to != parent[v]) {
                if (!skip) {
                    res.push_back(e.to);
                }
                skip = false;
            }
        }
        return res;
    }

    vector<pair<int, int>> get_path_decomposition(int u, int v, bool edge) {
        vector<pair<int, int>> up, down;

        while (head[u] != head[v]) {
            if (lid[u] < lid[v]) {
                down.push_back({lid[head[v]], lid[v]});
                v = parent[head[v]];
            } else {
                up.push_back({lid[u], lid[head[u]]});
                u = parent[head[u]];
            }
        }

        if (lid[u] < lid[v]) {
            down.push_back({lid[u] + edge, lid[v]});
        } else if (lid[v] + edge <= lid[u]) {
            up.push_back({lid[u], lid[v] + edge});
        }

        reverse(down.begin(), down.end());
        up.insert(up.end(), down.begin(), down.end());
        return up;
    }

    vector<int> restore_path(int u, int v) {
        vector<int> path;

        auto seg = get_path_decomposition(u, v, false);

        for (auto [a, b] : seg) {
            if (a <= b) {
                for (int i = a; i <= b; i++) {
                    path.push_back(order[i]);
                }
            } else {
                for (int i = a; i >= b; i--) {
                    path.push_back(order[i]);
                }
            }
        }

        return path;
    }

    pair<int, int> path_intersection(int a, int b, int c, int d) {
        int ab = lca(a, b);
        int ac = lca(a, c);
        int ad = lca(a, d);
        int bc = lca(b, c);
        int bd = lca(b, d);
        int cd = lca(c, d);

        int x = ab ^ ac ^ bc;
        int y = ab ^ ad ^ bd;

        if (x != y) {
            return {x, y};
        }

        int z = ac ^ ad ^ cd;

        if (x != z) {
            x = -1;
        }
        return {x, x};
    }

    template <class F>
    int max_path(F check, int u, int v) {
        if (!check(u)) {
            return -1;
        }

        auto pd = get_path_decomposition(u, v, false);

        for (auto [a, b] : pd) {
            if (!check(order[a])) {
                return u;
            }

            if (check(order[b])) {
              u = order[b]; continue;
            }

            int lo = min(a, b);
            int hi = max(a, b);

            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (check(order[mid])) {
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }

            return order[lo];
        }

        return u;
    }
};

int main() {
    // 1. Initialize a tree with 6 nodes (0 to 5)
    int n = 6;
    Tree<long long> tr(n);

    // 2. Add edges: (u, v, weight)
    // Structure: 0 - 1, 0 - 2, 1 - 3, 1 - 4, 2 - 5
    tr.add_edge(0, 1, 10);
    tr.add_edge(0, 2, 20);
    tr.add_edge(1, 3, 5);
    tr.add_edge(1, 4, 7);
    tr.add_edge(2, 5, 3);

    // 3. Build the HLD structure (rooting at node 0)
    tr.build(0);

    // --- Common Queries ---

    // Lowest Common Ancestor
    cout << "LCA of 3 and 4: " << tr.lca(3, 4) << endl; // Output: 1
    cout << "LCA of 3 and 5: " << tr.lca(3, 5) << endl; // Output: 0

    // Distances
    cout << "Edge distance (3 to 5): " << tr.dist(3, 5) << endl;          // Output: 4 (3-1-0-2-5)
    cout << "Weighted distance (3 to 5): " << tr.dist_weighted(3, 5) << endl; // Output: 38 (5+10+20+3)

    // Ancestor and Jumps
    cout << "2nd ancestor of 3: " << tr.kth_ancestor(3, 2) << endl; // Output: 0
    cout << "Jump from 3 towards 5 by 2 steps: " << tr.jump(3, 5, 2) << endl; // Output: 0

    // Path Restoration
    vector<int> path = tr.restore_path(3, 5);
    cout << "Path 3 to 5: ";
    for (int node : path) cout << node << " "; // Output: 3 1 0 2 5
    cout << endl;

    // Subtree properties
    cout << "Subtree size of node 1: " << tr.subtree_size(1) << endl; // Output: 3 (nodes 1, 3, 4)
    cout << "Is 3 in subtree of 1? " << (tr.in_subtree(3, 1) ? "Yes" : "No") << endl;

    // 4. HLD Path Decomposition (Useful for Segment Tree integrations)
    // Get segments in the 'lid' array that cover the path from 3 to 5
    auto segments = tr.get_path_decomposition(3, 5, false);
    cout << "HLD lid segments for path 3 to 5: " << endl;
    for (auto [l, r] : segments) {
        cout << "[" << l << ", " << r << "] ";
    }
    // These segments can be used to query a Segment Tree in O(log^2 N)

    return 0;
}
