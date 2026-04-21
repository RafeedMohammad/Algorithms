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

void splitChild(BTreeNode *parent, int i)
{
    BTreeNode *y = parent->children[i];

    // create new node z (same leaf status as y)
    BTreeNode *z = createNode(y->isLeaf);

    z->n = t - 1;

    // copy last (t-1) keys into z: indices after the midpoint
    for (int j = 0; j < t - 1; j++)
    {
        z->keys[j] = y->keys[j + t];
    }

    // if y is not leaf, copy the last t children of y into z

    if (y->isLeaf == false)
    {
        for (int j = 0; j < t; j++)
        {
            z->children[j] = y->children[j + t];
        }
    }

    // reduce number of keys in y
    y->n = t - 1;

    // shift parent's children to make space for z

    for (int j = parent->n; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }

    // Link z as parent's child
    parent->children[i + 1] = z;

    // Shift parent's keys to make space for middle key
    for (int j = parent->n - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }

    // move middle key of y to parent
    parent->keys[i] = y->keys[t - 1];

    // increase key count of parent
    parent->n = parent->n + 1;
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