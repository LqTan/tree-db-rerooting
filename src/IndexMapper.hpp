#ifndef INDEX_MAPPER_HPP
#define INDEX_MAPPER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

// Lớp ánh xạ nhãn đỉnh rời rạc hoặc dạng chuỗi sang chỉ số 1-based index liên tục
// Trích nguồn lý thuyết: VNOI Wiki [9], USACO Guide [10]
template <typename T>
class IndexMapper {
private:
    std::unordered_map<T, int> forward_map;
    std::vector<T> backward_map;
    int current_index;

public:
    IndexMapper() : current_index(0) {
        backward_map.push_back(T{}); // Giữ chỗ cho index 0 để bắt đầu từ chỉ số 1
    }

    int get_or_create_index(const T& original_id) {
        auto it = forward_map.find(original_id);
        if (it != forward_map.end()) {
            return it->second;
        }
        current_index++;
        forward_map[original_id] = current_index;
        backward_map.push_back(original_id);
        return current_index;
    }

    int get_index(const T& original_id) const {
        auto it = forward_map.find(original_id);
        if (it == forward_map.end()) {
            throw std::invalid_argument("Dinh khong ton tai trong anh xa.");
        }
        return it->second;
    }

    T get_original_id(int index) const {
        if (index < 1 || index >= static_cast<int>(backward_map.size())) {
            throw std::out_of_range("Chi so noi bo vuot qua pham vi.");
        }
        return backward_map[index];
    }

    int get_total_nodes() const {
        return current_index;
    }
};

#endif // INDEX_MAPPER_HPP