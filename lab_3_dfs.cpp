#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// Graph represented as adjacency list
class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int v) : V(v), adj(v) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);

        cout << "DFS traversal starting from node " << start << ": ";
        while (!s.empty()) {
            int node = s.top(); s.pop();
            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";
                // Push neighbors in reverse order for left-to-right traversal
                for (int i = adj[node].size() - 1; i >= 0; i--)
                    if (!visited[adj[node][i]])
                        s.push(adj[node][i]);
            }
        }
        cout << "\n";
    }
};

int main() {
    // Example graph:
    //   0 -- 1 -- 3
    //   |    |
    //   2    4
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);

    cout << "Graph edges: 0-1, 0-2, 1-3, 1-4\n";
    g.DFS(0);
    return 0;
}
