#include <mimalloc-new-delete.h>
#include <chrono>

#include <iostream>
#include <random>
#include <syncstream>
#include <vector>

#include "ford_fulkerson.hpp"

using namespace std;

int main() {
    constexpr size_t VERTICE = 5'000;
    constexpr size_t CAPACITY_AMOUNT = 10;
    constexpr int REPETITIONS = 3;
    vector max_capacities = {128};

    for (size_t i = 1; i < CAPACITY_AMOUNT; ++i) {
        max_capacities.push_back(max_capacities[0] * (i + 1));
    }

    cout << "max_flow,dfs vertices,dfs edges,dfs time (ms),bfs vertices,bfs "
            "edges,bfs time (ms),fat vertices,fat edges,fat time (ms)\n";

#pragma omp parallel for collapse(2)
    for (int i = 0; i < max_capacities.size(); ++i) {
        for (int j = 0; j < REPETITIONS; ++j) {
            double max_capacity = max_capacities[i];
            const uint32_t s = 0;
            const uint32_t t = VERTICE - 1;

            vector<capacity> raw_graph(VERTICE * VERTICE);
            const mdspan graph(raw_graph.data(), VERTICE, VERTICE);

            constexpr double edge_prob = 0.5;
            constexpr size_t max_edges = VERTICE * (VERTICE - 1);

            adjacency_matrix::fill_graph(VERTICE, graph, max_capacity, edge_prob);

            auto dfs_start = chrono::high_resolution_clock::now();

            adjacency_matrix::FordFulkersonDFS dfs_instance(raw_graph, VERTICE);

            auto max_flow = dfs_instance.max_flow(s, t);

            auto bfs_start = chrono::high_resolution_clock::now();
            chrono::duration dfs_duration = bfs_start - dfs_start;

            adjacency_matrix::FordFulkersonBFS bfs_instance(raw_graph, VERTICE);

            bfs_instance.max_flow(s, t);

            auto fat_start = chrono::high_resolution_clock::now();
            chrono::duration bfs_duration = fat_start - bfs_start;

            adjacency_matrix::FordFulkersonFattestPath fattest_path_instance(raw_graph, VERTICE);

            fattest_path_instance.max_flow(s, t);

            auto fat_end = chrono::high_resolution_clock::now();

            chrono::duration fat_duration = fat_end - fat_start;

            osyncstream so(cout);

            so << VERTICE << ',' << max_flow << ',' << dfs_instance.touched_vertices << ','
               << dfs_instance.touched_edges << ',' << dfs_duration.count() << ','
               << bfs_instance.touched_vertices << ',' << bfs_instance.touched_edges << ','
               << bfs_duration.count() << ',' << fattest_path_instance.touched_vertices << ','
               << fattest_path_instance.touched_edges << ',' << fat_duration.count() << '\n';
        }
    }
}
