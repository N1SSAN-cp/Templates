// Strongly Connected Components (Tarjan's Algorithm)
// Time: O(V + E)
#include <bits/stdc++.h>
using namespace std;

struct SCC {
    int V = 0;
    vector<vector<int>> adj;
    vector<int> tour_index, low_link;
    int tour;
    vector<int> stack;
    vector<bool> in_stack;
    vector<vector<int>> components;
    vector<int> which_component;

    SCC(int v = 0) { init(v); }
    SCC(const vector<vector<int>> &_adj) { init(_adj); }

    void init(int v) {
        V = v;
        adj.assign(V, {});
    }

    void init(const vector<vector<int>> &_adj) {
        adj = _adj;
        V = int(adj.size());
    }

    void add_edge(int a, int b) {
        adj[a].push_back(b);
    }

    void dfs(int node) {
        tour_index[node] = tour++;
        low_link[node] = tour_index[node];
        stack.push_back(node);
        in_stack[node] = true;

        for (int nb : adj[node]) {
            if (tour_index[nb] < 0) {
                dfs(nb);
                low_link[node] = min(low_link[node], low_link[nb]);
            } else if (in_stack[nb]) {
                low_link[node] = min(low_link[node], tour_index[nb]);
            }
        }

        if (low_link[node] == tour_index[node]) {
            components.emplace_back();
            vector<int> &comp = components.back();
            int x;
            do {
                assert(!stack.empty());
                x = stack.back();
                stack.pop_back();
                in_stack[x] = false;
                which_component[x] = int(components.size()) - 1;
                comp.push_back(x);
            } while (x != node);
        }
    }

    void build() {
        tour_index.assign(V, -1);
        low_link.resize(V);
        which_component.assign(V, -1);
        stack.clear();
        stack.reserve(V);
        in_stack.assign(V, false);
        tour = 0;
        components = {};

        for (int i = 0; i < V; i++) {
            if (tour_index[i] < 0) {
                dfs(i);
            }
        }
    }

    // Returns condensed DAG after build().
    // Node i = SCC i (same indexing as components[i]).
    // Tarjan's gives SCCs in reverse topological order, so edges go from later to earlier indices.
    vector<vector<int>> condensed() {
        int m = int(components.size());
        vector<vector<int>> dag(m);
        for (int u = 0; u < V; u++) {
            for (int v : adj[u]) {
                int cu = which_component[u], cv = which_component[v];
                if (cu != cv) {
                    dag[cu].push_back(cv);
                }
            }
        }
        for (auto &nbrs : dag) {
            sort(nbrs.begin(), nbrs.end());
            nbrs.erase(unique(nbrs.begin(), nbrs.end()), nbrs.end());
        }
        return dag;
    }
};

int main() {
    // 5 nodes: {0,1,2} form an SCC, {3} and {4} are singletons
    // 0->1->2->0 (cycle), 2->3, 3->4
    SCC scc(5);
    scc.add_edge(0, 1);
    scc.add_edge(1, 2);
    scc.add_edge(2, 0);
    scc.add_edge(2, 3);
    scc.add_edge(3, 4);
    scc.build();

    cout << "SCCs: " << scc.components.size() << "\n";
    for (int i = 0; i < (int)scc.components.size(); i++) {
        cout << "SCC " << i << ": ";
        for (int x : scc.components[i]) { cout << x << " "; }
        cout << "\n";
    }

    auto dag = scc.condensed();
    cout << "Condensed DAG:\n";
    for (int u = 0; u < (int)dag.size(); u++) {
        for (int v : dag[u]) {
            cout << u << " -> " << v << "\n";
        }
    }
}
