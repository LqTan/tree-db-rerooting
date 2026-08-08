#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "Tree.hpp"
#include "IndexMapper.hpp"
#include "InputValidator.hpp"
#include "TreeDPSolver.hpp"
#include "BaselineSolver.hpp"

// Hàm main tích hợp hệ thống, hỗ trợ tham số --baseline để chọn bộ giải đối chiếu
int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    bool use_baseline = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--baseline") {
            use_baseline = true;
        }
    }

    try {
        int N;
        if (!(std::cin >> N)) return 0;

        IndexMapper<std::string> mapper;
        std::vector<std::pair<int, int>> mapped_edges;
        mapped_edges.reserve(N - 1);

        for (int i = 0; i < N - 1; ++i) {
            std::string u_name, v_name;
            if (std::cin >> u_name >> v_name) {
                int u_idx = mapper.get_or_create_index(u_name);
                int v_idx = mapper.get_or_create_index(v_name);
                mapped_edges.push_back({u_idx, v_idx});
            }
        }

        // Bước 1: Xác thực cấu trúc đồ thị đầu vào có phải cây hợp lệ
        InputValidator::validate_and_build(N, mapped_edges);

        // Bước 2: Xây dựng cây tối ưu
        Tree tree(N);
        for (const auto& edge : mapped_edges) {
            tree.add_edge(edge.first, edge.second);
        }
        tree.optimize_memory();

        // Bước 3: Lựa chọn và chạy thực thi bộ giải phù hợp
        std::vector<long long> results;
        if (use_baseline) {
            BaselineSolver solver(tree);
            results = solver.solve();
        } else {
            TreeDPSolver solver(tree);
            results = solver.solve();
        }

        // Bước 4: Xuất kết quả map về tên đỉnh ban đầu
        for (int i = 1; i <= N; ++i) {
            std::string original_name = mapper.get_original_id(i);
            std::cout << original_name << " " << results[i] << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
