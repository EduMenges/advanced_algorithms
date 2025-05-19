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

vector<int64_t> hungarian(const mdspan<Weight, dextents<size_t, 2>> C) {
    constexpr auto INF = numeric_limits<Weight>::max();

    const auto J = C.extent(0);
    const auto W = C.extent(1);
    assert(J == W);

    const auto dummy_worker = W;

    vector<int32_t> worker_to_job(W + 1, -1);

    // Potentials per worker
    vector<Weight> h(W, 0);

    // Cumulative optimal costs
    vector<int64_t> answers;
    answers.reserve(J);

    int64_t current_cost = 0;

    for (int32_t j_cur = 0; j_cur < J; ++j_cur) {
        worker_to_job[dummy_worker] = j_cur;
        auto w_source = dummy_worker;

        // Johnson-reduced distances
        vector<Weight> dist(W + 1, INF);
        dist[w_source] = 0;

        vector<bool> visited(W + 1, false);
        vector<int32_t> prev(W + 1, dummy_worker);

        priority_queue<pair<int32_t, size_t>, vector<pair<int32_t, size_t>>,
                       greater<pair<int32_t, size_t>>>
            pq;

        // Connect every job to the dummy worker
        for (size_t v = 0; v < W; ++v) {
            auto edge = C[j_cur, v] - h[v];
            dist[v] = edge;
            pq.emplace(edge, v);
        }

        // Stop at the first free worker
        int w_free = -1;

        while (!pq.empty()) {
            auto [du, worker] = pq.top();
            pq.pop();

            if (visited[worker] || du < dist[worker]) {
                continue;
            }

            visited[worker] = true;

            auto associated_job = worker_to_job[worker];
            if (bool reached_unmatched_worker = associated_job == -1; reached_unmatched_worker) {
                w_free = worker;
                break;
            }

            // Relax edges: worker -> other workers
            auto relax = C[associated_job, worker] - h[worker];

            for (int v = 0; v < W; ++v) {
                if (visited[v]) {
                    continue;
                }

                auto edge = C[associated_job, v] - h[v] - relax;

                if (dist[worker] + edge < dist[v]) {
                    dist[v] = dist[worker] + edge;
                    prev[v] = worker;
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

        current_cost += static_cast<int64_t>(h[w_free]);

        for (size_t w; w_free != dummy_worker; w_free = w) {
            worker_to_job[w_free] = worker_to_job[w = prev[w_free]];
        }

        answers.push_back(current_cost);
    }

    return answers;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t N;
    cin >> N;

    vector<Weight> raw_costs(N * N);

    mdspan costs(raw_costs.data(), N, N);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            Weight val;
            cin >> val;
            costs[i, j] = val;
        }
    }

    auto ans = hungarian(costs);
    cout << ans[ans.size() - 1] << '\n';

    return 0;
}
