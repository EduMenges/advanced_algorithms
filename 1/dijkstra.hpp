#pragma once

#include <optional>
#include <vector>

using ListItem = std::pair<uint32_t, uint16_t>;
using Graph = std::vector<std::vector<std::pair<uint32_t, uint16_t>>>;

template <size_t K> class MinHeap {
  public:
    constexpr explicit MinHeap(size_t max_nodes) : node_indexes(max_nodes, UINT32_MAX) {}

    [[nodiscard]] constexpr bool empty() const { return heap.empty(); }

    constexpr void push(ListItem elem) {
        heap.push_back(elem);
        node_indexes[elem.first] = heap.size() - 1;
        sift_up(heap.size() - 1);
    }

    constexpr ListItem pop() {
        ListItem min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            node_indexes[heap[0].first] = 0;
            sift_down(0);
        }

        return min;
    }

    void update(ListItem item) {
        size_t index = node_indexes[item.first];

        uint16_t new_value = item.second;

        heap[index].second = new_value;

        sift_up(index);
    }

    size_t sift_down_count = 0;
    size_t sift_up_count = 0;

  private:
    constexpr void sift_down(size_t index) {
        using namespace std;

        sift_down_count += 1;

        while (true) {
            size_t smallest = index;

            for (size_t i = 0; i < K; ++i) {
                size_t child = K * index + i + 1;

                if (child < heap.size() && heap[child].second < heap[smallest].second) {
                    smallest = child;
                }
            }

            if (smallest == index)
                break;

            swap(heap[index], heap[smallest]);
            swap(node_indexes[heap[index].first], node_indexes[heap[smallest].first]);
            index = smallest;
        }
    }

    constexpr void sift_up(size_t index) {
        using namespace std;

        sift_up_count += 1;

        while (index > 0) {
            size_t parent = (index - 1) / K;

            if (heap[index].second < heap[parent].second) {
                swap(heap[index], heap[parent]);
                swap(node_indexes[heap[index].first], node_indexes[heap[parent].first]);
                index = parent;
            } else {
                break;
            }
        }
    }

    std::vector<ListItem> heap;
    std::vector<uint32_t> node_indexes;
};

struct Result {
    std::optional<uint64_t> dist;
    uint64_t push = 0;
    uint64_t pop = 0;
    uint64_t update = 0;
    size_t sift_up = 0;
    size_t sift_down = 0;
};

template <size_t K> Result dijkstra(const Graph &graph, uint32_t origin, uint32_t destination) {
    using namespace std;

    auto n = graph.size();

    uint64_t push_count = 1;
    uint64_t pop_count = 0;
    uint64_t update_count = 0;

    vector<bool> visited(n + 1, false);
    vector<uint32_t> dist(n + 1, UINT32_MAX);
    MinHeap<K> queue(n + 1);

    dist[origin] = 0;
    queue.push({origin, 0});

    while (!queue.empty()) {
        auto [v, _d_v] = queue.pop();
        visited[v] = true;
        pop_count += 1;

        for (const auto [u, d_u] : graph[v]) {
            if (visited[u]) {
                continue;
            }

            if (dist[u] == UINT32_MAX) {
                dist[u] = dist[v] + d_u;
                queue.push({u, dist[u]});
                push_count += 1;
            } else if (dist[v] + d_u < dist[u]) {
                dist[u] = dist[v] + d_u;
                queue.update({u, dist[u]});
                update_count += 1;
            }
        }
    }

    Result result{.dist = dist[destination],
                  .push = push_count,
                  .pop = pop_count,
                  .update = update_count,
                  .sift_up = queue.sift_up_count,
                  .sift_down = queue.sift_down_count};

    if (dist[destination] == UINT32_MAX) {
        result.dist = nullopt;
    }

    return result;
}
