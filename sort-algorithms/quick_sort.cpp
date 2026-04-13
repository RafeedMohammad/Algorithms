#include <iostream>
using namespace std;

int arr[] = {10, 7, 8, 9, 1, 5};

int partition(int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }

    int temp = arr[high];
    arr[high] = arr[i + 1];
    arr[i + 1] = temp;

    return i + 1;
}

void quickSort(int low, int high)
{
    if (low >= high)
    {
        return;
    }

    int p = partition(low, high);
    quickSort(low, p - 1);

    quickSort(p + 1, high);
}

int main()
{
    int n = sizeof(arr) / sizeof(arr[0]);

    quickSort(0, n - 1);

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }

    return 0;
}
