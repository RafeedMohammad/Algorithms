#include <iostream>
using namespace std;

void countingSort(int arr[], int n)
{
    int mx = arr[0];
    for (int i = 0; i < n; i++)
    {
        mx = max(mx, arr[i]);
    }
    // cout<<mx;
    int count[mx + 1] = {0};
    for (int i = 0; i < n; i++)
    {
        count[arr[i]]++;
    }

    cout << "Count Array" << endl;
    for (int i = 0; i <= mx; i++)
    {
        cout << count[i] << " ";
    }
    cout << endl;

    int index = 0;
    for (int i = 0; i <= mx; i++)
    {
        while (count[i] > 0)
        {
            arr[index++] = i;
            count[i]--;
        }
    }

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
}

int main()
{
    int arr[] = {4, 2, 2, 8, 3, 3, 1};
    int n = sizeof(arr) / sizeof(arr[0]);
    countingSort(arr, n);
}