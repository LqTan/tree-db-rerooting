#ifndef TREE_DP_SOLVER_HPP
#define TREE_DP_SOLVER_HPP

#include "Tree.hpp"
#include <vector>
using namespace std;

// Bộ giải quy hoạch động trên cây và Rerooting hai lượt DFS - Độ phức tạp O(N)
// Trích nguồn lý thuyết: CSES [15], GeeksforGeeks [18], VNOI Wiki [21], Prince Gupta YouTube [23]
class TreeDPSolver {
private:
    const Tree& tree;
    int N;
    vector<int> sz;
    vector<long long> down_dist;
    vector<long long> ans;

    // DFS 1 (Hậu thứ tự / Post-order): tính sz và down_dist từ con lên cha
    void dfs_down(int u, int parent) {
        sz[u] = 1;
        down_dist[u] = 0;
        for (int v : tree.get_neighbors(u)) {
            if (v == parent) continue;
            dfs_down(v, u);
            sz[u] += sz[v];
            down_dist[u] += down_dist[v] + sz[v];
        }
    }

    // DFS 2 (Tiền thứ tự / Pre-order): tính ans bằng cách đổi gốc O(1) từ cha xuống con
    void dfs_reroot(int u, int parent) {
        for (int v : tree.get_neighbors(u)) {
            if (v == parent) continue;
            // Áp dụng công thức đổi gốc: ans[v] = ans[u] + N - 2 * sz[v]
            ans[v] = ans[u] + N - 2LL * sz[v];
            dfs_reroot(v, u);
        }
    }

public:
    explicit TreeDPSolver(const Tree& t) : tree(t), N(t.size()) {
        sz.assign(N + 1, 0);
        down_dist.assign(N + 1, 0);
        ans.assign(N + 1, 0);
    }

    vector<long long> solve() {
        if (N == 1) {
            ans.assign(N + 1, 0);
            return ans;
        }
        dfs_down(1, 0);
        ans = down_dist; // Tại gốc tạm thời 1, ans chính bằng down_dist
        dfs_reroot(1, 0);
        return ans;
    }
};

#endif // TREE_DP_SOLVER_HPP