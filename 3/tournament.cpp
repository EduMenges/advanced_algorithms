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
#include <stack>
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
    adj[u].emplace_back(v, adj[v].size(), capacity, 0);
    adj[v].emplace_back(u, adj[u].size() - 1, 0, 0);
}

int dfs(uint32_t s,
        uint32_t t,
        span<int32_t> parent_node,
        span<uint32_t> parent_edge_index,
        span<vector<Edge>> adj) {
    fill(parent_node.begin(), parent_node.end(), -1);
    parent_node[s] = static_cast<int32_t>(s);

    std::stack<uint32_t> st;
    st.push(s);

    while (!st.empty()) {
        uint32_t u = st.top();
        st.pop();

        for (size_t i = 0; i < adj[u].size(); ++i) {
            const Edge& edge = adj[u][i];
            auto v = edge.to;

            if (parent_node[v] < 0 && edge.capacity - edge.flow > 0) {
                parent_node[v] = static_cast<int32_t>(u);
                parent_edge_index[v] = static_cast<uint32_t>(i);

                // Reached sink – back-track once to get the bottleneck
                if (v == t) {
                    int bottleneck = INF;
                    for (int cur = static_cast<int>(t); cur != static_cast<int>(s);
                         cur = parent_node[cur]) {
                        const Edge& pe = adj[parent_node[cur]][parent_edge_index[cur]];
                        bottleneck = std::min(bottleneck, pe.capacity - pe.flow);
                    }
                    return bottleneck;
                }

                st.push(v);
            }
        }
    }

    return 0;
}

int64_t maxflow(uint32_t s, uint32_t t, size_t n, span<vector<Edge>> adj) {
    vector<int32_t> parent_node(n);
    vector<uint32_t> parent_edge_index(n);

    int64_t total_flow = 0;
    int64_t aug;

    while ((aug = dfs(s, t, parent_node, parent_edge_index, adj))) {
        total_flow += aug;

        for (auto cur = t; cur != s; cur = parent_node[cur]) {
            int prev = parent_node[cur];
            auto edge_idx = parent_edge_index[cur];

            Edge& fwd = adj[prev][edge_idx];
            fwd.flow += aug;

            Edge& rev = adj[cur][fwd.reverse_edge];
            rev.flow -= aug;
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
