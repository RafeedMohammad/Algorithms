#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int V = 6;
vector<vector<int>> adj(V);
bool isDirected = false;
int timer = 0;

struct Node
{
    char color;
    int prev;
    int distance; // d[] - discovery time
    int finish;   // f[] - finish time
};
vector<Node> vertices(V);

void addEdge(int u, int v)
{
    adj[u].push_back(v);
    if (!isDirected)
    {
        adj[v].push_back(u);
    }
}

void DFS_Visit(int u)
{
    vertices[u].color = 'G';
    timer += 1;
    vertices[u].distance = timer;

    for (int v : adj[u])
    {
        if (vertices[v].color == 'W')
        {
            vertices[v].prev = u;
            DFS_Visit(v);
        }
    }
    vertices[u].color = 'B';
    timer += 1;
    vertices[u].finish = timer;
}

void DFS()
{
    for (int i = 0; i < V; i++)
    {
        vertices[i].color = 'W';
        vertices[i].distance = -1;
        vertices[i].prev = -1;
        vertices[i].finish = -1;
    }

    for (int i = 0; i < V; i++)
    {
        if (vertices[i].color == 'W')
        {
            DFS_Visit(i);
        }
    }
}

int main()
{
    addEdge(0, 1);
    addEdge(0, 5);
    addEdge(1, 2);
    addEdge(1, 4);
    addEdge(2, 3);
    addEdge(3, 4);
    addEdge(4, 5);

    DFS();

    cout << "vertex | color | d | f | prev\n";
    for (int i = 0; i < V; i++)
    {
        cout << "  " << i
             << "    |   " << vertices[i].color
             << "   | " << vertices[i].distance
             << " | " << vertices[i].finish
             << " |  " << vertices[i].prev << "\n";
    }

    return 0;
}