#include "LongNum.h"

void LongNum::reset(int ndeg)
{
	delete[] num;
	num = new uint32_t[ndeg + 1];
	for (int i = 0; i <= ndeg; i++)
	{
		num[i] = 0;
	}
}

LongNum LongNum::operator-() const
{
	LongNum tmp(*this);
	tmp.sign = !tmp.sign;
	return tmp;
}

LongNum LongNum::operator+(const LongNum& ln) const
{
	if (sign != ln.sign)
	{
		if (sign)
			return (*this - -ln);
		else
			return (ln - -*this);
	}
	long maxI = max(degree, ln.degree), minI = min(degree, ln.degree);
	LongNum b(maxI + 1);
	if (sign == ln.sign)
		b.sign = sign;
	uint64_t ans = 0, x = 0, y = 0;

	for (int i = 0; i <= minI; ++i)
	{
		x = num[i];
		y = ln.num[i];
		ans += x + y;
		b.num[i] = ans % MAX;
		ans /= MAX;
	}

	for (int i = minI + 1; i <= maxI; i++)
	{
		if (degree > ln.degree)
			ans += num[i];
		else
			ans += ln.num[i];
		b.num[i] = ans % MAX;
		ans /= MAX;
	}

	if (ans > 0)
	{
		b.degree = maxI + 1;
		b.num[maxI + 1] = ans;
	}
	else
		b.degree = maxI;

	while (b.num[b.degree] == 0 and b.degree > 0)
	{
		b.degree--;
	}

	return b;
}

LongNum LongNum::operator-(const LongNum& ln) const
{
	// need case (|n > ln|)
	if (sign != ln.sign) 
	{
		if (sign)
			return (*this + -ln); // case 3 - -1 -> 3 + 1 
		else
			return -(ln + -*this); // case -3 - 1 -> -(3 + 1) 
	}

	if (*this == ln)
	{
		LongNum resn(0);
		return resn; // case n - n == 0
	}
	if (sign and *this < ln) // case 4 - 5 -> -(5 - 4)
		return -(ln - *this);

	if (!sign and ln > *this) // case -4 - -2 -> -(4 - 2)
		return -(-*this - -ln);

	if (!sign)               //case -2 - -4 -> (4 - 2)
		return (-ln - -*this);


	//case n > n2

	int maxI = max(degree, ln.degree), minI = min(degree, ln.degree);
	LongNum b(maxI);
	b.degree = maxI;
	uint64_t ost = 0, a = 0, c = 0;
	for (int i = 0; i <= minI; i++)
	{
		if (num[i] < ost)
		{
			a = MAX;
			a -= ost;
			a += num[i];
			ost = 1;
		}
		else
		{
			a = num[i] - ost;
			ost = 0;
		}
		c = ln.num[i];
		if (a < c)
		{
			a += MAX;
			ost++;
		}
		b.num[i] = a - c;
	}

	for (int i = minI + 1; i <= maxI; i++)
	{
		if (num[i] < ost)
		{
			a = MAX;
			a -= ost;
			a += num[i];
			ost = 1;
		}
		else
		{
			a = num[i] - ost;
			ost = 0;
		}
		b.num[i] = a;
	}
	while (b.num[b.degree] == 0 and b.degree > 0)
	{
		b.degree--;
	}

	return b;
}

LongNum LongNum::operator*(const LongNum& ln) const
{
	if (ln == LongNum(0) or *this == LongNum(0))
		return LongNum(0);
	LongNum b(degree + 1 + ln.degree + 1);
	b.degree = degree + ln.degree + 1;
	for (int j = 0; j <= ln.degree; j++)
	{
		LongNum v(*this * ln.num[j]);
		v = v << j;
		b = b + v;
	}
	if (sign == ln.sign)
		b.sign = true;
	else
		b.sign = false;
	while (b.num[b.degree] == 0 and b.degree > 0)
	{
		b.degree--;
	}
	return b;
}

LongNum LongNum::operator*(const uint32_t n2) const
{
	if (n2 == 0) {
		return LongNum(0);
	}
	LongNum b(degree + 1);

	uint64_t ans = 0, ost = 0, bf = 0;
	for (int i = 0; i <= degree; i++)
	{
		bf = num[i];
		ans = bf * n2 + ost;
		ost = ans / MAX;
		b.num[i] = ans % MAX;
	}
	if (ost)
	{
		b.num[degree + 1] = ost;
		b.degree = degree + 1;
	}
	else
		b.degree = degree;

	while (b.num[b.degree] == 0 and b.degree > 0)
	{
		b.degree--;
	}

	return b;
}

LongNum LongNum::operator%(long deg)
{
	LongNum b(deg / MAX);
	unsigned long long ost = 0;
	for (int i = 0; i <= deg / MAX; i++)
	{
		ost *= MAX;
		ost += num[i];
	}
	for (int i = 0; i <= deg / MAX; i++)
	{
		b.num[i] = ost % deg;
		ost /= deg;
	}
	b.degree = deg / MAX;
	return b;
}

LongNum LongNum::operator%(const LongNum& ln) const
{
	LongNum res = *this - (*this / ln) * ln;
	if (res.sign == false)
		return res + ln;
	return res;
}

LongNum LongNum::operator<<(int n) const
{
	LongNum b(degree + n);
	b.sign = sign;
	b.degree = degree + n;
	for (int i = n; i <= n + degree; i++)
	{
		b.num[i] = num[i - n];
	}
	return b;
}

LongNum LongNum::operator>>(int n) const
{
	LongNum b(degree - n);
	b.sign = sign;
	b.degree = degree - n;
	if (b.degree <= 0) {
		b = LongNum(0);
		b.degree = 0;
	}
	for (int i = 0; i <= degree - n; i++)
	{
		b.num[i] = num[i + n];
	}
	return b;
}

LongNum LongNum::operator++(int)
{
	if (!sign)
		return -(--*this); // case ++(-4) -> -(--4);
	LongNum res(*this);
	uint64_t ans = 0, x = 1, y = 0;
	for (int i = 0; i <= degree; ++i)
	{
		x += num[i];
		ans += x;
		num[i] = ans % MAX;
		ans /= MAX;
		if (ans == 0)
			break;
		x = 0;
	}
	if (ans)
	{
		LongNum ntmp(degree + 1);
		for (int i = 0; i <= degree; i++)
		{
			ntmp.num[i] = num[i];
		}
		ntmp.num[degree + 1] = ans % MAX;
		delete[] num;
		num = ntmp.num;
		degree++;
		ntmp.num = nullptr;
		res = *this;
	}
	return res;
}

LongNum& LongNum::operator++()
{
	if (!sign)
	{
		*this = -(-- * this); // case ++(-4) -> -(--4);
		return *this;
	}


	uint64_t ans = 0, x = 1, y = 0;
	for (int i = 0; i <= degree; ++i)
	{
		x += num[i];
		ans += x;
		num[i] = ans % MAX;
		ans /= MAX;
		if (ans == 0)
			break;
		x = 0;
	}
	if (ans)
	{
		LongNum ntmp(degree + 1);
		for (int i = 0; i <= degree; i++)
		{
			ntmp.num[i] = num[i];
		}
		ntmp.num[degree + 1] = ans % MAX;
		delete[] num;
		num = ntmp.num;
		degree++;
		ntmp.num = nullptr;
	}
	return *this;
}

LongNum LongNum::operator--(int)
{
	if (!sign)
		return -((*this)++); // case (-4)-- -> -(4++);

	LongNum tmp(0), ans(*this);
	*tmp.num = 1;
	
	*this = *this - tmp;

	return ans;
}

LongNum& LongNum::operator--()
{
	if (!sign)
	{
		*this = -(++(*this)); // case --(-4) -> -(++4);
		return *this;
	}

	LongNum tmp(0);
	*tmp.num = 1;

	*this = *this - tmp;

	return *this;
}

bool LongNum::isOdd() const
{
	return(num[0] & 1) == 1;
}

bool LongNum::isEven() const
{
	return((num[0] & 1) == 0);
}

bool LongNum::operator<(const LongNum& ln) const
{
	if (ln.sign and !sign) // -n < n
		return true;
	if (sign and !ln.sign) // n < -n
		return false;

	if (sign) // (n and ln) > 0
	{
		if (degree < ln.degree)
			return true;
		if (degree > ln.degree)
			return false;

		for (int i = degree; i >= 0; i--)
		{
			if (ln.num[i] > num[i])
				return true;
			if (ln.num[i] < num[i])
				return false;
		}
		return false; // equal
	}

	// (n and ln) < 0
	if (degree < ln.degree)
		return false;
	if (degree > ln.degree)
		return true;

	for (int i = degree; i >= 0; i--)
	{
		if (ln.num[i] < num[i])
			return true;
		if (ln.num[i] > num[i])
			return false;
	}
	return false; // equal
}

bool LongNum::operator>(const LongNum& ln) const
{
	return !(*this <= ln);
}

bool LongNum::operator==(const LongNum& ln) const
{
	if (sign == ln.sign)
		if (degree == ln.degree)
		{
			for (int i = 0; i <= degree; i++)
			{
				if (num[i] != ln.num[i])
					return false;
			}
			return true;
		}
	return false;
}

bool LongNum::operator!=(const LongNum& ln) const
{
	return !(*this == ln);
}

bool LongNum::operator<=(const LongNum& ln) const
{
	return (*this < ln or *this == ln);
}

bool LongNum::operator>=(const LongNum& ln) const
{
	return !(*this < ln);
}

LongNum::LongNum()
{
	//*num = 0;
}

LongNum::LongNum(int deg)
{
	if (deg < 0)
		return;
	num = new uint32_t[deg + 1];
	for (int i = 0; i <= deg; i++)
	{
		num[i] = 0;
	}
}

LongNum::LongNum(long long n, bool b)
{
	bool ssign = true;
	if (n < 0)
	{
		n *= -1;
		ssign = false;
	}
	long long nn = n;
	int nl = 0;
	while (nn)
	{
		nl++;
		nn /= MAX;
	}
	num = new uint32_t[nl];
	for (int i = 0; i < nl; i++)
	{
		num[i] = n % MAX;
		n /= MAX;
	}
	degree = nl - 1;
	sign = ssign;
}

LongNum::LongNum(const LongNum& ln)
{
	try {
		num = new uint32_t[ln.degree + 1];
	}
	catch (bad_alloc)
	{
		cout << "bad_alloc error!!!\n";
	}
	degree = ln.degree;
	for (int i = 0; i <= degree; i++)
	{
		num[i] = ln.num[i];
	}
	sign = ln.sign;
}

string sum(string tmp, int n)
{
	int pos = 0, ost = 0, a = 0, b = 0;
	while (n and pos < tmp.length())
	{
		a = tmp[pos] - '0';
		a += n % 10 + ost;

		ost = a / 10;
		a %= 10;
		tmp[pos] = a + '0';

		n /= 10;
		pos++;
	}

	if (pos < (tmp.length()) and n == 0)
	{
		while (pos < (tmp.length()))
		{
			a = tmp[pos] - '0';
			a += ost;

			ost = a / 10;
			a %= 10;
			tmp[pos] = a + '0';
			pos++;
			if (ost == 0)
				break;
		}
	}

	if (pos >= (tmp.length()) and n != 0)
	{
		while (n)
		{
			a = n % 10 + ost;
			ost = a / 10;
			a %= 10;
			tmp += char(a + '0');
			pos++;
			n /= 10;
		}
	}

	if (ost > 0)
	{
		a = ost;
		tmp += char(a + '0');
		pos++;
	}
	return tmp;
}

string sum(string r, string l)
{
	int rl = r.length();
	int ll = l.length();

	int ost = 0, buf = 0;
	string ans = "";

	int minl = min(rl, ll);
	for (int i = 0; i < minl; i++)
	{
		buf = r[i] - '0' + l[i] - '0' + ost;
		ost = buf / 10;
		buf %= 10;
		ans += (char)(buf + '0');
	}
	int maxl = max(rl, ll);
	for (int i = minl; i < maxl; i++)
	{
		buf = ost;
		if (rl == maxl)
			buf += r[i] - '0';
		else
			buf += l[i] - '0';
		ost = buf / 10;
		buf %= 10;
		ans += (char)(buf + '0');
	}

	while (ost > 0)
	{
		buf = ost;
		ost = buf / 10;
		buf %= 10;
		ans += (char)(buf + '0');
	}
	return ans;
}

string mul(string tmp, int n)
{
	string ans = "", buf = "";
	for (int i = 0; i < tmp.length(); i++)
	{
		int sa = 0;
		sa = n * (tmp[i] - '0');
		buf = to_string(sa);
		reverse(buf.begin(), buf.end());

		for (int j = 0; j < i; j++)
		{
			buf = '0' + buf;
		}
		ans = sum(ans, buf);
	}

	return ans;
}

LongNum::LongNum(std::string& line, int deg)
{
	string res = "";
	for (auto it = line.begin(); it != line.end(); it++)
	{
		int buf = (int)(isdigit((*it)) ? (*it) - '0' : toupper(*it) - 'A' + 10);
		if (res != "")
			res = mul(res, deg);

		res = sum(res, buf);
	}
	if (res == "")
		res = "0";

	int pos = 0;
	int isize = res.length() / 6;
	if (res.length() % 6 != 0)
		isize++;

	num = new uint32_t[isize];
	for (int i = 0; i < isize - 1; i++)
	{
		string tmps = res.substr(i * 6, 6);
		reverse(tmps.begin(), tmps.end());
		num[i] = atoi(tmps.c_str());
	}

	string tmps = res.substr(6* (isize - 1));
	reverse(tmps.begin(), tmps.end());
	num[isize - 1] = atoi(tmps.c_str());
	degree = isize - 1;
}

LongNum::LongNum(LongBuilder* lb)
{
	try {
		num = new uint32_t[lb->degree + 1];
	}
	catch (bad_alloc)
	{
		cout << "bad_alloc error!!!\n";
	}
	degree = lb->degree;
	for (int i = 0; i <= degree; i++)
	{
		num[i] = lb->num[i];
	}
	sign = true;
}

LongNum& LongNum::operator=(const LongNum& ln)
{
	if (num != nullptr)
	{
		delete[] num;
	}
	try {
		num = new uint32_t[ln.degree + 1];
	}
	catch (bad_alloc)
	{
		cout << "bad_alloc error!!!\n";
	}
	degree = ln.degree;
	for (int i = 0; i <= degree; i++)
	{
		num[i] = ln.num[i];
	}
	sign = ln.sign;
	return *this;
}

LongNum::~LongNum()
{
	if (num)
	{		
		if (degree > 0)
			delete[] num;
		else
			delete num;
	}
}

ostream& operator<<(ostream& out, const LongNum& bin)
{
	if (bin.sign == false)
	{
		out << '-';
	}

	for (int i = bin.degree; i >= 0; i--)
	{
		if (i == bin.degree)
		{
			uint32_t tmp2 = bin.num[i];
			out << bin.num[i];
			continue;
		}

		uint32_t tmp = bin.num[i];
		int len = 6;
		while (tmp > 0)
		{
			len--;
			tmp /= 10;
		}
		
		for (int j = 0; j < len; j++)
		{
			out << '0';
		}
		if (bin.num[i] != 0)
			out << bin.num[i];
	}
	return out;
}
