#include <string>
#include <iosfwd>
#include <iostream>
#include <random>

using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: generator n <capacity> <w_max>\n";
        return EXIT_FAILURE;
    }

    auto n = stoull(argv[1]);

    uint64_t capacity = n * numeric_limits<uint8_t>::max() / 4;
    if (argc >= 3) {
        capacity = stoull(argv[2]);
    }

    uint16_t max_weight = numeric_limits<uint8_t>::max();
    if (argc >= 4) {
        max_weight = stoul(argv[3]);
    }

    random_device rd;
    mt19937_64 g(rd());

    constexpr uint8_t MAX_VALUE = numeric_limits<uint8_t>::max();

    uniform_int_distribution<uint16_t> value_dist(1, MAX_VALUE);
    uniform_int_distribution<uint16_t> weight_dist(1, max_weight);

    cout << n << ' ' << capacity << '\n';

    // Values
    for (uint64_t i = 0; i < n; ++i) {
        cout << value_dist(g) << ' ';
    }
    cout << '\n';

    // Weight
    for (uint64_t i = 0; i < n; ++i) {
        cout << weight_dist(g) << ' ';
    }
    cout << '\n';

    return EXIT_SUCCESS;
}
