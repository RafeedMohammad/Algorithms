#include <iostream>
#include <vector>
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

void preOrderIterative(Node *root)
{
    vector<Node *> stk;
    stk.push_back(root);

    while (!stk.empty())
    {
        Node *current = stk.back();
        stk.pop_back();
        cout << current->data << " ";

        if (current->right != nullptr)
        {
            stk.push_back(current->right);
        }

        if (current->left != nullptr)
        {
            stk.push_back(current->left);
        }
    }
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

    preOrderIterative(root);
    cout << endl;

    return 0;
}