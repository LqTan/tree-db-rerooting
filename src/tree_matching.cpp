#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> adj;
vector<vector<int>> dp;

void dfs(int v, int parent) {
    // Tinh dp[v][0]
    for (int u : adj[v]) {
        if (u == parent) continue;
        dfs(u, v);
        dp[v][0] += max(dp[u][0], dp[u][1]);
    }

    // Tinh dp[v][1]
    for (int u : adj[v]) {
        if (u == parent) continue;
        int candidate =
            dp[v][0]
            - max(dp[u][0], dp[u][1])
            + dp[u][0]
            + 1;
        dp[v][1] = max(dp[v][1], candidate);
    }
}

int main() {
    int n;
    cin >> n;
    
    adj.resize(n + 1);
    dp.assign(n + 1, vector<int>(2, 0));

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    cout << max(dp[1][0], dp[1][1]) << '\n';
    
    return 0;
}