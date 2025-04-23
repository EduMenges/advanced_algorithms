#include <sstream>
#include <iostream>
#include <cstdint>

#include "ford_fulkerson.hpp"

int main() {

    string str;

    while (std::getline(cin, str)) {
        if (str[0] == 'c') {
            continue;
        }

        if (str[0] == 'p') {
            break;
        }
    }

    size_t vertices;
    vector<capacity> raw_graph;

    istringstream ss(str);
    string buf;
    getline(ss, buf, ' ');
    getline(ss, buf, ' ');
    ss >> vertices;
    raw_graph.resize(vertices * vertices);
    mdspan graph(raw_graph.data(), vertices, vertices);

    uint32_t s;
    uint32_t t;

    while (std::getline(cin, str)) {
        if (str[0] == 'c') {
            continue;
        }

        istringstream ss(str);
        if (str[0] == 'n') {
            getline(ss, buf, ' ');
            uint32_t val;
            ss >> val;
            getline(ss, buf);
            if (buf[1] == 's') {
                s = val;
            } else if (buf[1] == 't') {
                t = val;
            }
        } else if (str[0] == 'a') [[likely]] {
            getline(ss, buf, ' ');
            uint32_t from, to, capacity;
            ss >> from >> to >> capacity;
            graph[from, to] = capacity;
        }
    }

    adjacency_matrix::FordFulkersonFattestPath ff(std::move(raw_graph), vertices);
    cout << ff.max_flow(s, t) << '\n';
}