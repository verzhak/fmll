
#include "ann/base/weight_init.h"

double fmll_weight_init_null(fmll_random * not_used)
{
	return 0;
}

double fmll_weight_init_0_5(fmll_random * not_used)
{
	return 0.5;
}

double fmll_weight_init_random_0_1(fmll_random * rnd)
{
	return fmll_random_double_0_1(rnd);
}

double fmll_weight_init_random_0_01(fmll_random * rnd)
{
	return fmll_random_double_0_1(rnd) / 10;
}

double fmll_weight_init_random_0_001(fmll_random * rnd)
{
	return fmll_random_double_0_1(rnd) / 100;
}

double fmll_weight_init_random_m0_1_0_1(fmll_random * rnd)
{
	return (fmll_random_double_0_1(rnd) - 0.5) / 5.0;
}

