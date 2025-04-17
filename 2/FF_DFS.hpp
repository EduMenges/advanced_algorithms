#pragma once

#include <mdspan>
#include <vector>

using namespace std;

class FordFulkersonDFS {
   public:
    FordFulkersonDFS(vector<int16_t> raw_graph, size_t vertices)
        : raw_graph(move(raw_graph)),
          vertices(vertices),
          graph(this->raw_graph.data(), vertices, vertices),
          parent(vertices, numeric_limits<uint16_t>::max()),
          visited(vertices, false) {}

    bool dfs(uint32_t u, uint32_t t) {
        if (u == t) {
            return true;
        }

        visited[u] = true;

        for (auto v = 0; v < vertices; ++v) {
            if (!visited[v] && graph[u, v] > 0) {
                parent[v] = u;

                if (dfs(v, t)) {
                    return true;
                }
            }
        }

        return false;
    }

    int64_t max_flow(uint32_t s, uint32_t t) {
        parent[s] = s;

        int64_t max_flow = 0;

        while (true) {
            fill(visited.begin(), visited.end(), false);

            if (!dfs(s, t)) {
                break;
            }

            auto path_flow = numeric_limits<int16_t>::max();
            uint32_t u;

            for (size_t v = t; v != s; v = parent[v]) {
                u = parent[v];
                path_flow = min(path_flow, graph[u, v]);
            }

            for (size_t v = t; v != s; v = parent[v]) {
                u = parent[v];
                graph[u, v] -= path_flow;
                graph[v, u] += path_flow;
            }

            max_flow += static_cast<int64_t>(path_flow);
        }

        return max_flow;
    }

    vector<int16_t> raw_graph;
    size_t vertices;
    mdspan<int16_t, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> graph;
    vector<uint32_t> parent;
    vector<bool> visited;
};
