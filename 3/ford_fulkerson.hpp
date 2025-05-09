#pragma once

#include <queue>
#include <stack>
#include <vector>

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

using namespace std;

using capacity = int16_t;

namespace adjacency_matrix {

using Graph = mdspan<capacity, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>>;

void fill_graph(size_t vertices, Graph graph, capacity max_capacity, double prob);

void fill_graph_with_fixed_max_flow(size_t vertices,
                                    Graph graph,
                                    capacity max_capacity,
                                    double prob);

class FordFulkerson {
   public:
    constexpr FordFulkerson(vector<capacity> raw_graph, size_t vertices)
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
            capacity path_flow = numeric_limits<capacity>::max();

            uint32_t u;

            for (size_t v = t; v != s; v = parent[v]) {
                touched_edges += 1;
                u = parent[v];
                path_flow = min(path_flow, graph[u, v]);
            }

            for (size_t v = t; v != s; v = parent[v]) {
                touched_edges += 1;
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
    Graph graph;
    vector<uint32_t> parent;
    vector<uint8_t> visited;
    uint64_t touched_vertices = 1;
    uint64_t touched_edges = 1;
};

class FordFulkersonFattestPath final : public FordFulkerson {
   public:
    FordFulkersonFattestPath(vector<capacity> raw_graph, size_t vertices)
        : FordFulkerson(std::move(raw_graph), vertices) {}

    bool find_path(uint32_t s, uint32_t t) override {
        vector<capacity> max_capacity(vertices, 0);
        max_capacity[s] = numeric_limits<capacity>::max();

        auto cmp = [](const pair<capacity, uint32_t>& l,
                      const pair<capacity, uint32_t>& r) -> bool { return r.first > l.first; };
        priority_queue<pair<capacity, uint32_t>, vector<pair<capacity, uint32_t>>, decltype(cmp)>
            pq(cmp);
        pq.emplace(max_capacity[s], s);

        while (!pq.empty()) {
            auto [cap, u] = pq.top();
            pq.pop();

            visited[u] = true;

            if (u == t) {
                return true;
            }

            for (uint32_t v = 0; v < vertices; ++v) {
                if (!visited[v]) {
                    touched_vertices += 1;
                    if (graph[u, v] > 0) {
                        touched_edges += 1;

                        if (auto new_capacity = min(cap, graph[u, v]);
                            new_capacity > max_capacity[v]) {
                            max_capacity[v] = new_capacity;
                            parent[v] = u;
                            pq.emplace(new_capacity, v);
                        }
                    }
                }
            }
        }

        return false;
    }
};
}
