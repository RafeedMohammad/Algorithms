#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int V = 6;
vector<vector<int>> adj(V);
bool isDirected = false;

struct Node
{
    char color;
    int prev;
    int distance;
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

void printGraph()
{
    for (int i = 0; i < adj.size(); i++)
    {
        cout << "vertex: " << i << ":";
        for (int neighbor : adj[i])
        {
            cout << "->" << neighbor;
        }

        cout << endl;
    }
}

void BFS(int s)
{
    for (int i = 0; i < V; i++)
    {
        vertices[i].color = 'W';
        vertices[i].distance = -1;
        vertices[i].prev = -1;
    }
    vertices[s].color = 'G';
    vertices[s].distance = 0;

    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (vertices[v].color == 'W')
            {
                vertices[v].color = 'G';
                vertices[v].distance = vertices[u].distance + 1;
                vertices[v].prev = u;
                q.push(v);
            }
        }

        vertices[u].color = 'B';
    }
}

void printPath(int s, int v)
{
    if (v == s)
    {
        cout << s << " ";
        return;
    }

    if (vertices[v].prev == -1)
    {
        cout << "No Path" << endl;
    }

    printPath(s, vertices[v].prev);
    cout << v << " ";
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

    printGraph();

    BFS(0);
    printPath(0, 3);
    return 0;
}

/*
     0 ——————— 1 ——————— 2
     |         |         |
     |         |         |
     |         |         |
     5 ——————— 4 ——————— 3

    Graph Characteristics:
    - Vertices: 6 (0 to 5)
    - Edges: 7
    - Type: Undirected

    BFS Shortest Path (from 0 to 3):
    - Path length / Distance: 3 edges
    - Your printPath(0, 3) will output: 0 1 2 3
*/