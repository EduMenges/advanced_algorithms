#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

constexpr int32_t INF = numeric_limits<int32_t>::max();

int maxflow(int s, int t, int n, mdspan<int32_t, dextents<size_t, 2>> cap, span<vector<int>> adj) {
    int total_flow = 0;
    vector<int> raw_flow(n * n);
    mdspan flow(raw_flow.data(), n, n);
    vector<int> parent(n);

    auto bfs = [&] {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        queue<pair<int, int>> q;
        q.push({s, INF});

        while (!q.empty()) {
            int u = q.front().first;
            int f = q.front().second;
            q.pop();

            for (int v : adj[u]) {
                if (parent[v] == -1 && cap[u, v] - flow[u, v] > 0) {
                    parent[v] = u;
                    int new_flow = min(f, cap[u, v] - flow[u, v]);
                    if (v == t)
                        return new_flow;
                    q.push({v, new_flow});
                }
            }
        }

        return 0;
    };

    int new_flow;
    while ((new_flow = bfs())) {
        total_flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            flow[prev, cur] += new_flow;
            flow[cur, prev] -= new_flow;
            cur = prev;
        }
    }

    return total_flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    size_t n;
    cin >> n;

    vector<uint32_t> wins(n + 1);

    for (size_t i = 1; i <= n; ++i) {
        cin >> wins[i];
    }

    vector<uint32_t> raw_games((n + 1) * (n + 1));
    mdspan games(raw_games.data(), n + 1, n + 1);
    uint64_t total_games = 0;

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = i + 1; j <= n; ++j) {
            int g;
            cin >> g;
            games[i, j] = g;
            games[j, i] = g;
            if (i != 1) [[likely]] {
                total_games += g;
            }
        }
    }

    uint64_t remaining_games = 0;
    for (size_t j = 2; j <= n; ++j) {
        remaining_games += games[1, j];
    }

    auto max_possible_victories = wins[1] + remaining_games;

    vector<int32_t> limits(n + 1);

    for (size_t i = 2; i <= n; ++i) {
        limits[i] = max_possible_victories - wins[i] - 1;
        auto already_lost = limits[i] < 0;
        if (already_lost) {
            cout << "false\n";
            return EXIT_SUCCESS;
        }
    }

    // Source index
    size_t s = 0;

    size_t game_nodes_start = s + 1;

    size_t game_node = game_nodes_start;

    size_t game_nodes_end = game_nodes_start + (n - 1) * (n - 2) / 2;

    // Where the teams-related nodes start
    size_t team_node_start = game_nodes_end;

    // Sink
    size_t t = team_node_start + n - 1;

    // Total amount of nodes in the graph
    size_t node_count = t + 1;

    vector<int32_t> raw_cap(node_count * node_count);
    mdspan cap(raw_cap.data(), node_count, node_count);

    vector<vector<int32_t>> adj(node_count);

    // Creating game nodes
    for (size_t i = 2; i <= n; ++i) {
        for (size_t j = i + 1; j <= n; ++j) {
            int gij = games[i, j];
            if (gij == 0)
                continue;

            auto g_idx = game_node++;

            // s -> game
            cap[s, g_idx] = gij;
            adj[s].push_back(g_idx);
            adj[g_idx].push_back(s);

            // game -> team i
            auto tid_i = team_node_start + (i - 2);
            cap[g_idx, tid_i] = INF;
            adj[g_idx].push_back(tid_i);
            adj[tid_i].push_back(g_idx);

            // game -> team j
            auto tid_j = team_node_start + (j - 2);
            cap[g_idx, tid_j] = INF;
            adj[g_idx].push_back(tid_j);
            adj[tid_j].push_back(g_idx);
        }
    }

    // team i -> t
    for (size_t i = 2; i <= n; ++i) {
        auto tid = team_node_start + (i - 2);
        cap[tid, t] = limits[i];
        adj[tid].push_back(t);
        adj[t].push_back(tid);
    }

    int result = maxflow(s, t, node_count, cap, adj);
    if (result == total_games) {
        cout << "true\n";
    } else {
        cout << "false\n";
    }

    return 0;
}
