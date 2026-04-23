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

BTreeNode *root = NULL; // global root

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

// This function inserts a key into a note that is guranteed not full
void insertNonFull(BTreeNode *node, int key)
{
    int i = node->n - 1;

    if (node->isLeaf == true)
    {
        while (i >= 0 && node->keys[i] > key)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->n = node->n + 1;
    }

    else
    {
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }

        i++;

        if (node->children[i]->n == MAX_KEYS)
        {
            splitChild(node, i);

            // after split, middle key moved up
            // decide which child should get the key

            if (node->keys[i] < key)
            {
                i++;
            }
        }

        insertNonFull(node->children[i], key);
    }
}

void insert(int key)
{
    // If tree is empty
    if (root == NULL)
    {
        root = createNode(true);
        root->keys[0] = key;
        root->n = 1;
        return;
    }

    // If root is full, tree grows in height
    if (root->n == MAX_KEYS)
    {
        BTreeNode *newRoot = createNode(false);

        // old root becomes child of new root
        newRoot->children[0] = root;

        // split old root
        splitChild(newRoot, 0);

        // decide which child will contain new key
        int i = 0;
        if (newRoot->keys[0] < key)
            i++;

        insertNonFull(newRoot->children[i], key);

        root = newRoot;
    }
    else
    {
        insertNonFull(root, key);
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
    if (i < root->n && root->keys[i] == key)
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

// int main()
// {
//     BTreeNode *root = createNode(true);
//     root->keys[0] = 10;
//     root->keys[1] = 20;
//     root->keys[2] = 30;
//     root->n = 3;
//     traverse(root);

//     BTreeNode *found = search(root, 40);

//     cout << endl;

//     if (found != NULL)
//     {
//         cout << "found" << endl;
//     }
//     else
//     {
//         cout << "not found" << endl;
//     }
//     return 0;
// }

int main()
{
    insert(10);
    insert(20);
    insert(5);
    insert(6);
    insert(12);
    insert(30);
    insert(7);
    insert(17);

    cout << "Traversal: ";
    traverse(root);

    cout << endl;

    int target = 12;
    BTreeNode *found = search(root, target);

    if (found != NULL)
        cout << target << " found\n";
    else
        cout << target << " not found\n";

    return 0;
}