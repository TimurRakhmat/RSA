#include "LongNum.h"

uint32_t LongBuilder::rand_10_to_1000000() {
    // This method only works well when RAND_MAX is 2**n -1.
    // This is commonly true

    uint32_t r;
    do {
        r = rand();
#if RAND_MAX < 0x3FFFFFFF
        // Adding 1 to RAND_MAX as an int should be avoided to prevent int overflow.
        r *= RAND_MAX + (uint32_t)1;
        r += rand();
#endif
        r &= 0x3FFFFFFF; // only use lower 30 bits.
    } while (r < 0 || r > 1000000);
    return r;
}

void LongBuilder::generate(int deg)
{
	int p = deg / 20;
    if (!p)
    {
        p = 1;
        this->degree = p - 1;
        if (num != nullptr)
            delete[] num;
        num = new uint32_t[p];
        num[0] = rand() % (int)pow(10, deg);
        return;
    }
    this->degree = p - 1;
    if (num != nullptr)
        delete[] num;
	num = new uint32_t[p];
	for (int i = 0; i < p - 1; i++)
	{
		num[i] = rand_10_to_1000000();
	}
    auto h = rand_10_to_1000000();
    while (h == 0)
    {
        h = rand_10_to_1000000();
    }

    num[p - 1] = h;
}
