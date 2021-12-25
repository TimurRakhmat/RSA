#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <vector>
#include <complex>

const double M_PI = acos(-1);
constexpr uint64_t MAX = 1000000;

using namespace std;

typedef complex<double> cd;
typedef vector<cd> vcd;

class LongBuilder
{
	uint32_t* num = nullptr;
	long degree = 0;
	uint32_t rand_10_to_1000000();
public:
	void generate(int);
	friend class LongNum;
};

class LongNum
{
	bool sign = true;
	long degree=0;
	uint32_t* num;
	void reset(int);
public:
	LongNum operator-() const;
	LongNum operator+(const LongNum&)const;
	LongNum operator-(const LongNum&)const;
	LongNum operator*(const LongNum&)const;
	LongNum operator/(const LongNum&)const;
	LongNum operator*(const uint32_t n2)const;
	LongNum operator%(long);
	LongNum operator%(const LongNum&)const;
	LongNum operator<<(int) const;
	LongNum operator>>(int) const;
	LongNum operator++(int);
	LongNum& operator++();
	LongNum operator--(int);
	LongNum& operator--();

	bool isOdd() const;
	bool isEven() const;

	bool operator <(const LongNum&)const;
	bool operator >(const LongNum&)const;
	bool operator==(const LongNum&)const;
	bool operator!=(const LongNum&)const;
	bool operator<=(const LongNum&)const;
	bool operator>=(const LongNum&)const;

	LongNum abs()const;

	LongNum mulKaratsuba(const LongNum&)const;
	LongNum mulFourier(const LongNum&)const;
	LongNum FastDegree(long)const;
	LongNum FastDegree(const LongNum&)const;
	LongNum FastModDegree(const LongNum& p, const LongNum& n)const;

	int Legendre(const LongNum&) const;
	int Jacobi(const LongNum&) const; // bad Jacobi, remove
	int Jacobi2(const LongNum&) const;
	int Fermat(const LongNum&) const;

	LongNum();
	LongNum(int);
	LongNum(long long, bool);
	LongNum(const LongNum&);
	LongNum(string&, int);
	LongNum(LongBuilder *);
	LongNum& operator=(const LongNum&);
	~LongNum();

	friend ostream& operator<< (ostream& out, const LongNum& bin);
	friend LongNum gcd(const LongNum&, const LongNum&);
	friend pair<pair<LongNum, LongNum>, LongNum> fullgcd(const LongNum&, const LongNum&);
	friend LongNum bingcd(const LongNum&, const LongNum&);

	friend int Solovay_Strassen(const LongNum&);
	friend int Miller_Rabin(const LongNum&);
};

