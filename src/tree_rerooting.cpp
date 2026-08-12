#include <iostream>
#include <vector>
using namespace std;

int n;
vector<vector<int>> adj;
vector<int> sub;
vector<long long> in;
vector<long long> ans;

void dfs1(int u, int parent) {
    sub[u] = 1;
    in[u] = 0;

    for (int v : adj[u]) {
        if (v == parent) continue;
        dfs1(v, u);
        sub[u] += sub[v];
        in[u] += in[v] + sub[v];
    }
}

void dfs2(int u, int parent) {
    for (int v : adj[u]) {
        if (v == parent) continue;
        ans[v] = ans[u] + n - 2 * sub[v];
        dfs2(v, u);
    }
}

int main() {
    cin >> n;

    adj.resize(n + 1);
    sub.resize(n + 1);
    in.resize(n + 1);
    ans.resize(n + 1);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs1(1, 0);
    ans[1] = in[1];
    dfs2(1, 0);

    for (int u = 1; u <= n; u++) {
        cout << ans[u] << '\n';
    }

    return 0;
}