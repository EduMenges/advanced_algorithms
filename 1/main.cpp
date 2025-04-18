#include "dijkstra.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: dijkstra <origin> <destination>\n";
        return EXIT_FAILURE;
    }

    uint32_t origin = stoull(argv[1]);
    uint32_t destination = stoull(argv[2]);

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



    auto result = dijkstra<2>(graph, origin, destination);

    return EXIT_SUCCESS;
}