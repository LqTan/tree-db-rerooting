#ifndef BASELINE_SOLVER_HPP
#define BASELINE_SOLVER_HPP

#include "Tree.hpp"
#include <vector>
#include <queue>

// Thuật toán đối chiếu baseline sử dụng BFS từ từng đỉnh - Độ phức tạp O(N^2)
// Trích nguồn lý thuyết: GeeksforGeeks [26], HackMD [27]
class BaselineSolver {
private:
    const Tree& tree;
    int N;

    long long bfs_distance(int start_node) {
        std::vector<int> dist(N + 1, -1);
        std::queue<int> q;

        dist[start_node] = 0;
        q.push(start_node);

        long long total_dist = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            total_dist += dist[u];

            for (int v : tree.get_neighbors(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return total_dist;
    }

public:
    explicit BaselineSolver(const Tree& t) : tree(t), N(t.size()) {}

    std::vector<long long> solve() {
        std::vector<long long> ans(N + 1, 0);
        if (N == 1) {
            ans.assign(N + 1, 0);
            return ans;
        }
        for (int i = 1; i <= N; ++i) {
            ans[i] = bfs_distance(i);
        }
        return ans;
    }
};

#endif // BASELINE_SOLVER_HPP