#include <iostream>
#include <cstdlib>
using namespace std;

struct Stack
{
    int top;
    int cap;
    int *arr;
};

Stack *createStack(int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->cap = capacity;
    stack->top = -1;
    stack->arr = (int *)malloc(stack->cap * sizeof(int));
    return stack;
}

bool isFull(Stack *stack)
{
    return stack->top == stack->cap - 1;
}

bool isEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, int item)
{
    if (isFull(stack))
    {
        cout << "Stack is full" << endl;
    }

    stack->top++;
    stack->arr[stack->top] = item;
}

int pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        cout << "Stack is empty" << endl;
    }

    int item = stack->arr[stack->top--];
    return item;
}

int peek(Stack *stack)
{
    if (isEmpty(stack))
    {
        cout << "Stack is empty" << endl;
    }

    return stack->arr[stack->top];
}

int main()
{
    Stack *stack = createStack(5);
    push(stack, 1);
    push(stack, 2);
    push(stack, 3);
    push(stack, 4);
    push(stack, 5);
    cout << peek(stack) << endl;
    cout << pop(stack) << endl;
    cout << peek(stack) << endl;
}
