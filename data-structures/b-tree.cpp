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
    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < root->n; i++)
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