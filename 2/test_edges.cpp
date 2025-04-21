#include <mimalloc-new-delete.h>
#include <chrono>

#include <iostream>
#include <random>
#include <syncstream>
#include <vector>

#include "ford_fulkerson.hpp"

using namespace std;

int main() {
    constexpr int EDGE_SET_AMOUNT = 20;
    constexpr size_t REPETITIONS = 3;
    array vertices = {10000};

    cout << "vertices,edges,max_flow,dfs vertices,dfs edges,dfs time (ms),bfs vertices,bfs "
            "edges,bfs time (ms),fat vertices,fat edges,fat time (ms)\n";

#pragma omp parallel for collapse(2)
    for (int i = 0; i < vertices.size(); ++i) {
        for (int j = 0; j <= EDGE_SET_AMOUNT; ++j) {
            const size_t vertice = vertices[i];
            const uint32_t s = 0;
            const uint32_t t = vertice - 1;

            vector<capacity> raw_graph(vertice * vertice);
            const mdspan graph(raw_graph.data(), vertice, vertice);

            const size_t max_edges = vertice * (vertice - 1);

            const size_t expected_edges = ((max_edges - vertice) / EDGE_SET_AMOUNT) * j + vertice;
            const double edge_prob =
                static_cast<double>(expected_edges) / static_cast<double>(max_edges);

            for (size_t k = 0; k < REPETITIONS; ++k) {
                adjacency_matrix::fill_graph(vertice, graph, 127, edge_prob);

                auto dfs_start = chrono::high_resolution_clock::now();

                adjacency_matrix::FordFulkersonDFS dfs_instance(raw_graph, vertice);

                auto max_flow = dfs_instance.max_flow(s, t);

                auto bfs_start = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> dfs_duration = bfs_start - dfs_start;

                adjacency_matrix::FordFulkersonBFS bfs_instance(raw_graph, vertice);

                bfs_instance.max_flow(s, t);

                auto fat_start = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> bfs_duration = fat_start - bfs_start;

                adjacency_matrix::FordFulkersonFattestPath fattest_path_instance(raw_graph,
                                                                                 vertice);

                fattest_path_instance.max_flow(s, t);

                auto fat_end = chrono::high_resolution_clock::now();

                chrono::duration<double, milli> fat_duration = fat_end - fat_start;

                osyncstream so(cout);

                so << vertice << ',' << expected_edges << ',' << max_flow << ','
                   << dfs_instance.touched_vertices << ',' << dfs_instance.touched_edges << ','
                   << dfs_duration.count() << ',' << bfs_instance.touched_vertices << ','
                   << bfs_instance.touched_edges << ',' << bfs_duration.count() << ','
                   << fattest_path_instance.touched_vertices << ','
                   << fattest_path_instance.touched_edges << ',' << fat_duration.count() << '\n';

                fill(raw_graph.begin(), raw_graph.end(), 0);
            }
        }
    }
}
