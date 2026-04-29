#include <iostream>
using namespace std;

int arr[] = {1, 4, 2, 10, 9, 12, 15};

void merge(int low, int mid, int high)
{
    int *temp = new int[high - low + 1];
    int i = low;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= high)
        temp[k++] = arr[j++];

    for (int x = 0; x < k; x++)
        arr[low + x] = temp[x];
}

void mergeSort(int low, int high)
{
    if (low >= high)
        return;

    int mid = low + (high - low) / 2;

    mergeSort(low, mid);
    mergeSort(mid + 1, high);

    merge(low, mid, high);
}

int main()
{
    int n = sizeof(arr) / sizeof(arr[0]);

    mergeSort(0, n - 1);

    cout << "Sorted Array:\n";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    return 0;
}