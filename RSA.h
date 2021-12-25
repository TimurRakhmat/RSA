#pragma once

#include "LongNum.h"


class RSA
{
private:
	LongNum* d = nullptr;
	LongNum* e = nullptr;

	LongNum* n = nullptr;
	LongNum* fn = nullptr;
	double eps = 0.02;
public:
	enum Tests {
		SOLOVAY,
		MILLER,
	};
	void generate_keys(int, int , double);
	LongNum set_info(const LongNum& ln);
	LongNum get_text(const LongNum& ln);
	~RSA();
};

