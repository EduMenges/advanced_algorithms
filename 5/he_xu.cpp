#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

using namespace std;

pair<uint64_t, uint64_t> knapsack_dp(span<const uint16_t> weights,
                                     span<const uint16_t> values,
                                     const uint64_t capacity) {
    assert(weights.size() == values.size());

    auto n = weights.size();

    vector<uint64_t> prev(capacity + 1);
    uint64_t updates = 0;

    for (size_t i = 0; i <= n; ++i) {
        for (int64_t w = capacity; w >= 0; --w) {
            if (weights[i] <= w) {
                prev[w] = max(prev[w], prev[w - weights[i]] + values[i]);
                updates += 1;
            }
        }
    }

    return {prev[capacity], updates};
}

pair<uint64_t, uint64_t> knapsack_hexu(span<const uint16_t> weights,
                                       span<const uint16_t> values,
                                       uint64_t capacity) {
    assert(weights.size() > 0);
    assert(weights.size() == values.size());

    random_device rd;
    mt19937_64 g(rd());

    auto w_max = *ranges::max_element(weights);
    uint64_t n = weights.size();

    vector<size_t> order(n);
    ranges::iota(order, 0);
    ranges::shuffle(order, g);

    constexpr int64_t NEG_INF = numeric_limits<int64_t>::lowest();
    vector<int64_t> prev(capacity + 1, NEG_INF);
    prev[0] = 0;

    uint64_t updates = 0;

    for (uint64_t pos = 0; pos < n; ++pos) {
        auto i = pos + 1;
        auto id = order[pos];
        auto wi = static_cast<int64_t>(weights[id]);
        auto pi = values[id];

        double mu = static_cast<double>(i) / static_cast<double>(n) * static_cast<double>(capacity);
        double delta = 2.0 * std::sqrt(i) * static_cast<double>(w_max);

        auto low = std::max(static_cast<int64_t>(mu - delta), wi);
        auto high = std::min(capacity, static_cast<uint64_t>(std::ceil(mu) + delta));

        for (int64_t j = high; j >= low; --j) {
            if (prev[j - wi] != NEG_INF) {
                if (auto potential = prev[j - wi] + pi; prev[j] < potential) {
                    prev[j] = potential;
                    updates += 1;
                }
            }
        }
    }

    return {static_cast<uint64_t>(*ranges::max_element(prev)), updates};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n = 5;
    uint64_t capacity = 10;
    cin >> n >> capacity;

    vector<uint16_t> values(n);

    for (size_t i = 0; i < n; ++i) {
        cin >> values[i];
    }

    vector<uint16_t> weights(n);

    for (size_t i = 0; i < n; ++i) {
        cin >> weights[i];
    }

    auto start = chrono::steady_clock::now();

    auto [result_dp, updates_dp] = knapsack_dp(weights, values, capacity);

    auto now = chrono::steady_clock::now();
    auto time_dp =
        chrono::duration_cast<chrono::milliseconds>(chrono::duration(now - start)).count();

    start = chrono::steady_clock::now();

    auto [result_hexu, updates_hexu] = knapsack_hexu(weights, values, capacity);

    now = chrono::steady_clock::now();
    auto time_hexu =
        chrono::duration_cast<chrono::milliseconds>(chrono::duration(now - start)).count();

    cout << result_dp << ',' << time_dp << ',' << updates_dp << ',' << result_dp << ',' << time_hexu
         << ',' << updates_hexu << '\n';
}
