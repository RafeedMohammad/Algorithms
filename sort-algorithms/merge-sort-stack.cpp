#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void merge(vector<int> &arr, int low, int mid, int high)
{
    vector<int> temp;
    int i = low, j = mid + 1;

    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
        {
            temp.push_back(arr[i++]);
        }

        else
        {
            temp.push_back(arr[j++]);
        }
    }

    while (i <= mid)
    {
        temp.push_back(arr[i++]);
    }

    while (j <= high)
    {
        temp.push_back(arr[j++]);
    }

    for (int k = low; k <= high; k++)
    {
        arr[k] = temp[k - low];
    }
}

void mergeSortIterative(vector<int> &arr)
{
    int n = arr.size();
    stack<pair<pair<int, int>, int>> stk;
    stk.push({{0, n - 1}, 0});

    while (!stk.empty())
    {
        int low = stk.top().first.first;
        int high = stk.top().first.second;
        int state = stk.top().second;

        stk.pop();

        if (low >= high)
        {
            continue;
        }

        int mid = (low + high) / 2;

        if (state == 0)
        {
            stk.push({{low, high}, 1}); // will be merged at last

            stk.push({{mid + 1, high}, 0}); // right part

            stk.push({{low, mid}, 0}); // left part
        }

        else
        {
            merge(arr, low, mid, high);
        }
    }
}

int main()
{
    vector<int> arr = {5, 2, 8, 1, 3};
    mergeSortIterative(arr);

    for (int x : arr)
    {
        cout << x << " ";
    }

    cout << endl;

    return 0;
}