#include "mimalloc-new-delete.h"
#include "dijkstra.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace std;

int main() {
    std::string s;

    uint32_t n;

    vector<vector<pair<uint32_t, uint16_t>>> graph;

    while (getline(cin, s)) {
        std::string temp;
        std::istringstream iss(s);

        if (s[0] == 'p') {
            iss >> temp >> temp >> n;
            graph.resize(n + 1);
        } else if (s[0] == 'a') [[likely]] {
            uint32_t o, d;
            uint16_t w;

            iss >> temp >> o >> d >> w;

            graph[o].emplace_back(d, w);
        }
    }

    const constexpr size_t SAMPLES = 50;

    thread_local mt19937 rng(random_device().operator()());
    uniform_int_distribution<uint32_t> dist(1, n + 1);

    chrono::duration<double, std::milli> total_duration{};

    for (size_t i = 0; i < SAMPLES; ++i) {
        auto u = dist(rng);
        auto v = dist(rng);

        while (v == u) {
            v = dist(rng);
        }

        auto start = chrono::high_resolution_clock ::now();

        auto result = dijkstra<4>(graph, u, v);

        auto end = chrono::high_resolution_clock ::now();

        auto elapsed = end - start;

        total_duration += elapsed;
    }

    chrono::duration<double, std::milli> avg_duration = total_duration / SAMPLES;

    cout << "Average duration: " << avg_duration << '\n';

    return EXIT_SUCCESS;
}