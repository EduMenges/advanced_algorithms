#include <mimalloc-new-delete.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <syncstream>
#include <thread>
#include <vector>

#include "dijkstra.hpp"

using namespace std;

void fill_edges(Graph& graph, uint32_t num_nodes, uint64_t num_edges, uint64_t current_edges) {
    const constexpr uint16_t MIN_WEIGHT = 1;
    const constexpr uint16_t MAX_WEIGHT = 1000;

    thread_local random_device rd;
    thread_local mt19937 gen(rd());

    uniform_int_distribution<uint32_t> node_dist(0, num_nodes - 1);
    uniform_int_distribution<uint16_t> weight_dist(MIN_WEIGHT, MAX_WEIGHT);

    for (auto& node : graph) {
        if (node.size() < num_edges / num_nodes) {
            node.reserve(num_edges / num_nodes - node.size());
        }
    }

    while (current_edges < num_edges) {
        auto u = node_dist(gen);
        auto v = node_dist(gen);

        if (u != v) {
            auto weight = weight_dist(gen);

            bool exists = false;
            for (const auto& neighbor : graph[u]) {
                if (neighbor.first == v) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                graph[u].emplace_back(v, weight);
                graph[v].emplace_back(u, weight);
                current_edges += 1;
            }
        }
    }
}

constexpr size_t DIJKSTRA_AMOUNT = 15;

void run(const Graph& graph, uint32_t n, uint64_t m) {
    const constexpr size_t NODE_AMOUNT = 10;

    thread_local random_device rd;
    thread_local mt19937 gen(rd());

    uniform_int_distribution<uint32_t> dist(0, n - 1);

    array<pair<uint32_t, uint32_t>, NODE_AMOUNT> nodes;

    for (auto i = 0; i < NODE_AMOUNT; ++i) {
        auto origin = dist(gen);
        auto dest = dist(gen);

        while (dest == origin) {
            dest = dist(gen);
        }

        nodes[i] = {origin, dest};
    }

    osyncstream synced_out(cout);
    synced_out << n << ',' << m << ',';

    array<pair<chrono::duration<double, std::milli>, Result>, DIJKSTRA_AMOUNT> total_results{};

    constexpr array kary_dijkstras = {dijkstra<2>,  dijkstra<3>,  dijkstra<4>,  dijkstra<5>,
                                      dijkstra<6>,  dijkstra<7>,  dijkstra<8>,  dijkstra<9>,
                                      dijkstra<10>, dijkstra<11>, dijkstra<12>, dijkstra<13>,
                                      dijkstra<14>, dijkstra<15>, dijkstra<16>};

    const constexpr int RUNS = 10;

    for (auto [origin, dest] : nodes) {
        for (int _ = 1; _ <= RUNS; ++_) {
            for (size_t f = 0; f < kary_dijkstras.size(); ++f) {
                auto start = chrono::high_resolution_clock ::now();

                auto result = kary_dijkstras[f](graph, origin, dest);

                auto end = chrono::high_resolution_clock ::now();

                chrono::duration<double, std::milli> elapsed = end - start;

                auto& [time, current_result] = total_results[f];

                time += elapsed;
                current_result.push += result.push;
                current_result.pop += result.pop;
                current_result.update += result.update;
                current_result.sift_up += result.sift_down;
                current_result.sift_down += result.sift_up;
            }
        }
    }

    const constexpr int SAMPLE_AMOUNT = RUNS * NODE_AMOUNT;

    for (size_t i = 0; i < total_results.size(); ++i) {
        auto& [time, result] = total_results[i];

        time /= SAMPLE_AMOUNT;
        result.push /= SAMPLE_AMOUNT;
        result.pop /= SAMPLE_AMOUNT;
        result.update /= SAMPLE_AMOUNT;
        result.sift_up /= SAMPLE_AMOUNT;
        result.sift_down /= SAMPLE_AMOUNT;

        synced_out << result.push << ',' << result.pop << ',' << result.update << ','
                   << result.sift_up << ',' << result.sift_down << ',' << time.count();

        if (i != total_results.size() - 1) {
            synced_out << ',';
        }
    }

    synced_out << '\n';
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: test_dijkstra <max_nodes>\n";
        return EXIT_FAILURE;
    }

    uint64_t max_nodes = stoull(argv[1]);

    cout << "n;m;";

    for (size_t i = 2; i < 2 + DIJKSTRA_AMOUNT; ++i) {
        cout << "K=" << i << " pu,K=" << i << " po,K=" << i << " up,K=" << i << " su,K=" << i
             << " sd,K=" << i << " t(ms),";
    }

    cout << '\n';

    vector<uint64_t> node_values{1'000};

    while (node_values.back() * 2 <= max_nodes) {
        node_values.push_back(node_values.back() * 2);
    }

#pragma omp parallel for
    for (int32_t i_n = 0; i_n < node_values.size(); ++i_n) {
        auto n = static_cast<uint64_t>(node_values[i_n]);
        Graph graph(n);
        uint64_t m_limit = (((n * (n - 1)) / 2) * 90) / 100;
        uint64_t prev_m = 0;

        for (uint64_t m = n * 2; m <= m_limit; m *= 2) {
            fill_edges(graph, n, m, prev_m);
            run(graph, n, m);
            prev_m = m;
        }
    }

    return EXIT_SUCCESS;
}
