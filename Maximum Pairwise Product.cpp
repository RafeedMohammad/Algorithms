#include<iostream>
#include<vector>
#include<cstdlib>
using namespace std;

#define int long long

int MaxPairWiseProduct(const vector<int>& numbers)
{
	int result = 0;
	int n = numbers.size();
	for(int i=0; i<n; i++)
	{
		for(int j=i+1; j<n; j++)
		{
			if(numbers[i] * numbers[j] > result)
			{
				result = numbers[i] * numbers[j];
			}
		}
	}
	
	return result;
}

int MaxPairWiseProductFast(const vector<int>& numbers)
{
	int  n = numbers.size();
	
	int max_index1 = -1;
	for(int i=0; i<n; i++)
	{
		if(max_index1 == -1 || (numbers[i] > numbers[max_index1]))
		{
			max_index1 = i;
		}
	}
	
	int max_index2 = -1;
	for(int j=0; j<n; j++)
	{
		if((j != max_index1) && ((max_index2 == -1) || (numbers[j] > numbers[max_index2])))
		{
			max_index2 = j;
		}
	}
	
	return (numbers[max_index1] * numbers[max_index2]);
}

int32_t main()
{
	int n;
	cin >> n;
	vector<int> numbers(n);
	
	for(int i=0; i<n; i++)
	{
		cin >> numbers[i];
	}
	
	int result = MaxPairWiseProductFast(numbers);
	cout << result <<endl;
	
return 0; 
}
