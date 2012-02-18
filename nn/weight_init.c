
#include "nn/weight_init.h"

double fmll_weight_init_null()
{
	return 0;
}

double fmll_weight_init_0_5()
{
	return 0.5;
}

double fmll_weight_init_random_0_1()
{
	return 0; // TODO drand48();
}

double fmll_weight_init_random_0_01()
{
	return 0; // TODO drand48() / 10;
}

double fmll_weight_init_random_0_001()
{
	return 0; // TODO drand48() / 100;
}

double fmll_weight_init_random_m0_1_0_1()
{
	return 0; // TODO (drand48() - 0.5) / 5.0;
}

