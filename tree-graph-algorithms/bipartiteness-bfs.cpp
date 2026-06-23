#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int V = 6;
vector<vector<int>> adj(V);
bool isDirected = false;

vector<int> side(V, -1);

struct Node
{
    char color;
    int prev;
    int distance;
};
vector<Node> vertices(V);

void resetGraph(int n)
{
    V = n;
    adj.assign(V, {});
    side.assign(V, -1);
    vertices.assign(V, Node());
}

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

bool BFS_bipartite(int s)
{
    side[s] = 0;
    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (side[v] == -1)
            {
                side[v] = 1 - side[u]; // provided opposite color for unvisited
                q.push(v);
            }

            else if (side[v] == side[u])
            {
                return false;
            }
        }
    }

    return true;
}

bool isBipartite()
{
    side.assign(V, -1);

    for (int i = 0; i < V; i++)
    {
        if (side[i] == -1)
        {
            if (!BFS_bipartite(i))
            {
                return false;
            }
        }
    }

    return true;
}

void printGroups()
{
    cout << "RED  group: ";
    for (int i = 0; i < V; i++)
        if (side[i] == 0)
            cout << i << " ";
    cout << "\nBLUE group: ";
    for (int i = 0; i < V; i++)
        if (side[i] == 1)
            cout << i << " ";
    cout << "\n";
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
    // ── Test 1: bipartite (even cycle — square) ──
    resetGraph(4);
    adj.assign(V, {});
    side.assign(V, -1);

    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 3);
    addEdge(3, 0);

    if (isBipartite())
    {
        cout << "Test 1: BIPARTITE\n";
        printGroups();
    }
    else
    {
        cout << "Test 1: NOT BIPARTITE\n";
    }

    // ── Test 2: not bipartite (triangle — odd cycle) ──
    resetGraph(3);

    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 0);

    if (isBipartite())
    {
        cout << "Test 2: BIPARTITE\n";
        printGroups();
    }
    else
    {
        cout << "Test 2: NOT BIPARTITE\n";
    }

    return 0;
}