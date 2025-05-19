#include <random>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 2) {
        cout << "Usage: generator number_of_vertices\n";
        return EXIT_FAILURE;
    }

    auto n = stoul(argv[1]);

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint32_t> dist(0, n * n);

    cout << n << '\n';

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
           cout << dist(gen) << ' ';
        }
        cout << '\n';
    }

    return EXIT_SUCCESS;
}
