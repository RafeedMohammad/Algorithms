#include <iostream>
#include <climits>
using namespace std;

int arr[] = {1, 4, 2, 10, 9, 12, 15};
int cnt = 0;
int inv = 0;

void merge(int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int leftArr[n1 + 1];
    int rightArr[n2 + 1];

    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = arr[low + i];
    }

    for (int i = 0; i < n2; i++)
    {
        rightArr[i] = arr[mid + 1 + i];
    }

    leftArr[n1] = INT_MAX;
    rightArr[n2] = INT_MAX;

    int i = 0, j = 0;

    for (int k = low; k <= high; k++)
    {
        cnt++;
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i++];
        }

        else
        {
            arr[k] = rightArr[j++];
            inv += (n1 - i);
        }
    }
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

    cout << "Sorted Array:" << endl;
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    cout << "Number of comparisons: " << cnt << endl;
    cout << "Number of inversions: " << inv;

    return 0;
}