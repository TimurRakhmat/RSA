#pragma once
#include <fstream>
#include <string>
#include <iostream>

constexpr uint64_t MAX = 1000000000;

using namespace std;

class LongBuilder
{
	uint32_t* num = nullptr;
	long degree = 0;
	uint32_t rand_10_to_1000000000();
public:
	void generate(int);
	friend class LongNum;
};

class LongNum
{
	bool sign = true;
	long degree=0;
	uint32_t* num;
public:
	LongNum operator-() const;
	LongNum operator+(const LongNum&)const;
	LongNum operator-(const LongNum&)const;
	LongNum operator*(const LongNum&)const;
	LongNum operator*(const uint32_t n2)const;
	LongNum operator<<(int) const;

	bool operator <(const LongNum&)const;
	bool operator >(const LongNum&)const;
	bool operator==(const LongNum&)const;
	bool operator!=(const LongNum&)const;
	bool operator<=(const LongNum&)const;
	bool operator>=(const LongNum&)const;

	LongNum mulKaratsuba(const LongNum&)const;
	LongNum mulFourier(const LongNum&)const;
	LongNum subNewton(const LongNum&)const;
	LongNum FastDegree(long)const;
	LongNum FastModDegree(long, long)const;

	LongNum();
	LongNum(const LongNum&);
	LongNum(string&, int);
	LongNum(LongBuilder *);
	LongNum& operator=(const LongNum&);
	~LongNum();

	friend ostream& operator<< (ostream& out, const LongNum& bin);
};

