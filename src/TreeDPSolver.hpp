#ifndef TREE_DP_SOLVER_HPP
#define TREE_DP_SOLVER_HPP

#include "Tree.hpp"
#include <vector>
#include <cstddef>
using namespace std;

// Bo giai quy hoach dong tren cay va Rerooting hai luot duyet - Do phuc tap O(N)
// Trich nguon ly thuyet: CSES [15], GeeksforGeeks [18], VNOI Wiki [21], Prince Gupta YouTube [23]
//
// Ghi chu sua loi (so voi phien ban truoc): ban truoc dung DFS de quy
// (ham tu goi lai chinh no) cho ca dfs_down va dfs_reroot. Voi cay suy
// bien dang duong thang o quy mo N = 2*10^5, do sau de quy co the dat
// toi 2*10^5 khung ham, vuot qua gioi han ngan xep mac dinh cua he
// thong (thuong 1 MB - 8 MB tuy nen tang), gay loi Segmentation fault
// (da kiem chung thuc nghiem: crash voi ulimit -s 1024, khong crash voi
// 8192). Bao cao truoc day de xuat "cau hinh trinh bien dich de nang
// gioi han ngan xep" nhung giai phap nay khong duoc cai dat trong ma
// nguon va phu thuoc vao co che rieng cua tung compiler/OS (khong nhat
// quan giua GCC/MinGW/MSVC), nen khong dam bao an toan khi build lai
// tren may khac.
//
// Giai phap ap dung o day: chuyen ca hai luot duyet dfs_down va
// dfs_reroot sang dang LAP (iterative), mo phong lai dung ngu nghia cua
// ham de quy bang mot ngan xep tuong minh (vector) cap phat tren
// Heap thay vi Stack cua he dieu hanh. Vi Heap co dung luong lon (tuong
// duong RAM vat ly, hang GB), giai phap nay loai bo hoan toan nguy co
// tran ngan xep bat ke do sau cua cay, khong phu thuoc compiler hay he
// dieu hanh, va khong can bat ky co bien dich dac biet nao.
class TreeDPSolver {
private:
    const Tree& tree;
    int N;
    vector<int> sz;
    vector<long long> down_dist;
    vector<long long> ans;

    // Buffer dung chung cho hai luot duyet lap, tranh cap phat lai nhieu lan
    vector<int> parent;
    vector<size_t> child_cursor;   // vi tri neighbor dang xet cua tung dinh (cho dfs_down)
    vector<int> stack_nodes;       // ngan xep tuong minh tren Heap

    // Luot 1 (mo phong Hau thu tu / Post-order bang ngan xep tuong minh):
    // tinh sz va down_dist tu con len cha, ket qua giong het ban de quy.
    void dfs_down_iterative(int root) {
        stack_nodes.clear();
        stack_nodes.push_back(root);
        parent[root] = 0;
        sz[root] = 1;
        down_dist[root] = 0;
        child_cursor[root] = 0;

        while (!stack_nodes.empty()) {
            int u = stack_nodes.back();
            const vector<int>& neighbors = tree.get_neighbors(u);

            if (child_cursor[u] < neighbors.size()) {
                int v = neighbors[child_cursor[u]++];
                if (v == parent[u]) continue;
                parent[v] = u;
                sz[v] = 1;
                down_dist[v] = 0;
                child_cursor[v] = 0;
                stack_nodes.push_back(v);
            } else {
                // Tat ca con cua u da duoc xu ly xong (tuong duong thoi diem
                // ham de quy dfs_down(u) chuan bi return) -> gop ket qua len cha
                stack_nodes.pop_back();
                if (!stack_nodes.empty()) {
                    int p = stack_nodes.back();
                    sz[p] += sz[u];
                    down_dist[p] += down_dist[u] + sz[u];
                }
            }
        }
    }

    // Luot 2 (mo phong Tien thu tu / Pre-order bang ngan xep tuong minh):
    // tinh ans bang cach doi goc O(1) tu cha xuong con.
    void dfs_reroot_iterative(int root) {
        stack_nodes.clear();
        vector<int> parent_stack;
        stack_nodes.push_back(root);
        parent_stack.push_back(0);

        while (!stack_nodes.empty()) {
            int u = stack_nodes.back();
            stack_nodes.pop_back();
            int p = parent_stack.back();
            parent_stack.pop_back();

            for (int v : tree.get_neighbors(u)) {
                if (v == p) continue;
                // Ap dung cong thuc doi goc: ans[v] = ans[u] + N - 2 * sz[v]
                ans[v] = ans[u] + N - 2LL * sz[v];
                stack_nodes.push_back(v);
                parent_stack.push_back(u);
            }
        }
    }

public:
    explicit TreeDPSolver(const Tree& t) : tree(t), N(t.size()) {
        sz.assign(N + 1, 0);
        down_dist.assign(N + 1, 0);
        ans.assign(N + 1, 0);
        parent.assign(N + 1, 0);
        child_cursor.assign(N + 1, 0);
        stack_nodes.reserve(N + 1);
    }

    vector<long long> solve() {
        if (N == 1) {
            ans.assign(N + 1, 0);
            return ans;
        }
        dfs_down_iterative(1);
        ans = down_dist; // Tai goc tam thoi 1, ans chinh bang down_dist
        dfs_reroot_iterative(1);
        return ans;
    }
};

#endif // TREE_DP_SOLVER_HPP
