#include <iostream>
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

Node *stck[10];

Node *stck1[10];
Node *stck2[10];

void preOrderIterative(Node *root)
{
    stck[0] = nullptr;
    int top = 1;
    Node *ptr = root;

    while (ptr != nullptr)
    {
        cout << ptr->data << "  ";

        if (ptr->right != nullptr)
        {
            stck[top++] = ptr->right;
        }

        if (ptr->left != nullptr)
        {
            ptr = ptr->left;
        }

        else
        {
            ptr = stck[--top];
        }
    }
}

void inOrderIterative(Node *root)
{
    stck[0] = nullptr;
    int top = 1;
    Node *ptr = root;

    while (ptr != nullptr || top > 1)
    {
        while (ptr != nullptr)
        {
            stck[top++] = ptr;
            ptr = ptr->left;
        }

        ptr = stck[--top];

        if (ptr != nullptr)
        {
            cout << ptr->data << "  ";
            ptr = ptr->right;
        }
    }
}

void postOrderIterative(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    int top1 = 0;
    int top2 = 0;

    stck1[top1++] = root;

    while (top1 > 0)
    {
        Node *curr = stck1[--top1];
        stck2[top2++] = curr;

        if (curr->left != nullptr)
        {
            stck1[top1++] = curr->left;
        }

        if (curr->right != nullptr)
        {
            stck1[top1++] = curr->right;
        }
    }

    while (top2 > 0)
    {
        Node *curr = stck2[--top2];
        cout << curr->data << "  ";
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

    cout << "Preorder Traversal: " << endl;
    preOrderIterative(root);
    cout << endl;

    cout << "Inorder Traversal: " << endl;

    inOrderIterative(root);

    cout << endl;

    cout << "Inorder Traversal: " << endl;

    postOrderIterative(root);

    return 0;
}

/*

          1
        /   \
       2     3
      / \   / \
     4   5 6   7

*/