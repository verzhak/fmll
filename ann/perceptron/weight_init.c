
#include "ann/perceptron/weight_init.h"

int fmll_perceptron_weight_init_0(fmll_perceptron * perc, fmll_random * rnd)
{
	int ret = 0;
	unsigned u, v, t, q, N_u, prev_num, dim = perc->dim, layers_num = perc->layers_num, * N = perc->N;
	double ** w = perc->w;

	fmll_try;

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					w[t][q] = 0;
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

int fmll_perceptron_weight_init_random(fmll_perceptron * perc, fmll_random * rnd)
{
	int ret = 0;
	unsigned u, v, t, q, N_u, prev_num, dim = perc->dim, layers_num = perc->layers_num, * N = perc->N;
	double ** w = perc->w;

	fmll_try;

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					w[t][q] = fmll_random_generate(rnd);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

