#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *next;
};

Node *head = NULL;

void printList()
{
    Node *temp = head;

    while (temp != NULL)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }

    cout << endl;
}

void insertEnd(int value)
{
    Node *newNode = new Node();
    newNode->data = value;
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
        return;
    }

    Node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
}

void deleteNode(int value)
{
    if (head == NULL)
    {
        return;
    }

    if (head->data == value)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Node *prev = head;
    Node *curr = head->next;

    while (curr != NULL)
    {
        if (curr->data == value)
        {
            prev->next = curr->next;
            delete curr;
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

int main()
{
    insertEnd(10);
    insertEnd(20);
    insertEnd(30);
    deleteNode(20);

    cout << "Linked List: ";
    printList();

    return 0;
}