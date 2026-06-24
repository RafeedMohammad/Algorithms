#include <iostream>
#include <vector>
using namespace std;

class DisjointSet
{
    vector<int> rank, parent;

public:
    DisjointSet(int n)
    {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);

        for (int i = 0; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int findParent(int node)
    {
        if (node == parent[node])
        {
            return node;
        }
        return parent[node] = findParent(parent[node]);
    }

    void unionByRank(int u, int v)
    {
        int ultimateParentOfU = findParent(u);
        int ultimateParentOfV = findParent(v);

        if (ultimateParentOfU == ultimateParentOfV)
        {
            return;
        }

        if (rank[ultimateParentOfU] < rank[ultimateParentOfV])
        {
            parent[ultimateParentOfU] = ultimateParentOfV;
        }

        if (rank[ultimateParentOfU] > rank[ultimateParentOfV])
        {
            parent[ultimateParentOfU] = ultimateParentOfV;
        }

        else
        {
            parent[ultimateParentOfV] = ultimateParentOfU;
            rank[ultimateParentOfU]++;
        }
    }
};

int main()
{
    DisjointSet ds(7);
    ds.unionByRank(1, 2);
    ds.unionByRank(2, 3);
    ds.unionByRank(4, 5);
    ds.unionByRank(6, 7);

    if (ds.findParent(6) == ds.findParent(7))
    {
        cout << "connected" << endl;
    }

    ds.unionByRank(5, 6);

    if (ds.findParent(3) == ds.findParent(7))
    {
        cout << "connected" << endl;
    }

    else
    {
        cout << "Not connected" << endl;
    }

    ds.unionByRank(3, 7);

    return 0;
}