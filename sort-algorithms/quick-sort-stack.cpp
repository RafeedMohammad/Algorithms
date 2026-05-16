#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int partition(vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortStack(vector<int> &arr)
{
    int n = arr.size();
    if (n <= 1)
    {
        return;
    }

    stack<pair<int, int>> stk;
    stk.push({0, n - 1});

    while (!stk.empty())
    {
        int low = stk.top().first;
        int high = stk.top().second;
        stk.pop();

        if (low >= high)
        {
            continue;
        }

        int p = partition(arr, low, high);

        if (p - 1 > low)
        {
            stk.push({low, p - 1});
        }

        if (p + 1 < high)
        {
            stk.push({p + 1, high});
        }
    }
}

int main()
{
    vector<int> arr = {5, 2, 8, 1, 3};
    quickSortStack(arr);

    for (int x : arr)
    {
        cout << x << " ";
    }

    cout << endl;

    return 0;
}