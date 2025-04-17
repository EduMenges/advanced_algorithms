#include <iostream>
#include <random>
#include <vector>

#include "FF_DFS.hpp"
#include "mimalloc-new-delete.h"

using namespace std;

int main() {
    size_t vertices = 6;
    uint32_t s = 0;
    uint32_t t = vertices - 1;

    vector<int16_t> raw_graph = {0, 5, 15, 0, 0, 0,   //
                                 0, 0, 0,  5, 5, 0,   //
                                 0, 0, 0,  5, 5, 0,   //
                                 0, 0, 0,  0, 0, 15,  //
                                 0, 0, 0,  0, 0, 5,   //
                                 0, 0, 0,  0, 0, 0};
    mdspan graph(raw_graph.data(), vertices, vertices);

    // thread_local mt19937 rng((random_device()).operator()());
    //
    // uniform_int_distribution<uint16_t> capacity_dist(1, numeric_limits<uint16_t>::max());
    // uniform_real_distribution<double> prob_dist(0.0, 1.0);
    // constexpr double edge_prob = 0.3;
    //
    // for (size_t i = 0; i < vertices; ++i) {
    //     for (size_t j = 0; j < vertices; ++j) {
    //         if (i != j && edge_prob <= prob_dist(rng)) {
    //             graph[i,j] = capacity_dist(rng);
    //         }
    //     }
    // }

    FordFulkersonDFS instance(raw_graph, vertices);

    cout << instance.max_flow(s, t) << '\n';
}
