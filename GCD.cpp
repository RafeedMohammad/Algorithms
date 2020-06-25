#include<iostream>
using namespace std;

#define int 	long long

//weathering with you
//grave of the fire - flies 
int NaiveGCD(int a, int b)
{
	int best = 0;
	for(int i=1; i<a+b; i++)
	{
		if(a%i == 0 && b%i ==0)
		{
			best = i;
		}
	}
	
	return best;
}

int EuclidGCD(int a, int b)
{
	if(b == 0)
	{
		return a;
	}
	
	int a_prime = a % b;
	
	return EuclidGCD(b, a_prime);
}


int32_t main()
{
	int a,b;
	cin>>a>>b;
	cout<<EuclidGCD(a,b);

return 0;
}
