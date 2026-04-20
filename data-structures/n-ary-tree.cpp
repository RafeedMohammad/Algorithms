#include <iostream>
#include <vector>
using namespace std;

struct TreeNode *GetNewNode(int data);
void addChild(TreeNode *node, int data);
void Print(TreeNode *root);
void recursivePrint(TreeNode *root);

struct TreeNode
{
    int data;
    // for binary tree
    //  TreeNode *left;
    //  TreeNode *right;

    // for n-ary tree
    vector<TreeNode *> children;
};

// Function to create a new node
TreeNode *GetNewNode(int data)
{
    TreeNode *newNode = new TreeNode();
    newNode->data = data;
    return newNode;
};

// Function to add a child to a node
void addChild(TreeNode *node, int data)
{
    TreeNode *newNode = GetNewNode(data);
    node->children.push_back(newNode);
}

void Print(TreeNode *root)
{
    cout << root->data << " ";
    cout << root->children[0]->data << " ";
    cout << root->children[1]->data << " ";
}

void recursivePrint(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    cout << root->data << " ";
    for (int i = 0; i < root->children.size(); i++)
    {
        recursivePrint(root->children[i]);
    }
}

int main()
{
    TreeNode *root;
    root = GetNewNode(3);
    addChild(root, 10);
    addChild(root, 20);
    recursivePrint(root);
}