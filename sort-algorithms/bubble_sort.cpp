/*Bubble Sort*/

#include<iostream>
using namespace std;

void  bubbleSort(int array[], int n)
{
	for(int i = 1; i <= n-1; i++)
	{
		for(int j = 0; j <= (n-i-1); j++)
		{
			if(array[j] > array[j+1])
			{
				swap(array[j], array[j+1]);
				
			}
		}
	}
	
	
}

int main()
{
	int a[5] = {4,2,5,1,3};
	
	cout<<"Unsorted Array: ";
	for(int i=0; i <= 4; i++)
	{
		cout<<a[i]<<" ";
	}
	
	cout<<endl;
	
	
	bubbleSort(a, 5);
	
	
	cout<<"Sorted Array: ";
	for(int i=0; i <= 4; i++)
	{
		cout<<a[i]<<" ";
	}
	
return 0;	
}
