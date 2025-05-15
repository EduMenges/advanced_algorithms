#if __has_include(<mimalloc-new-delete.h>)
#include <mimalloc-new-delete.h>
#endif

#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

using Weight = int32_t;

vector<Weight> hungarian(const vector<vector<Weight>>& C) {
    const auto J = C.size();
    const auto W = C[0].size();
    assert(J <= W);

    // job[w] -> Associates worker `w` with job[w]
    // job[w] == -1 indicates the worker is free
    // job[W] -> Dummy worker
    vector<int> job(W + 1, -1);

    // Potentials per worker
    vector<Weight> h(W, 0);

    // Cumulative optimal costs
    vector<Weight> answers;
    Weight ans = 0;

    constexpr auto INF = numeric_limits<Weight>::max();

    for (int j_cur = 0; j_cur < J; ++j_cur) {
        int w_source = W;
        job[w_source] = j_cur;

        // Johnson-reduced distances
        vector<Weight> dist(W + 1, INF);
        dist[w_source] = 0;

        vector<bool> visited(W + 1, false);
        vector<int> prev(W + 1, -1);

        priority_queue<pair<Weight, int>, vector<pair<Weight, int>>, greater<pair<Weight, int>>> pq;
        pq.emplace(0, w_source);

        // Stop at the first free worker
        int w_free = -1;

        while (!pq.empty()) {
            auto [du, u] = pq.top();
            pq.pop();

            if (visited[u] || du != dist[u]) {
                continue;
            }

            visited[u] = true;

            if (bool reached_unmatched_worker = job[u] == -1; reached_unmatched_worker) {
                w_free = u;
                break;
            }

            // Relax edges: u (worker) -> v (other workers)
            for (int v = 0; v < W; ++v) {
                if (visited[v]) {
                    continue;
                }

                Weight edge = C[job[u]][v] - h[v];
                if (u != W) {
                    edge -= C[job[u]][u] - h[u];
                }

                if (edge < 0) {
                    edge = 0;
                }

                if (dist[u] + edge < dist[v]) {
                    dist[v] = dist[u] + edge;
                    prev[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }

        // Now `w_cur` is a free worker and dist[w_cur] is the length of the best augmenting path

        // Update potentials
        const auto delta = dist[w_free];
        for (int w = 0; w < W; ++w) {
            if (dist[w] > delta) {
                dist[w] = delta;
            }
            h[w] += dist[w];
        }

        ans += h[w_free];

        for (int w; w_free != W; w_free = w) {
            job[w_free] = job[w = prev[w_free]];
        }

        answers.push_back(ans);
    }

    return answers;
}

int main() {
    vector<vector<int>> costs{{8, 5, 9}, {4, 2, 4}, {7, 3, 8}};
    assert((hungarian(costs) == vector<int>{5, 9, 15}));

    return 0;
}
