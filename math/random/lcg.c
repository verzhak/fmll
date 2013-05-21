
#include "math/random/random.h"

/* Линейный конгруэнтный метод */

int lcg_init(fmll_random * rnd, const unsigned long seed)
{
	srand(seed);
	rnd->max = (double) RAND_MAX;

	return 0;
}

double lcg_generate(fmll_random * rnd)
{
	return rand();
}

