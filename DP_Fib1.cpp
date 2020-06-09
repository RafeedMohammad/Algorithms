#include<iostream>
using namespace std;

int fib(int n)
{
	if(n==1 || n==0)
	{
		return n;
	}
	
	int ans = fib(n-1) + fib(n-2); 
	return ans;
	
}


int main()
{
	int n;
	cin >> n;
	cout<<fib(n)<<endl;

return 0;	
}
