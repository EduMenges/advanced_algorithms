#if __has_include(<mdspan>)
#include <mdspan>
#else
#include "util/mdspan.hpp"
#endif

#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    size_t n;
    cin >> n;

    vector<int32_t> wins(n + 1);

    for (size_t i = 1; i <= n; ++i) {
        int32_t w;
        cin >> w;
        wins[i] = w;
    }

    vector<int32_t> raw_games((n + 1) * (n + 1));
    mdspan games(raw_games.data(), n + 1, n + 1);

    int32_t total_games = 0;

    for (size_t i = 1; i + 1 <= n; ++i) {
        for (size_t j = i + 1; j <= n; ++j) {
            int32_t g;
            cin >> g;
            games[i, j] = g;
            games[j, i] = g;

            if (i != 1 && j != 1) [[likely]] {
                total_games += g;
            }
        }
    }

    int32_t remaining_games = 0;

    for (size_t i = 2; i <= n; ++i) {
        remaining_games += games[1, i];
    }

    auto max_possible_victories = wins[1] + remaining_games;

    vector<int32_t> limits(n + 1);

    for (size_t i = 2; i <= n; ++i) {
        auto limit_i = max_possible_victories - wins[i] - 1;
        auto already_lost = limit_i < 0;

        if (already_lost) {
            cout << "false\n";
            return EXIT_SUCCESS;
        }

        limits[i] = limit_i;
    }

    int32_t s = 0;
    int32_t game_node_start = 1;
    int32_t game_node = game_node_start;
    int32_t team_node_start = 1 + (n - 1) * (n - 2) / 2;
    int32_t t = team_node_start + n - 1;
    int32_t node_count = t + 1;


}
