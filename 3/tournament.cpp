#if __has_include(<mimalloc-new-delete.h>)
#include <mimalloc-new-delete.h>
#endif

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

constexpr int32_t INF = numeric_limits<int32_t>::max();

struct Edge {
    uint32_t to;            // Destination node of the edge
    uint32_t reverse_edge;  // Index of the reverse edge in the 'to' node's adjacency list
    int capacity;           // Maximum capacity of the edge
    int flow;               // Current flow through the edge
};

namespace {
constexpr void add_edge(vector<vector<Edge>>& adj, uint32_t u, uint32_t v, int capacity) {
    adj[u].emplace_back(v, static_cast<uint32_t>(adj[v].size()), capacity, 0);
    adj[v].emplace_back(u, static_cast<uint32_t>(adj[u].size()) - 1, 0, 0);
}

bool bfs(uint32_t s, uint32_t t, vector<int>& level, span<vector<Edge>> adj) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;

    queue<uint32_t> q;
    q.push(s);

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();

        for (const auto& edge : adj[u]) {
            if (level[edge.to] < 0 && edge.capacity - edge.flow > 0) {
                level[edge.to] = level[u] + 1;
                q.push(edge.to);
            }
        }
    }

    return level[t] >= 0;
}

// DFS for finding blocking flow
int dfs(uint32_t u,
        uint32_t t,
        int flow,
        span<size_t> ptr,
        const vector<int>& level,
        span<vector<Edge>> adj) {
    if (u == t || flow == 0) {
        return flow;
    }

    for (; ptr[u] < adj[u].size(); ++ptr[u]) {
        Edge& edge = adj[u][ptr[u]];
        if (level[edge.to] == level[u] + 1 && edge.capacity - edge.flow > 0) {
            int pushed = dfs(edge.to, t, min(flow, edge.capacity - edge.flow), ptr, level, adj);
            if (pushed > 0) {
                edge.flow += pushed;
                adj[edge.to][edge.reverse_edge].flow -= pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int64_t maxflow(uint32_t s, uint32_t t, size_t n, span<vector<Edge>> adj) {
    vector<int32_t> level(n);
    vector<size_t> ptr(n);

    int64_t total_flow = 0;

    // Continue while there is a path in the level graph
    while (bfs(s, t, level, adj)) {
        fill(ptr.begin(), ptr.end(), 0);

        // Find blocking flows
        while (auto flow = dfs(s, t, INF, ptr, level, adj)) {
            total_flow += flow;
        }
    }

    return total_flow;
}

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n;
    cin >> n;

    vector<uint32_t> wins(n + 1);

    for (size_t i = 1; i <= n; ++i) {
        cin >> wins[i];
    }

    vector<uint32_t> raw_games((n + 1) * (n + 1));
    mdspan games(raw_games.data(), n + 1, n + 1);
    uint64_t total_games_excluding_team_1 = 0;

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = i + 1; j <= n; ++j) {
            int g;
            cin >> g;
            games[i, j] = g;
            games[j, i] = g;
            if (i != 1) [[likely]] {
                total_games_excluding_team_1 += g;
            }
        }
    }

    uint64_t remaining_games_team_1 = 0;
    for (size_t j = 2; j <= n; ++j) {
        remaining_games_team_1 += games[1, j];
    }

    auto max_possible_victories_team_1 = wins[1] + remaining_games_team_1;

    vector<int32_t> limits(n + 1);

    for (size_t i = 2; i <= n; ++i) {
        limits[i] = max_possible_victories_team_1 - wins[i] - 1;
        if (auto already_lost = limits[i] < 0; already_lost) {
            cout << "false\n";
            return EXIT_SUCCESS;
        }
    }

    // Source index
    size_t s = 0;

    size_t game_nodes_start = s + 1;

    // Pairs of teams excluding team 1
    size_t num_game_nodes = (n - 1) * (n - 2) / 2;

    // Where the teams-related nodes start (teams 2 to n)
    size_t team_node_start = game_nodes_start + num_game_nodes;

    // Sink
    size_t t = team_node_start + n - 1;

    // Total amount of nodes in the graph
    size_t node_count = t + 1;

    // Adjacency list to store the graph
    vector<vector<Edge>> adj(node_count);

    size_t current_game_node = game_nodes_start;

    // Creating game nodes and edges from source to game nodes and game nodes to team nodes
    for (size_t i = 2; i <= n; ++i) {
        for (size_t j = i + 1; j <= n; ++j) {
            int gij = games[i, j];
            if (gij == 0)
                continue;

            auto g_idx = current_game_node++;

            // Add edge from source to game node with capacity equal to the number of games between
            // i and j
            add_edge(adj, s, g_idx, gij);

            // Add edge from game node to team i with infinite capacity
            auto tid_i = team_node_start + (i - 2);
            add_edge(adj, g_idx, tid_i, INF);

            // Add edge from game node to team j with infinite capacity
            auto tid_j = team_node_start + (j - 2);
            add_edge(adj, g_idx, tid_j, INF);
        }
    }

    // Add edges from team nodes to the sink with capacity equal to the remaining allowed losses for
    // each team
    for (size_t i = 2; i <= n; ++i) {
        auto tid = team_node_start + (i - 2);
        add_edge(adj, tid, t, limits[i]);
    }

    // Calculate the max flow from source to sink
    int result = maxflow(s, t, node_count, adj);

    // If the max flow equals the total number of games played between teams 2 to n,
    // it means all these games can be allocated without any team exceeding their victory limit
    if (result == total_games_excluding_team_1) {
        cout << "true\n";
    } else {
        cout << "false\n";
    }

    return EXIT_SUCCESS;
}
