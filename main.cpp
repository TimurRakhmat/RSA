#include <iostream>
#include "LongNum.h"
typedef unsigned int uint;

using namespace std;

int main()
{
	LongBuilder lb;

	lb.generate(1024);

	string n = "1023423423409";
	LongNum n1(n, 10);
	lb.generate(1024);
	n = "900000879798723420001";
	LongNum n2(n, 10);
	cout << n1 << endl;
	cout << n2 << endl;
	LongNum g3( n1.mulKaratsuba(n2));
	cout << g3;
}