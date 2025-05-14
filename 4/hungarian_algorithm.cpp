#if __has_include(<mimalloc-new-delete.h>)
#include <mimalloc-new-delete.h>
#endif

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using Weight = int32_t;

constexpr int INF = numeric_limits<Weight>::max();

pair<Weight, vector<int>> hungarian(const vector<vector<Weight>>& cost) {
    auto n = cost.size();
    vector<int> u(n, 0); // Labels do U
    vector<int> v(n, 0); // Labels do V
    vector<int> match_left(n, -1);
    vector<int> match_right(n, -1);

    for (int i = 0; i < n; ++i) {
        vector<int> effective_costs(n, INF);
        vector<int> predecessors(n, -1);
        vector used(n, false);

        int marked_i = i;
        int marked_j = -1;
        int j;

        while (true) {
            j = -1;
            for (int k = 0; k < n; ++k) {
                if (!used[k]) {
                    auto effective_cost = cost[marked_i][k] - u[marked_i] - v[k];
                    if (effective_cost < effective_costs[k]) {
                        effective_costs[k] = effective_cost;
                        predecessors[k] = marked_j;
                    }
                    if (j == -1 || effective_costs[k] < effective_costs[j])
                        j = k;
                }
            }

            int delta = effective_costs[j];
            for (int k = 0; k < n; ++k) {
                if (used[k]) {
                    u[match_right[k]] += delta;
                    v[k] -= delta;
                } else {
                    effective_costs[k] -= delta;
                }
            }
            u[i] += delta;

            used[j] = true;
            marked_j = j;
            if (match_right[j] == -1) {
                break;
            }
            marked_i = match_right[j];
        }

        while (predecessors[j] != -1) {
            match_right[j] = match_right[predecessors[j]];
            j = predecessors[j];
        }
        match_right[j] = i;
    }

    // Monta match_left
    int total_weight = 0;
    for (int j = 0; j < n; ++j) {
        if (match_right[j] != -1) {
            match_left[match_right[j]] = j;
            total_weight += cost[match_right[j]][j];
        }
    }

    return {total_weight, match_left};
}

int main() {
    vector<vector<Weight>> weight = {{10, 19, 8}, {10, 18, 7}, {5, 7, 9}};

    auto [max_weight, match] = hungarian(weight);

    cout << "Emparelhamento ótimo (máximo peso):\n";
    for (int i = 0; i < match.size(); ++i) {
        cout << "Vértice " << i << " emparelhado com " << match[i]
             << " (peso: " << weight[i][match[i]] << ")\n";
    }
    cout << "Peso total do emparelhamento: " << max_weight << endl;

    return 0;
}
