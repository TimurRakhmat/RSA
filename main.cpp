#include <iostream>
#include "RSA.h"
typedef unsigned int uint;

using namespace std;

pair<pair<int, int>, int> get()
{
	return make_pair(make_pair(3, 4), 5);
}


int main()
{
	/*LongBuilder lb;

	lb.generate(2048);

	string n = "990077777777777777777777777009000001";
	LongNum n1(&lb);*/

	/*lb.generate(160);
	n        = "9999999999999999999000001";
	LongNum n2(&lb);
	cout << n1 << endl;
	cout << n2 << endl;
	LongNum g3( n1.mulKaratsuba(n2));
	cout << g3 << endl;
	LongNum g4(n1.mulFourier(n2));
	
	cout << g4 << endl;
	cout << n1 * n2 << endl;
	cout << g4 - g3 << endl;
	cout << n1*n2 - g3 << endl;*/
	/*LongNum n5(1223, true), n6(121, true);
	LongNum n7(5, true);
	LongNum n8(19, true);
	LongNum n9(221, true);*/

	
	//cout << LongNum(560, true).Fermat(LongNum(3559, true)) << endl;
	//cout << LongNum(5, true).Legendre(LongNum(19, true)) << endl;
	//cout << LongNum(19, true).Jacobi2(LongNum(7, true)) << endl;
	//cout << LongNum(19, true).Jacobi2(LongNum(5, true)) << endl;
	//cout << n9.FastModDegree((n8 - LongNum(1, true)) / LongNum(2, true), n8) << endl;
	//cout << Solovay_Strassen(n8) << endl;
	//cout << Miller_Rabin(n9) << endl;

	//n = "5555555557000000";
	//LongNum n3(n, 10);
	//n = "8000000000000";
	//LongNum n4(n, 10);
	////cout << n1 << endl;
	//auto ppr = fullgcd(n3, n4);
	//cout << bingcd(n3, n4)  << endl;
	//cout << ppr.first.first << " -  " << ppr.first.second << " - " << ppr.second << endl;

	srand(static_cast<unsigned int>(time(NULL)));
	RSA rsa;
	rsa.generate_keys(60, RSA::SOLOVAY, 0.9);

	string tx = "115359189341";
	LongNum n1(tx, 10);
	LongNum res = rsa.set_info(n1);
	cout << "to encrypt: \t" <<  n1 << endl;
	cout << "res after crypt: \t" << res << endl;
	cout << "after decrypt: \t" << rsa.get_text(res) << endl;
}