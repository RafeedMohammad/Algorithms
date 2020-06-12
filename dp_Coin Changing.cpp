//Coin changing problem

#include<iostream>
#include<climits>
using namespace std;


int minCoins(int n, int coins[], int T, int dp[])
{
	//Base Case
	if(n==0)
	{
		return n;
	}
	
	//Lookup
	if(dp[n] != 0)
	{
		return dp[n];
	}
	
	//Recursive Case
	int ans = INT_MAX;
	for(int i=0; i<T; i++)
	{
		if(n-coins[i] >= 0)
		{
			int subProb = minCoins(n-coins[i], coins, T, dp);
			ans = min(ans, subProb+1);
		}
		
	}
	
	dp[n] = ans;
	return dp[n];
}

int main()
{
	int N = 15;
	int coins[] = {1, 7, 10};
	int dp[100] = {0};
	int T = sizeof(coins)/sizeof(int);
	cout<<minCoins(N, coins, T, dp)<<endl;
	

return 0; 	
}

 
