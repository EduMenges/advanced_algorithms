#if __has_include(<mimalloc-new-delete.h>)
#include <mimalloc-new-delete.h>
#endif

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

constexpr int32_t INF = numeric_limits<int32_t>::max();

struct Edge {
    int to;            // Destination node of the edge
    int capacity;      // Maximum capacity of the edge
    int flow;          // Current flow through the edge
    int reverse_edge;  // Index of the reverse edge in the 'to' node's adjacency list
};

void add_edge(vector<vector<Edge>>& adj, int u, int v, int capacity) {
    adj[u].emplace_back(v, capacity, 0, (int)adj[v].size());
    adj[v].emplace_back(u, 0, 0, (int)adj[u].size() - 1);
}

int bfs(int s, int t, span<int> parent_node, span<int> parent_edge_index, span<vector<Edge>> adj) {
    fill(parent_node.begin(), parent_node.end(), -1);
    queue<pair<int, int>> q;
    q.emplace(s, INF);
    parent_node[s] = s;

    while (!q.empty()) {
        auto [u, f] = q.front();
        q.pop();

        for (size_t i = 0; i < adj[u].size(); ++i) {
            const Edge& edge = adj[u][i];
            int v = edge.to;

            if (parent_node[v] == -1 && edge.capacity - edge.flow > 0) {
                parent_node[v] = u;
                parent_edge_index[v] = i;
                int new_flow = min(f, edge.capacity - edge.flow);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }

    return 0;
}

int64_t maxflow(int s, int t, size_t n, span<vector<Edge>> adj) {
    vector<int> parent_node(n);
    vector<int> parent_edge_index(n);

    int64_t total_flow = 0;
    int64_t new_flow;

    while ((new_flow = bfs(s, t, parent_node, parent_edge_index, adj))) {
        total_flow += new_flow;

        int cur = t;
        while (cur != s) {
            int prev = parent_node[cur];
            int edge_idx = parent_edge_index[cur];

            Edge& forward_edge = adj[prev][edge_idx];
            forward_edge.flow += new_flow;

            int reverse_edge_idx = forward_edge.reverse_edge;
            Edge& reverse_edge = adj[cur][reverse_edge_idx];
            reverse_edge.flow -= new_flow;

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
        auto already_lost = limits[i] < 0;
        if (already_lost) {
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

    return 0;
}
