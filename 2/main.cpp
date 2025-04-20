#include <mimalloc-new-delete.h>

#include <iostream>
#include <random>
#include <vector>

#include "ford_fulkerson.hpp"

using namespace std;

namespace {
void fill_graph(
    size_t vertices,
    mdspan<capacity, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> graph,
    capacity max_capacity, double prob) {
    thread_local mt19937_64 rng(0);

    uniform_int_distribution<capacity> capacity_dist(1, max_capacity);
    uniform_real_distribution prob_dist(0.0, 1.0);

    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = 0; j < vertices; ++j) {
            if (i != j && prob_dist(rng) <= prob) {
                graph[i, j] = capacity_dist(rng);
            }
        }
    }
}
}

int main() {
    for (size_t vertice = 1'000; vertice <= 10'000; vertice += 1'000) {
        uint32_t s = 0;
        uint32_t t = vertice - 1;

        vector<capacity> raw_graph(vertice * vertice);
        mdspan graph(raw_graph.data(), vertice, vertice);

        fill_graph(vertice, graph, 100, 0.03);

        cout << "Finished graph\n";

        adjagency_matrix::FordFulkersonDFS instanceDFS(raw_graph, vertice);

        cout << "DFS: " << instanceDFS.max_flow(s, t) << '\n';

        adjagency_matrix::FordFulkersonBFS instanceBFS(raw_graph, vertice);

        cout << "BFS: " << instanceBFS.max_flow(s, t) << '\n';

        adjagency_matrix::FordFulkersonFattestPath instanceFattestPath(raw_graph, vertice);

        cout << "Fattest path: " << instanceFattestPath.max_flow(s, t) << '\n';
    }
}
