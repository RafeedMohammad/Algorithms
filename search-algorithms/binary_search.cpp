#include<iostream>
using namespace std;

int binarySearch(int arr[], int n, int target)
{
    int low = 0;
    int high = n-1;

    while(low <= high)
    {
        int mid = low + (high-low)/2;
        if(arr[mid] == target)
        {
            return mid;
        }

        else if(arr[mid] < target)
        {
            low = mid+1;
        }

        else{
            high = mid-1;
        }
    }
}


int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target;
    cin>>target;
    int result = binarySearch(arr, n, target);
    cout<<result;

    return 0;
}