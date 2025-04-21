#include "ford_fulkerson.hpp"
#include <random>

void adjacency_matrix::fill_graph(
size_t vertices,
mdspan<capacity, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> graph,
capacity max_capacity,
double prob) {
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