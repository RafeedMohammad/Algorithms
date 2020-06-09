#include<iostream>
using namespace std;

int fib(int n, int dp[])
{
	if(n==1 || n==0)
	{
		return n;
	}
	
	//Recursion
	if(dp[n] != 0)
	{
		return dp[n];
	}
	
	int ans = fib(n-1, dp) + fib(n-2, dp); 
	return dp[n] = ans;
	
}

int fibBU(int n)
{
	int dp[100] = {0};
	dp[1] = 1;
	
	for(int i=2; i<=n; i++)
	{
		dp[i] = dp[i-1] + dp[i-2];
	}
	
	return dp[n];
}


int main()
{
	int n;
	cin >> n;
	//int dp[100] = {0};
	//cout<<fib(n, dp)<<endl;
	//Bottom-Up Approach
	cout << fibBU(n) << endl;

return 0;	
}
