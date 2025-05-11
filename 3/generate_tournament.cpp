#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // team amount, rounds, winning probability, bias of team 1
    int teams = (argc > 1) ? atoi(argv[1]) : 5;
    int rounds = (argc > 2) ? atoi(argv[2]) : 2;
    double win_p = (argc > 3) ? atof(argv[3]) : 0.5;
    double bias = (argc > 4) ? atof(argv[4]) : 0.1;

    if (teams <= 0 || rounds <= 0 || win_p < 0.0 || win_p > 1.0 || bias < -1.0 || bias > 1.0) {
        cerr << "Invalid arguments. Ensure n > 0, c > 0, and 0 ≤ α ≤ 1, and -1 ≤ β ≤ 1.\n";
        return 1;
    }

    vector<int> raw_games(teams * teams, rounds);
    mdspan games(raw_games.data(), teams, teams);

    vector<double> raw_p(teams * teams, 0.0);
    mdspan p(raw_p.data(), teams, teams);
    vector<int> wins(teams, 0);

    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    for (size_t j = 1; j < teams; ++j) {
        p[0, j] = min(1.0, dist(gen) + bias);
    }

    for (size_t i = 1; i < teams; ++i) {
        for (size_t j = i + 1; j < teams; ++j) {
            p[i, j] = dist(gen);
        }
    }

    auto total_combinations = (teams * (teams - 1)) / 2;
    int total_games = rounds * total_combinations;
    int samples = static_cast<int>(ceil(win_p * total_games));
    uniform_int_distribution<> dist_i(0, teams - 2);

    for (int s = 0; s < samples; ++s) {
        int i, j;

        do {
            i = dist_i(gen);
            uniform_int_distribution<> dist_j(i + 1, teams - 1);
            j = dist_j(gen);
        } while (games[i, j] == 0);

        games[i, j] -= 1;

        if (dist(gen) < p[i, j]) {
            wins[i] += 1;
        } else {
            wins[j] += 1;
        }
    }

    cout << teams << '\n';
    for (auto w : wins) {
        cout << w << ' ';
    }
    cout << '\n';

    for (int i = 0; i + 1 < teams; ++i) {
        for (int j = i + 1; j < teams; ++j) {
            cout << games[i, j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}
