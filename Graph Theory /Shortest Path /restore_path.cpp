#pragma once
#include <bits/stdc++.h>
using namespace std;

// Path Reconstruction from parent array
// Works for BFS, 0/1 BFS, Dijkstra, Bellman-Ford
// par[src] must be -1

auto restore_path = [](vector<int>& par, int t) -> vector<int> {
    vector<int> path = {t};
    while (par[path.back()] != -1) {
        path.push_back(par[path.back()]);
    }
    reverse(path.begin(), path.end());
    return path;
};
