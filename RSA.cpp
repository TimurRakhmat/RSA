#include "RSA.h"

void RSA::generate_keys(int len, int type, double ex)
{
	if (ex > 1 - eps or ex < 0.5)
		return; // TODO exception
	LongBuilder lb;
	LongNum p, q;

	lb.generate(len);
	p = LongNum(&lb);
	int min_count = 0;
	if (p.isEven())
		--p;
	do
	{
		double ex_f = 1;
		if (type == SOLOVAY)
		{
			while (ex_f > 1 - ex)
			{
				if (Solovay_Strassen(p) == 0)
					break;
				else
				{
					ex_f *= 0.5;
				}
			}
		}
		else // MILLER_RABIN
		{
			while (ex_f > 1 - ex)
			{
				if (Miller_Rabin(p) == 0)
					break;
				else
				{
					ex_f *= 0.25;
				}
			}
		}
		if (ex_f < 1 - ex)
			break;
		if (min_count < 50)
		{
			p = p - LongNum(2, true);
		}
		else if (min_count == 50)
			p = p + LongNum(100, true);
		else
		{
			p = p + LongNum(2, true);
		}
		min_count++;
	} while (1); // gen p

	min_count = 0;
	lb.generate(len);
	q = LongNum(&lb);
	if (q.isEven())
		--q;
	do
	{
		double ex_f = 1;
		if (type == SOLOVAY)
		{
			while (ex_f > 1 - ex)
			{
				if (Solovay_Strassen(q) == 0)
					break;
				else
				{
					ex_f *= 0.5;
				}
			}
		}
		else // MILLER_RABIN
		{
			while (ex_f > 1 - ex)
			{
				if (Miller_Rabin(q) == 0)
					break;
				else
				{
					ex_f *= 0.25;
				}
			}
		}
		if ((p - q).abs() < LongNum(10000, true))
		{
			q = q - LongNum(2000, true);
		}
		if (ex_f < 1 - ex)
			break;
		if (min_count < 50)
		{
			q = q + LongNum(2, true);
		}
		else if (min_count == 50)
			q = q - LongNum(100, true);
		else
		{
			q = q - LongNum(2, true);
		}
		min_count++;
	} while (1); // gen q


	n = new LongNum(p.mulKaratsuba(q));
	fn = new LongNum((--p).mulKaratsuba((--q)));
	do {
		int p = (rand() % (MAX - 1000)) + 1000;
		e = new LongNum(p, true);
		if (gcd(*e, *fn) == LongNum(1, true))
			break;
		delete e;
	} while (1);

	cout << gcd(*e, *fn) << endl;

	auto keys = fullgcd(*e, *fn);
	d = new LongNum(keys.first.first);
	cout << (*d).mulFourier(*e) << endl;
	cout << *fn << endl;
	cout << (*d).mulFourier(*e) % *fn << endl;
	cout << (keys.first.second.mulFourier(*e) % *fn) << endl;
	
	
}

LongNum RSA::set_info(const LongNum& ln)
{
	LongNum shifrt = ln.FastModDegree(*e, *n);
	return shifrt;
}

LongNum RSA::get_text(const LongNum& ln)
{
	LongNum shifrt = ln.FastModDegree(*d, *n);
	return shifrt;
}

RSA::~RSA()
{
	if (fn)
		delete fn;
	if (d)
		delete d;
	if (e)
		delete e;
	if (n)
		delete n;
}
