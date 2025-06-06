#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

using namespace std;

uint64_t knapsack_dp(span<const uint16_t> weights,
                     span<const uint16_t> values,
                     const uint64_t capacity) {
    assert(weights.size() == values.size());

    auto n = weights.size();

    vector<uint64_t> curr(capacity + 1), prev(capacity + 1);

    for (size_t i = 0; i <= n; ++i) {
        for (uint64_t w = 0; w <= capacity; ++w) {
            if (weights[i] <= w) {
                curr[w] = max(prev[w], prev[w - weights[i]] + values[i]);
            } else {
                curr[w] = prev[w];
            }
        }
        swap(curr, prev);
    }

    return curr[capacity];
}

uint64_t knapsack_hexu(span<const uint16_t> weights,
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

    for (uint64_t pos = 0; pos < n; ++pos) {
        auto i = pos + 1;
        auto id = order[pos];
        auto wi = weights[id];
        auto pi = values[id];

        double mu = static_cast<double>(i) / static_cast<double>(n) * static_cast<double>(capacity);
        double delta = 2.0 * std::sqrt(i) * static_cast<double>(w_max);

        auto low = static_cast<uint64_t>(std::max(mu - delta, 0.0));
        auto high = std::min(capacity, static_cast<uint64_t>(std::ceil(mu) + delta));

        vector curr(prev);

        for (uint64_t j = low; j <= high; ++j) {
            auto best = prev[j];

            if (j >= wi && prev[j - wi] != NEG_INF) {
                best = std::max(best, prev[j - wi] + pi);
            }

            curr[j] = best;
        }

        prev = std::move(curr);
    }

    return static_cast<uint64_t>(*ranges::max_element(prev));
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

    // vector<uint16_t> weights = {3, 4, 7, 8, 9};
    // vector<uint16_t> values = {4, 5, 10, 11, 13};

    cout << knapsack_hexu(weights, values, capacity) << '\n';
}
