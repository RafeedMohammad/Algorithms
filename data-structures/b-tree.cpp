#include <iostream>
using namespace std;

const int t = 3; // minimum degree
const int MAX_KEYS = 2 * t - 1;
const int MAX_CHILDREN = 2 * t;

struct BTreeNode
{
    int keys[MAX_KEYS];
    BTreeNode *children[MAX_CHILDREN];
    int n; // current number of keys
    bool isLeaf;
};

BTreeNode *createNode(bool leaf)
{
    BTreeNode *newNode = new BTreeNode();
    newNode->isLeaf = leaf;
    newNode->n = 0;

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        newNode->children[i] = NULL;
    }

    return newNode;
}

void traverse(BTreeNode *root)
{
    int i;
    if (root == NULL)
    {
        return;
    }

    for (i = 0; i < root->n; i++)
    {
        if (root->isLeaf == false)
        {
            traverse(root->children[i]);
        }

        cout << root->keys[i] << " ";
    }

    if (root->isLeaf == false)
    {
        traverse(root->children[i]);
    }
}

BTreeNode *search(BTreeNode *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    int i = 0;

    // find first key >= key
    while (i < root->n && key > root->keys[i])
    {
        i++;
    }

    // if key found
    while (i < root->n && root->keys[i] == key)
    {
        return root;
    }

    // if leaf, key not found
    if (root->isLeaf == true)
    {
        return NULL;
    }

    // go to correct child
    return search(root->children[i], key);
}

int main()
{
    BTreeNode *root = createNode(true);
    root->keys[0] = 10;
    root->keys[1] = 20;
    root->keys[2] = 30;
    root->n = 3;
    traverse(root);

    BTreeNode *found = search(root, 40);

    cout << endl;

    if (found != NULL)
    {
        cout << "found" << endl;
    }
    else
    {
        cout << "not found" << endl;
    }
    return 0;
}