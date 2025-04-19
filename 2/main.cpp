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

    vector<int16_t> raw_graph(vertices * vertices);
    mdspan graph(raw_graph.data(), vertices, vertices);

    thread_local mt19937_64 rng(random_device().operator()());

    uniform_int_distribution<int16_t> capacity_dist(1, numeric_limits<int16_t>::max());
    uniform_real_distribution<double> prob_dist(0.0, 1.0);
    constexpr double EDGE_PROB = 0.03;

    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = i + 1; j < vertices; ++j) {
            if (prob_dist(rng) <= EDGE_PROB) {
                graph[i,j] = capacity_dist(rng);
            }
        }
    }

    cout << "Finished graph\n";

    FordFulkersonAMDFS instanceDFS(raw_graph, vertices);

    cout << "DFS: " << instanceDFS.max_flow(s, t) << '\n';

    FordFulkersonAMBFS instanceBFS(raw_graph, vertices);

    cout << "BFS: " << instanceBFS.max_flow(s, t) << '\n';

    FordFulkersonAMFattestPath instanceFattestPath(raw_graph, vertices);

    cout << "Fattest path: " << instanceFattestPath.max_flow(s, t) << '\n';
}
