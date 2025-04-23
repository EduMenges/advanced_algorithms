#include <mimalloc-new-delete.h>
#include <chrono>

#include <iostream>
#include <syncstream>
#include <vector>

#include "ford_fulkerson.hpp"

using namespace std;

constexpr int CAPACITY_AMOUNT = 20;
constexpr capacity MAX_CAPACITY = numeric_limits<capacity>::max() / (CAPACITY_AMOUNT - 1);

namespace {
void increase_capacity(adjacency_matrix::Graph graph, int a) {
    if (a == 0) {
        return;
    }

    size_t vertices = graph.extent(0);
    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = i + 1; j < vertices; ++j) {
            if (graph[i, j] > 0) {
                graph[i, j] += MAX_CAPACITY * a;
            }
        }
    }
}
}

int main() {
    constexpr size_t VERTEX = 10'000;
    constexpr int REPETITIONS = 3;

    cout << "max_flow,dfs vertices,dfs edges,dfs time (ms),bfs vertices,bfs "
            "edges,bfs time (ms),fat vertices,fat edges,fat time (ms)\n";

    vector<capacity> raw_graph(VERTEX * VERTEX);
    const mdspan graph(raw_graph.data(), VERTEX, VERTEX);

    constexpr double edge_prob = 0.5;

    adjacency_matrix::fill_graph(VERTEX, graph, MAX_CAPACITY, edge_prob);

    for (size_t i = 0; i < (VERTEX - 1); ++i) {
        graph[i, i + 1] = MAX_CAPACITY;
    }

#pragma omp parallel for
    for (int i = 0; i < CAPACITY_AMOUNT; ++i) {
        vector<capacity> raw_graph_i(raw_graph);
        const mdspan graph_i(raw_graph_i.data(), VERTEX, VERTEX);

        increase_capacity(graph_i, i);

        for (int j = 0; j < REPETITIONS; ++j) {
            const uint32_t s = 0;
            const uint32_t t = VERTEX - 1;

            auto dfs_start = chrono::high_resolution_clock::now();

            adjacency_matrix::FordFulkersonDFS dfs_instance(raw_graph_i, VERTEX);

            auto max_flow = dfs_instance.max_flow(s, t);

            auto bfs_start = chrono::high_resolution_clock::now();
            chrono::duration dfs_duration = bfs_start - dfs_start;

            adjacency_matrix::FordFulkersonBFS bfs_instance(raw_graph_i, VERTEX);

            bfs_instance.max_flow(s, t);

            auto fat_start = chrono::high_resolution_clock::now();
            chrono::duration bfs_duration = fat_start - bfs_start;

            adjacency_matrix::FordFulkersonFattestPath fattest_path_instance(raw_graph_i, VERTEX);

            fattest_path_instance.max_flow(s, t);

            auto fat_end = chrono::high_resolution_clock::now();

            chrono::duration fat_duration = fat_end - fat_start;

            osyncstream so(cout);

            so << max_flow << ',' << dfs_instance.touched_vertices << ','
               << dfs_instance.touched_edges << ',' << dfs_duration.count() << ','
               << bfs_instance.touched_vertices << ',' << bfs_instance.touched_edges << ','
               << bfs_duration.count() << ',' << fattest_path_instance.touched_vertices << ','
               << fattest_path_instance.touched_edges << ',' << fat_duration.count() << '\n';
        }
    }
}
