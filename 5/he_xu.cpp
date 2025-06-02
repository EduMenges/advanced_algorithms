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

uint64_t knapsack_hexu(span<uint16_t> weights, span<uint16_t> values, uint64_t capacity) {
    assert(weights.size() > 0);
    assert(weights.size() == values.size());

    random_device rd;
    mt19937_64 g(rd());

    auto w_max = *ranges::max_element(weights);

    ranges::shuffle(views::zip(weights, values), g);

    uint64_t n = weights.size();

    unordered_map<uint64_t, int64_t> prev;
    prev[0] = 0;

    for (uint64_t i = 1; i <= n; ++i) {
        double mu_i =
            static_cast<double>(i) / static_cast<double>(n) * static_cast<double>(capacity);
        double delta_i = 2.0 * std::sqrt(i) * static_cast<double>(w_max);

        auto low = static_cast<uint64_t>(std::max(mu_i - delta_i, 0.0));
        auto high = std::min(capacity, static_cast<uint64_t>(mu_i + delta_i));

        unordered_map<uint64_t, int64_t> curr;

        for (uint64_t j = low; j <= high; ++j) {
            auto val1 = prev.contains(j) ? prev[j] : numeric_limits<int64_t>::lowest();

            auto val2 = numeric_limits<int64_t>::lowest();
            if (j >= weights[i - 1] && prev.contains(j - weights[i - 1])) {
                val2 = prev[j - weights[i - 1]] + values[i - 1];
            }

            curr[j] = std::max(val1, val2);
        }

        prev = move(curr);
    }

    return static_cast<uint64_t>(*ranges::max_element(views::values(prev)));
}

int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    //
    // size_t n;
    // uint64_t W;
    // cin >> n >> W;
    //
    // vector<uint16_t> p(n);
    //
    // for (size_t i = 0; i < n; ++i) {
    //     cin >> p[i];
    // }
    //
    // vector<uint16_t> w(n);
    //
    // for (size_t i = 0; i < n; ++i) {
    //     cin >> w[i];
    // }

    vector<uint16_t> weights = {2, 3, 4, 5}, values = {3, 4, 5, 6};
    uint64_t capacity = 5;
    cout << knapsack_hexu(weights, values, capacity) << '\n';
}
