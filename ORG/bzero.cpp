#include <iostream>
#include <string.h>
using namespace std;
int main()
{
	char s[] = "a";
	int n = 10;
	bzero(s, n);
	cout << s;
	return 0;
}
