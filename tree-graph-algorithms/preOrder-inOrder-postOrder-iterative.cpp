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
    cout << "---Preorder Traversal---" << endl;
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
    cout << endl;
}

void inOrderIterative(Node *root)
{
    vector<Node *> stk;
    Node *curr = root;
    cout << "---Inorder Traversal---" << endl;
    while (curr != nullptr || !stk.empty())
    {
        while (curr != nullptr)
        {
            stk.push_back(curr);
            curr = curr->left;
        }

        Node *printer = stk.back();
        stk.pop_back();
        cout << printer->data << " ";
        curr = printer->right;
    }

    cout << endl;
}

void postOrderIterative(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    vector<Node *> stk1;
    vector<Node *> stk2;

    stk1.push_back(root);

    while (!stk1.empty())
    {
        Node *current = stk1.back();
        stk1.pop_back();
        stk2.push_back(current);

        if (current->left != nullptr)
        {
            stk1.push_back(current->left);
        }

        if (current->right != nullptr)
        {
            stk1.push_back(current->right);
        }
    }

    cout << "---Postorder Traversal---" << endl;
    while (!stk2.empty())
    {
        cout << stk2.back()->data << " ";
        stk2.pop_back();
    }

    cout << endl;
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

    inOrderIterative(root);
    cout << endl;

    postOrderIterative(root);

    return 0;
}