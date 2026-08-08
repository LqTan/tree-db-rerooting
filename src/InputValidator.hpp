#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>

// Lớp xác thực cấu trúc cây sử dụng DSU để tìm chu trình và kiểm tra tính liên thông
// Trích nguồn lý thuyết: Algorithms for Competitive Programming [11], USACO Guide [10]
class InputValidator {
private:
    struct DSU {
        std::vector<int> parent;
        DSU(int n) {
            parent.resize(n + 1);
            std::iota(parent.begin(), parent.end(), 0);
        }
        int find_set(int v) {
            if (v == parent[v]) return v;
            return parent[v] = find_set(parent[v]); // Path compression
        }
        bool union_sets(int a, int b) {
            a = find_set(a);
            b = find_set(b);
            if (a != b) {
                parent[b] = a;
                return true;
            }
            return false; // Phát hiện chu trình
        }
    };

public:
    static void validate_and_build(int N, const std::vector<std::pair<int, int>>& edges) {
        if (N <= 0) {
            throw std::invalid_argument("Loi: So dinh phai lon hon 0.");
        }
        if (edges.size() != static_cast<size_t>(N - 1)) {
            throw std::invalid_argument("Loi: So canh khong thoa man bat bien cay (E = N - 1).");
        }

        DSU dsu(N);
        int components = N;

        for (const auto& edge : edges) {
            int u = edge.first;
            int v = edge.second;

            if (u < 1 || u > N || v < 1 || v > N) {
                throw std::out_of_range("Loi: Chi so dinh nam ngoai pham vi [1, N].");
            }
            if (u == v) {
                throw std::invalid_argument("Loi: Ton tai tu khuyen (self-loop).");
            }
            if (!dsu.union_sets(u, v)) {
                throw std::invalid_argument("Loi: Do thi co chu trinh, khong phai cau truc cay.");
            }
            components--;
        }

        if (components != 1) {
            throw std::invalid_argument("Loi: Do thi bi phan manh, khong lien thong.");
        }
    }
};

#endif // INPUT_VALIDATOR_HPP