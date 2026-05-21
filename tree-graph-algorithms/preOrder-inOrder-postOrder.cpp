#include <iostream>
#include <queue>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;

    Node(int data)
    {
        this->data = data;
        this->left = nullptr;
        this->right = nullptr;
    }
};

void preOrder(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    cout << root->data << " ";

    preOrder(root->left);
    preOrder(root->right);
}

int main()
{
    Node *root = new Node(1);
    Node *leftChild = new Node(2);
    Node *rightChild = new Node(3);

    root->left = leftChild;
    root->right = rightChild;

    Node *node4 = new Node(4);
    Node *node5 = new Node(5);

    leftChild->left = node4;
    leftChild->right = node5;

    Node *node6 = new Node(6);
    Node *node7 = new Node(7);

    rightChild->left = node6;
    rightChild->right = node7;

    preOrder(root);
    cout << endl;

    return 0;
}