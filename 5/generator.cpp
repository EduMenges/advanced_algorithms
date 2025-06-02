#include <random>
#include <iosfwd>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: generator n\n";
    }

    auto n = stoull(argv[1]);

    random_device rd;
    mt19937_64 g(rd());

    constexpr uint8_t MAX_VALUE = numeric_limits<uint8_t>::max();
    constexpr uint8_t MAX_WEIGHT = numeric_limits<uint8_t>::max();

    uniform_int_distribution<uint8_t> value_dist(1, MAX_VALUE);
    uniform_int_distribution<uint8_t> weight_dist(1, MAX_WEIGHT);

    // Capacity is a forth of the maximum weight
    uint64_t capacity = n * MAX_WEIGHT / 4;

    cout << n << ' ' << capacity << '\n';

    // Values
    for (uint64_t i = 0; i < n; ++i) {
        cout << static_cast<uint32_t>(value_dist(g)) << ' ';
    }
    cout << '\n';

    // Weight
    for (uint64_t i = 0; i < n; ++i) {
        cout << static_cast<uint32_t>(weight_dist(g)) << ' ';
    }
    cout << '\n';
}