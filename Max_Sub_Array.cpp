//Brute-Force Approach-1
// Time complexity = O(n^3)
//Naive approach


#include<iostream>
#include<climits>
using namespace std;

int ans = INT_MIN;

int Maximum_Sum_SubArray(int arr[], int n)
{
	for(int sub_array_size = 1; sub_array_size <= n; ++sub_array_size)
	{
		for(int start_index = 0; start_index < n; ++start_index)
		{
			if(start_index + sub_array_size > n)
			{
				break;
			}
			
		    int sum = 0;
		    for(int i = start_index; i < (start_index+sub_array_size); i++)
		    {
		    	sum += arr[i]; 
			}
			
			ans = max(ans, sum);
		}
		
	}
	
	return ans;
	
}



int main()
{
	
	int array[5] = {1,4,5,-2,3};
	
	int result = Maximum_Sum_SubArray(array, 5);
	cout<<"Maximum Sum: "<<ans;
	
	
	return 0;
}
