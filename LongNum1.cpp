#include "LongNum.h"

LongNum LongNum::mulKaratsuba(const LongNum& ln) const
{
    int rl = degree;
    int ll = ln.degree;

    int maxI = max(rl, ll);

    while ((maxI+1) & maxI)
        maxI++;

    if (maxI == 0)
        return *this * ln;


    int k = (maxI + 1) / 2;

    LongNum r1, r2, l1, l2;
    //delete[] r1.num, r2.num, l1.num, l2.num;

    r1.num = new uint32_t[k];
    r2.num = new uint32_t[k];
    l1.num = new uint32_t[k];
    l2.num = new uint32_t[k];

    r1.degree = k - 1;
    l1.degree = k - 1;
    r2.degree = k - 1;
    l2.degree = k - 1;

    for (int i = 0; i < k; i++)
    {
        r1.num[i] = 0;
        r2.num[i] = 0;
        l1.num[i] = 0;
        l2.num[i] = 0;
    }

    for (int i = 0; i < k; i++)
    {
        if (rl >= i)
            r2.num[i] = num[i];
        if (ll >= i)
            l2.num[i] = ln.num[i];
        if (rl >= k + i)
            r1.num[i] = num[i + k];
        if (ll >= k + i)
            l1.num[i] = ln.num[i + k];
    }
    LongNum lln;
    lln.num = new uint32_t;
    *lln.num = 1;

    LongNum k1 = r1.mulKaratsuba(l1), k2 = r2.mulKaratsuba(l2), k3 = (r1 + r2).mulKaratsuba(l1 + l2);
    /*cout << k1 << endl;
    cout << k2 << endl;
    cout << k3 << endl;
    cout << (lln << 2*k) << endl;
    cout << (lln << k) << endl;
    LongNum res1(k1 * (lln << (2*k)));
    cout << res1 << endl;
    LongNum res2(k3 - k2 - k1);
    res2 = res2 * (lln << k);
    cout << res2 << endl;
    LongNum res3(k2);
    cout << res3 << endl;
    LongNum fulres(res1 + res2 + res3);
    return fulres;*/

    return k1 * (lln << (2 * k)) + (k3 - k2 - k1) * (lln << k) + k2;
}

LongNum LongNum::mulFourier(const LongNum&) const
{
	return LongNum();
}

LongNum LongNum::subNewton(const LongNum&) const
{
	return LongNum();
}

LongNum LongNum::FastDegree(long) const
{
	return LongNum();
}

LongNum LongNum::FastModDegree(long, long) const
{
	return LongNum();
}
