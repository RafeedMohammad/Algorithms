#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

using PII = pair<int, int>;

int V = 5;
vector<vector<pair<int, int>>> adj(V);

vector<int> minEdge(V, INT_MAX);
vector<int> parent(V, -1);
vector<bool> inMST(V, false); // whether vertex is already in tree or not

priority_queue<
    PII,
    vector<PII>,
    greater<PII>>
    pq;

void addEdge(int u, int v, int w)
{
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

// void printPQ()
// {
//     priority_queue<
//         PII,
//         vector<PII>,
//         greater<PII>>
//         temp = pq;

//     cout << "Priority Queue:\n";

//     while (!temp.empty())
//     {
//         cout << "("
//              << temp.top().first
//              << ", "
//              << temp.top().second
//              << ")\n";

//         temp.pop();
//     }

//     cout << endl;
// }

void prim(int src)
{
    minEdge[src] = 0;
    pq.push({0, src});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u])
        {
            continue;
        }

        inMST[u] = true;

        for (auto [v, w] : adj[u])
        {
            if (!inMST[v] && w < minEdge[v])
            {
                parent[v] = u;
                minEdge[v] = w;
                pq.push({w, v});
            }
        }
    }
}

void printMST()
{
    int totalWeight = 0;
    cout << "MST Edges: " << endl;
    for (int v = 1; v < V; v++)
    {
        cout << parent[v] << " - " << v
             << " weight: " << minEdge[v] << endl;

        totalWeight += minEdge[v];
    }

    cout << "total weight: " << totalWeight << endl;
}

int main()
{
    addEdge(0, 1, 2);
    addEdge(0, 2, 6);
    addEdge(0, 3, 1);
    addEdge(1, 4, 5);
    addEdge(2, 4, 1);
    addEdge(3, 4, 4);

    prim(0);

    printMST();

    return 0;
}

/*
              (wt: 2)           (wt: 5)
           0 ——————————————— 1 ——————————————— 4
           | \                               / |
           |  \ (wt: 6)             (wt: 1) /  |
   (wt: 1) |   \                           /   | (wt: 4)
           |    ———————————— 2 ————————————    |
           |                                   |
           +———————————————— 3 ————————————————+

    Graph Characteristics:
    - Vertices: 5 (0 to 4)
    - Edges: 6
    - Type: Weighted, Undirected
*/