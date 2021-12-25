#include "LongNum.h"

int LongNum::Legendre(const LongNum& b) const
{
	if (LongNum(*this) % b == LongNum(0))
		return 0;

	LongNum p = (b - LongNum(1, true)) / LongNum(2, true);
	if (this->FastModDegree(p, b) == LongNum(1, true))
		return 1;
	return -1;

}

int LongNum::Jacobi(const LongNum& bb) const
{
	if (gcd(*this, bb) != LongNum(1, true))
		return 0;
	int r = -1;

	LongNum b(bb);
	LongNum a(*this);
	while (1) {
		int t = 0;
		while (a.isEven())
		{
			a = a / LongNum(2, true);
			t++;
		}
		if (t % 2 == 1)
		{
			LongNum ost = b % LongNum(8, true);
			if (ost == LongNum(3, true) or ost == LongNum(5, true))
				r = -r;
		}

		if (a % LongNum(4, true) == LongNum(3, true) and b % LongNum(4, true) == LongNum(3, true))
		{
			r = -r;
		}
		LongNum tmp(a);
		a = b % tmp;
		b = tmp;
		if (a == LongNum(0))
			return r;
	}
}

int LongNum::Jacobi2(const LongNum& aa) const
{
	if (this->isEven())
		return 0; // error even number
	LongNum n = *this, a = aa;
	if (a.isEven())
	{
		int k = 1;
		if (((n*n - LongNum(1, true)) / LongNum(8, true)).isOdd())
			k = -1;
		return k * n.Jacobi2(a / LongNum(2, true));
	}

	if (a == LongNum(1, true))
		return 1;
	if (a < n)
	{
		int k = 1;
		--n;
		--a;
		if ((n / LongNum(2, true)* (a / LongNum(2, true))).isOdd())
			k = -k;
		++n;
		++a;
		return k * a.Jacobi2(n);
	}

	return n.Jacobi2(a % n);
}

int LongNum::Fermat(const LongNum& a) const
{
	if (*this % a == LongNum(0, true))
		return -1;
	if (a.FastModDegree(*this - LongNum(1, true), *this) == LongNum(1, true))
		return 1;
	return 0;
}

int Solovay_Strassen(const LongNum& ln)
{
	if (ln.isEven())
		return 0;
	LongBuilder lb;
	LongNum a;
	do {
		int cnt = 0;
		if (ln.degree == 0)
		{
			cnt = log10(ln.num[0]);
		}
		else
		{
			cnt = ln.degree * 20;
		}
		lb.generate(cnt);
		a = LongNum(&lb);
	} while ((a > (ln - LongNum(1, true))) or (a < LongNum(3, true)));

	if (gcd(a, ln) > LongNum(1, true))
		return 0;
	if (a.FastModDegree((ln - LongNum(1, true)) / LongNum(2, true), ln) != LongNum(ln.Jacobi2(a), true))
		return 0;
	return 1;
}

int Miller_Rabin(const LongNum& ln)
{
	if (ln == LongNum(2))
		return true;
	if (ln < LongNum(2) || ln.isEven())
		return 0;
	LongBuilder lb;
	LongNum a;
	do {
		int cnt = 0;
		if (ln.degree == 0)
		{
			cnt = log10(ln.num[0]);
		}
		else
		{
			cnt = ln.degree * 20;
		}
		lb.generate(cnt);
		a = LongNum(&lb);
	} while ((a > (ln - LongNum(1, true))) or (a < LongNum(3, true)));

	/*for (LongNum tmp; (tmp = gcd(ln, a)) != LongNum(1); ++a)
		if (ln > tmp)
			return 0;*/

	LongNum s(0), d = ln;
	--d;
	while (d.isEven()) {
		d = d / LongNum(2, true);
		s++;
	}

	if (a.FastModDegree(d, ln) == LongNum(1, true) or a.FastModDegree(d, ln) == ln - LongNum(1, true))
		return 1;
	
	for (LongNum it(0); it < s; ++it)
	{
		LongNum pw = LongNum(2, true).FastDegree(it) * d;
		if (a.FastModDegree(pw, ln) == ln - LongNum(1))
			return 1;
	}

	return 0;
}
