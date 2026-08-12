#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

// Trình sinh cây đa dạng phục vụ kiểm thử thực nghiệm
// Trích nguồn phương pháp: VNOI Wiki [6, 29]
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

long long rand_range(long long l, long long r) {
    return uniform_int_distribution<long long>(l, r)(rng);
}

void generate_single_node() {
    cout << 1 << "\n";
}

void generate_line_tree(int N) {
    cout << N << "\n";
    vector<pair<int, int>> edges;
    for (int i = 1; i < N; ++i) {
        edges.push_back({i, i + 1});
    }
    shuffle(edges.begin(), edges.end(), rng);
    for (auto const& edge : edges) {
        cout << edge.first << " " << edge.second << "\n";
    }
}

void generate_star_tree(int N) {
    cout << N << "\n";
    int center = 1;
    vector<pair<int, int>> edges;
    for (int i = 2; i <= N; ++i) {
        edges.push_back({center, i});
    }
    shuffle(edges.begin(), edges.end(), rng);
    for (auto const& edge : edges) {
        cout << edge.first << " " << edge.second << "\n";
    }
}

void generate_balanced_tree(int N) {
    cout << N << "\n";
    vector<pair<int, int>> edges;
    for (int i = 2; i <= N; ++i) {
        int parent = i / 2;
        edges.push_back({parent, i});
    }
    shuffle(edges.begin(), edges.end(), rng);
    for (auto const& edge : edges) {
        cout << edge.first << " " << edge.second << "\n";
    }
}

void generate_random_tree(int N) {
    cout << N << "\n";
    vector<pair<int, int>> edges;
    for (int i = 2; i <= N; ++i) {
        int parent = rand_range(1, i - 1);
        edges.push_back({parent, i});
    }
    shuffle(edges.begin(), edges.end(), rng);
    for (auto const& edge : edges) {
        if (rand_range(0, 1)) {
            cout << edge.second << " " << edge.first << "\n";
        } else {
            cout << edge.first << " " << edge.second << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Cach dung: " << argv << " <N> <Loai_Cay>\n";
        cerr << "Loai_Cay: single, line, star, balanced, random\n";
        return 1;
    }

    int N = stoi(argv[1]);
    string type = argv[2];

    if (type == "single") {
        generate_single_node();
    } else if (type == "line") {
        generate_line_tree(N);
    } else if (type == "star") {
        generate_star_tree(N);
    } else if (type == "balanced") {
        generate_balanced_tree(N);
    } else if (type == "random") {
        generate_random_tree(N);
    } else {
        cerr << "Loi: Khong ho tro dang cay " << type << "\n";
        return 1;
    }

    return 0;
}
