#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge
{
    int u, v, weight;
};

int parent[100];
int rankArr[100]; // using Union by Rank, not Size

void makeSet(int v)
{
    parent[v] = v;
    rankArr[v] = 0;
}

int findSet(int x)
{
    if (parent[x] != x)
    {
        parent[x] = findSet(parent[x]);
    }

    return parent[x];
}

void unionSet(int u, int v)
{
    int rootU = findSet(u);
    int rootV = findSet(v);

    if (rankArr[rootU] < rankArr[rootV])
    {
        parent[rootU] = rootV;
    }

    else if (rankArr[rootU] > rankArr[rootV])
    {
        parent[rootV] = rootU;
    }

    else
    {
        parent[rootV] = rootU;
        rankArr[rootU]++;
    }
}

bool compareEdge(Edge a, Edge b)
{
    return a.weight < b.weight;
}

void kruskal(int V, Edge edges[], int E)
{
    for (int i = 0; i < V; i++)
    {
        makeSet(i);
    }

    sort(edges, edges + E, compareEdge);

    Edge mst[100];
    int mstSize = 0;
    int mstCost = 0;

    for (int i = 0; i < E; i++)
    {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        if (findSet(u) != findSet(v))
        {
            mst[mstSize++] = edges[i];
            mstCost += w;

            unionSet(u, v);
        }
    }

    cout << "MST Edges:" << endl;
    for (int i = 0; i < mstSize; i++)
        cout << "  " << mst[i].u << " -- " << mst[i].v
             << "  (weight " << mst[i].weight << ")" << endl;

    cout << "Total MST cost: " << mstCost << endl;
}

int main()
{
    int V = 4;

    Edge edges[] = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}};
    int E = 5;

    kruskal(V, edges, E);
}