
#include "ann/ff/weight_init.h"

int fmll_ff_weight_init_random(fmll_ff * ff, fmll_random * rnd)
{
	unsigned v, u, num = ff->num;
	double * b = ff->b, ** w = ff->w;

	for(v = 0; v < num; v++)
	{
		b[v] = fmll_random_generate(rnd);

		for(u = 0; u < num; u++)
			w[v][u] = fmll_random_generate(rnd);
	}

	return 0;
}

