#include <mimalloc-new-delete.h>

#include <iostream>
#include <random>
#include <vector>

#include "ford_fulkerson.hpp"

using namespace std;

int main() {
    size_t vertices = 10'000;
    uint32_t s = 0;
    uint32_t t = vertices - 1;

    vector<capacity> raw_graph(vertices * vertices);
    mdspan graph(raw_graph.data(), vertices, vertices);

    thread_local mt19937_64 rng(0);

    uniform_int_distribution<capacity> capacity_dist(1, numeric_limits<int8_t>::max());
    uniform_real_distribution<double> prob_dist(0.0, 1.0);
    constexpr double EDGE_PROB = 0.003;
    size_t edges = 0;

    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = 0; j < vertices; ++j) {
            if (i != j && prob_dist(rng) <= EDGE_PROB) {
                graph[i,j] = capacity_dist(rng);
                edges += 1;
            }
        }
    }

    cout << "Finished graph: " << edges << " edges\n";

    adjagency_matrix::FordFulkersonDFS instanceDFS(raw_graph, vertices);

    cout << "DFS: " << instanceDFS.max_flow(s, t) << '\n';

    adjagency_matrix::FordFulkersonBFS instanceBFS(raw_graph, vertices);

    cout << "BFS: " << instanceBFS.max_flow(s, t) << '\n';

    adjagency_matrix::FordFulkersonFattestPath instanceFattestPath(raw_graph, vertices);

    cout << "Fattest path: " << instanceFattestPath.max_flow(s, t) << '\n';
}
