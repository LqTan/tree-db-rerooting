#ifndef TREE_HPP
#define TREE_HPP

#include <vector>

// Lớp biểu diễn đồ thị dạng cây dưới dạng danh sách kề tối ưu bộ nhớ
// Trích nguồn lý thuyết: GeeksforGeeks [7], VNOI Wiki [8]
class Tree {
private:
    int num_nodes;
    std::vector<std::vector<int>> adj;

public:
    explicit Tree(int n) : num_nodes(n) {
        adj.resize(num_nodes + 1);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int size() const {
        return num_nodes;
    }

    const std::vector<int>& get_neighbors(int u) const {
        return adj[u];
    }

    // Tối ưu bộ nhớ vật lý bằng cách loại bỏ dung lượng thừa
    void optimize_memory() {
        for (int i = 1; i <= num_nodes; ++i) {
            adj[i].shrink_to_fit();
        }
    }
};

#endif // TREE_HPP