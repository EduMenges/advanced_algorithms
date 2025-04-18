#pragma once

#include <optional>
#include <queue>
#include <vector>

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "../util/mdspan.hpp"
#endif

using namespace std;

using capacity = int16_t;

class FordFulkerson {
   public:
    FordFulkerson(vector<capacity> raw_graph, size_t vertices)
        : raw_graph(std::move(raw_graph)),
          vertices(vertices),
          graph(this->raw_graph.data(), vertices, vertices),
          parent(vertices),
          visited(vertices, false) {}

    virtual ~FordFulkerson() = default;

    int64_t max_flow(uint32_t s, uint32_t t) {
        parent[s] = s;

        int64_t max_flow = 0;

        while (find_path(s, t)) {
            auto path_flow = numeric_limits<capacity>::max();

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

            fill(visited.begin(), visited.end(), false);
        }

        return max_flow;
    }

    virtual bool find_path(uint32_t s, uint32_t t) = 0;

    vector<capacity> raw_graph;
    size_t vertices;
    mdspan<capacity, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> graph;
    vector<uint32_t> parent;
    vector<bool> visited;
};

class FordFulkersonBFS final : public FordFulkerson {
   public:
    FordFulkersonBFS(vector<capacity> raw_graph, size_t vertices)
        : FordFulkerson(std::move(raw_graph), vertices) {}

    bool find_path(uint32_t s, uint32_t t) override {
        queue<uint32_t> q;

        q.push(s);
        visited[s] = true;

        while (!q.empty()) {
            auto u = q.front();
            q.pop();

            if (u == t) {
                return true;
            }

            for (uint32_t v = 0; v < vertices; ++v) {
                if (!visited[v] && graph[u, v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return false;
    }
};

class FordFulkersonDFS final : public FordFulkerson {
   public:
    FordFulkersonDFS(vector<capacity> raw_graph, size_t vertices)
        : FordFulkerson(std::move(raw_graph), vertices) {}

    bool find_path(uint32_t u, uint32_t t) override {
        if (u == t) {
            return true;
        }

        visited[u] = true;

        for (auto v = 0; v < vertices; ++v) {
            if (!visited[v] && graph[u, v] > 0) {
                parent[v] = u;

                if (find_path(v, t)) {
                    return true;
                }
            }
        }

        return false;
    }
};

class FordFulkersonFattestPath final : public FordFulkerson {
   public:
    FordFulkersonFattestPath(vector<capacity> raw_graph, size_t vertices)
        : FordFulkerson(std::move(raw_graph), vertices) {}

    bool find_path(uint32_t s, uint32_t t) override {
        vector<capacity> max_capacity(vertices, 0);
        max_capacity[s] = numeric_limits<capacity>::max();

        for (uint32_t i = 0; i < vertices; ++i) {
            optional<uint32_t> u = nullopt;

            capacity max_cap = 0;
            for (uint32_t j = 0; j < vertices; ++j) {
                if (!visited[j] && max_capacity[j] > max_cap) {
                    max_cap = max_capacity[j];
                    u = j;
                }
            }

            if (!u.has_value() || *u == t) {
                break;
            }

            visited[*u] = true;

            for (uint32_t v = 0; v < vertices; ++v) {
                if (!visited[v] && graph[*u, v] > 0) {
                    if (auto new_capacity = min(max_capacity[*u], graph[*u, v]);
                        new_capacity > max_capacity[v]) {
                        max_capacity[v] = new_capacity;
                        parent[v] = *u;
                    }
                }
            }
        }

        return max_capacity[t] > 0;
    }
};
