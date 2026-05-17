/*
    circular queue formulas:
    1. rear = (rear+1) % n [Enqueue]
    2. front = (front+1) % n [Dequeue]

    Queue Full Condition:
    (rear+1) % n == front

    Queue Empty Condition:
    front = -1 OR front == rear

    Current number of elements:
    count = (n-front+rear) % n


    enqueue -> from rear
    dequeue -> from front

    rear -> next will point to front

*/

#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *next;
};

class CircularQeueue
{
private:
    Node *front;
    Node *rear;

public:
    CircularQeueue()
    {
        front = NULL;
        rear = NULL;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void enqueue(int val)
    {
        Node *newNode = new Node();
        newNode->data = val;
        newNode->next = NULL;

        if (isEmpty())
        {
            front = rear = newNode;
            rear->next = front;
        }

        else
        {
            rear->next = newNode;
            rear = newNode;
            rear->next = front;
        }

        cout << val << " enqueued" << endl;
    }

    int dequeue()
    {
        if (isEmpty())
        {
            cout << "The queue is empty! Nothing to dequeue" << endl;
            return -1;
        }

        int val;

        // case: only one element in the queue
        if (front == rear)
        {
            val = front->data;
            delete front;
            front = rear = NULL;
        }

        else
        {
            Node *temp = front;
            val = front->data;

            front = front->next;
            rear->next = front;
            delete temp;
        }

        cout << val << " dequeued" << endl;
        return val;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return;
        }

        Node *temp = front;

        cout << "Circular Queue: " << endl;
        do
        {
            cout << temp->data << " ";
            temp = temp->next;
        } while (temp != front);

        cout << endl;
    }
};

int main()
{
    CircularQeueue cq;

    cq.enqueue(29);
    cq.enqueue(15);
    cq.enqueue(10);
    cq.enqueue(12);
    cq.enqueue(35);
    cq.enqueue(40);
    cq.display(); // whole circular array

    cq.dequeue();
    cq.dequeue();
    cq.dequeue();
    cq.display(); // circular array after 3 dequeues

    cq.dequeue();
    cq.dequeue();
    cq.dequeue();
    cq.dequeue(); // to check if isEmpty() works inside dequeue
    cq.display(); // empty circular array
}