#include "LongNum.h"

LongNum LongNum::mulKaratsuba(const LongNum& ln) const
{
    if (ln == LongNum(0) or *this == LongNum(0))
        return LongNum(0);
    int rl = degree;
    int ll = ln.degree;

    int maxI = max(rl, ll);

    while ((maxI+1) & maxI)
        maxI++;

    if (maxI == 0)
        return *this * ln;


    int k = (maxI + 1) / 2;

    //optimize variant
    LongNum r1(-1), r2(-1), l1(-1), l2(-1);
    {
        if (rl >= k)
        {
            r1.num = num + k;
            r1.degree = rl - k;
        }
        else
        {
            r1 = LongNum(0);
        }
        if (ll >= k)
        {
            l1.num = ln.num + k;
            l1.degree = ll - k;
        }
        else
        {
            l1 = LongNum(0);
        }
        r2.num = num;
        if (rl >= k)
        {
            r2.degree = k - 1;
        }
        else
        {
            r2.degree = rl;
        }
        l2.num = ln.num;
        if (ll >= k)
        {
            l2.degree = k - 1;
        }
        else
        {
            l2.degree = ll;
        }
    }


    //safe variant
    /*LongNum r1(k-1), r2(k-1), l1(k-1), l2(k-1);

    r1.degree = k - 1;
    l1.degree = k - 1;
    r2.degree = k - 1;
    l2.degree = k - 1;

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
    }*/


    LongNum k1 = r1.mulKaratsuba(l1), k2 = r2.mulKaratsuba(l2), k3 = (r1 + r2).mulKaratsuba(l1 + l2);

    //optimize variant
    {
        l2.num = nullptr;
        l2.degree = 0;
        r2.num = nullptr;
        r2.num = 0;
        if (rl >= k)
        {
            r1.num = nullptr;
            r1.degree = 0;
        }
        if (ll >= k)
        {
            l1.num = nullptr;
            l1.degree = 0;
        }
    }

    return (k1  << (2*k)) + ((k3 - k2 - k1)  << k) + k2;
}

vcd fft(const vcd& as) {
    int n = as.size();
    int k = 0; // Длина n в битах
    while ((1 << k) < n) k++;
    vector<int> rev(n);
    rev[0] = 0;
    int high1 = -1;
    for (int i = 1; i < n; i++) {
        if ((i & (i - 1)) == 0) // Проверка на степень двойки. Если i ей является, то i-1 будет состоять из кучи единиц.
            high1++;
        rev[i] = rev[i ^ (1 << high1)]; // Переворачиваем остаток
        rev[i] |= (1 << (k - high1 - 1)); // Добавляем старший бит
    }

    vcd roots(n);
    for (int i = 0; i < n; i++) {
        double alpha = 2 * M_PI * i / n;
        roots[i] = cd(cos(alpha), sin(alpha));
    }

    vcd cur(n);
    for (int i = 0; i < n; i++)
        cur[i] = as[rev[i]];

    for (int len = 1; len < n; len <<= 1) {
        vcd ncur(n);
        int rstep = roots.size() / (len * 2);
        for (int pdest = 0; pdest < n;) {
            int p1 = pdest;
            for (int i = 0; i < len; i++) {
                cd val = roots[i * rstep] * cur[p1 + len];
                ncur[pdest] = cur[p1] + val;
                ncur[pdest + len] = cur[p1] - val;
                pdest++, p1++;
            }
            pdest += len;
        }
        cur.swap(ncur);
    }
    return cur;
}

vcd fft_rev(const vcd& as) {
    vcd res = fft(as);
    for (int i = 0; i < (int)res.size(); i++) res[i] /= as.size();
    reverse(res.begin() + 1, res.end());
    return res;
}

LongNum LongNum::mulFourier(const LongNum& ln) const
{
    if (ln == LongNum(0) or *this == LongNum(0))
        return LongNum(0);

    int maxI = max(degree, ln.degree);
    int k = 0;
    while ((1 << k) < (maxI + 1)) k++;
    vcd a(1 << (k + 1)), b(1 << (k + 1));
    for (int i = 0; i <= degree; i++)
    {
        a[i] = num[i];
    }
    for (int i = 0; i <= ln.degree; i++)
    {
        b[i] = ln.num[i];
    }
    for (int i = (degree + 1); i < (2 << k); i++)
        a[i] = 0;
    for (int i = (ln.degree + 1); i < (2 << k); i++)
        b[i] = 0;

    a = fft(a);
    b = fft(b);
    for (int i = 0; i < (2 << k); i++)
    {
        a[i] *= b[i];
    }
    a = fft_rev(a);
	LongNum res(a.size() - 1);
    res.degree = (a.size() - 1);
    long long ost = 0;
    for (int i =  0; i < (2 << k); i++)
    {
        long long tmp = 0;
        tmp += (floor(a[i].real()));
        double tmpd = floor(a[i].real()) + 0.5;
        if (tmpd < a[i].real())
            tmp += 1;
        if (tmp < 0)
            tmp = 0;
        tmp += ost;
        long long tmpr = tmp % MAX;
        res.num[i] = tmpr;
        ost = tmp / MAX;
    }

    while (res.num[res.degree] == 0 and res.degree > 0)
    {
        res.degree--;
    }
    return res;
}

LongNum LongNum::operator/(const LongNum& ln) const
{
    if (ln == LongNum(0))
        throw exception("Division by zero");

    if (*this < ln)
        return LongNum(0);

    if (*this == ln)
        return LongNum(1, true);

    LongNum _other(ln);
    bool ssign = sign == ln.sign;
    _other.sign = true;

    int iter_count = 0;
    if (this->degree - ln.degree > 1)
        iter_count = std::ceil(log2(degree - ln.degree));
    iter_count += 2;


    unsigned long long _tmp = MAX * MAX * MAX;
    unsigned long long _tmp2 = MAX * (ln.num[ln.degree]);
    if (ln.degree > 0)
        _tmp2 += ln.num[ln.degree - 1];
    _tmp /= _tmp2;

    LongNum x(_tmp, true);

    unsigned long long n = ln.degree + 2;
    for (int i = 0; i != iter_count; i++)
    {
        LongNum resTmp(-1);
        resTmp = LongNum(2, true) << n;
        resTmp = resTmp - (_other * x);
        x = resTmp * x;
        n *= 2;
        unsigned int best_n = (1 << i) + ln.degree + 2;
        x = x >> (n - best_n);
        n = best_n;
    }

    x = x * *this;
    x = x >> n;

    if ((x) * _other > *this)
        x--;
    x.sign = ssign;
    while (x.num[x.degree] == 0 and x.degree > 0)
    {
        x.degree--;
    }
    return x;
}

LongNum LongNum::FastDegree(long p) const
{
    if (p == 0) {
        LongNum ans(0);
        *ans.num = 1;
        return ans;
    }
    if (p % 2 == 0) {
        return (*this * *this).FastDegree(p >> 1);
    }
    return *this * (this->FastDegree(p - 1));
};

LongNum LongNum::FastDegree(const LongNum& p) const
{
    if (p == LongNum(0)) {
        LongNum ans(0);
        *ans.num = 1;
        return ans;
    }
    if (p.isEven()) {
        return (*this * *this).FastDegree(p / LongNum(2, true));
    }
    return *this * (this->FastDegree(p - LongNum(1, true)));
}

LongNum LongNum::FastModDegree(const LongNum& p,const LongNum& n) const
{

    if (p == LongNum(0)) {
        return LongNum(1, true);
    }

    LongNum ans(*this);
    ans = ans % n;

    if ((p % LongNum(2, true)) == LongNum(0)) {
        LongNum res = (ans * ans).FastModDegree(p / LongNum(2, true), n) % n;
        return res;
    }
    LongNum res = (ans * (ans.FastModDegree(p - LongNum(1, true), n))) % n;
    return res;
}

LongNum gcd(const LongNum& r, const LongNum& l)
{
    LongNum rr(r), ll(l);
    while (rr != LongNum(0) and ll != LongNum(0))
    {
        if (rr > ll)
        {
            rr = rr % ll;
        }
        else
            ll = ll % rr;
    }
    return rr + ll;
}

pair<pair<LongNum, LongNum>, LongNum> fullgcd(const LongNum& a, const LongNum& b)
{
    if (a == LongNum(0)) {
        LongNum x(0, true), y(1, true);
        return make_pair(make_pair(x, y), b);
    }
    LongNum x1, y1, d;
    auto ppr = fullgcd(b % a, a);
    x1 = ppr.first.second - (b / a) * ppr.first.first;
    y1 = ppr.first.first;
    return make_pair(make_pair(x1, y1), ppr.second);
}

LongNum bingcd(const LongNum& a, const LongNum& b)
{
    if (a == b || a == 0)
        return b;

    if (b == 0)
        return a;

    if (a.isEven())
    {
        return b.isOdd()
            ? bingcd(a / LongNum(2, true), b)        // TODO div on longlong
            : bingcd(a / LongNum(2, true), b / LongNum(2, true)) * 2;
    }
    else if (b.isEven())
    {
        return bingcd(a, b / LongNum(2, true));
    }
    else
    {
        return a > b
            ? bingcd((a - b) / LongNum(2, true), b)
            : bingcd((b - a)/LongNum(2, true), a);
    }
}
