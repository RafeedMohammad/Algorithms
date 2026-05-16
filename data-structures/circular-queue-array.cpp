#include <iostream>
using namespace std;

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
*/

const int n = 5;
int arr[n];
int front = -1; // checkEmpty
int rear = -1;

bool isEmpty()
{
    return front == -1;
}

bool isFull()
{
    return (rear + 1) % n == front;
}

void enqueue(int val)
{
    if (isFull())
    {
        cout << "The cirucular queue is full!" << endl;
    }

    if (isEmpty())
    {
        front = 0;
        rear = 0;
    }

    else
    {
        rear = (rear + 1) % n;
    }

    arr[rear] = val;
    cout << val << " Enqueued!" << endl;
}

int dequeue()
{
    if (isEmpty())
    {
        cout << "The queue is empty, nothing to dequeue!" << endl;
        return -1;
    }

    int val = arr[front];

    if (front == rear)
    {
        front = -1;
        rear = -1;
    }

    else
    {
        front = (front + 1) % n;
    }

    cout << val << " dequeued" << endl;
}

int main()
{
}