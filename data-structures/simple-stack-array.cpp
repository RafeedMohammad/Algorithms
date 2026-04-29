#include <iostream>
using namespace std;
#define MAX 1000

int st[MAX];
int top = -1;

int isEmpty()
{
    return top == -1;
}

int isFull()
{
    return top == MAX - 1;
}

void push(int x)
{
    if (isFull())
    {
        cout << "Stack Overflow" << endl;
        return;
    }

    st[++top] = x;
}

int pop()
{
    if (isEmpty())
    {
        cout << "Stack underflow" << endl;
        return -1;
    }
    cout << "pop operation called" << endl;
    return st[top--];
}

int peek()
{
    if (isEmpty())
    {
        return -1;
    }

    return st[top];
}

void printStack()
{

    for (int i = top; i >= 0; i--)
    {
        cout << st[i] << " ";
    }
    cout << endl;
}

int main()
{
    push(20);
    push(40);
    push(10);
    cout << "top element: " << peek() << endl;
    printStack();
    pop();
    printStack();

    return 0;
}