#include <iostream>
#include <stack>
#include <vector>
#include <climits>
using namespace std;

vector<int> arr = {5, 2, 8, 1, 3};
vector<int> L, R; // global sentinel

void merge(int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[low + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    int i = 0, j = 0;

    for (int k = low; k <= high; k++)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i++];
        }

        else
        {
            arr[k] = R[j++];
        }
    }
}

void mergeSortIterative()
{
    int n = arr.size();

    L.resize(n + 1);
    R.resize(n + 1);

    stack<pair<pair<int, int>, int>>
        stk;
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
            merge(low, mid, high);
        }
    }
}

int main()
{
    cout << "Unsorted Array:" << endl;
    for (int x : arr)
    {
        cout << x << " ";
    }
    cout << endl;

    mergeSortIterative();

    cout << "Sorted Array: " << endl;
    for (int x : arr)
    {
        cout << x << " ";
    }

    cout << endl;

    return 0;
}