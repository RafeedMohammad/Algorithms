#include <iostream>
using namespace std;

int main()
{
    int a = 10, b = 7;
    b = a + b;
    a = b - a;
    b = b - a;
    cout << a << " " << b;
}