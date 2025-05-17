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

vector<Weight> hungarian(const mdspan<Weight, dextents<size_t, 2>> C) {
    const auto J = C.extent(0);
    const auto W = C.extent(1);
    assert(J <= W);

    vector<int32_t> worker_to_job(W + 1, -1);

    // Potentials per worker
    vector<Weight> h(W, 0);

    // Cumulative optimal costs
    vector<Weight> answers;
    Weight ans = 0;

    constexpr auto INF = numeric_limits<Weight>::max();

    for (int32_t j_cur = 0; j_cur < J; ++j_cur) {
        const auto dummy_worker = W;
        auto w_source = dummy_worker;
        worker_to_job[w_source] = j_cur;

        // Johnson-reduced distances
        vector<Weight> dist(W + 1, INF);
        dist[w_source] = 0;

        vector<uint8_t> visited(W + 1, false);
        vector<int> prev(W + 1, -1);

        priority_queue<pair<Weight, size_t>, vector<pair<Weight, size_t>>,
                       greater<pair<Weight, size_t>>>
            pq;
        pq.emplace(0, w_source);

        // Stop at the first free worker
        int w_free = -1;

        while (!pq.empty()) {
            auto [du, worker] = pq.top();
            pq.pop();

            if (visited[worker] || du != dist[worker]) {
                continue;
            }

            visited[worker] = true;

            auto associated_job = worker_to_job[worker];
            if (bool reached_unmatched_worker = associated_job == -1; reached_unmatched_worker) {
                w_free = worker;
                break;
            }

            // Relax edges: worker -> v (other workers)
            for (int v = 0; v < W; ++v) {
                if (visited[v]) {
                    continue;
                }

                Weight edge = C[associated_job, v] - h[v];
                if (worker != dummy_worker) {
                    edge -= C[associated_job, worker] - h[worker];
                }

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

        ans += h[w_free];

        for (size_t w; w_free != dummy_worker; w_free = w) {
            worker_to_job[w_free] = worker_to_job[w = prev[w_free]];
        }

        answers.push_back(ans);
    }

    return answers;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t N;
    cin >> N;

    vector<int> raw_costs(N * N);

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
