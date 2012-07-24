
#include "ann/perceptron/weight_init.h"

int fmll_perceptron_weight_init_0(fmll_perceptron * perc, fmll_random * rnd)
{
	return fmll_perceptron_weight_init_random(perc, NULL);
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

int fmll_perceptron_weight_init_lecun(fmll_perceptron * perc, fmll_random * rnd)
{
	int ret = 0;
	unsigned u, v, t, q, N_u, prev_num, dim = perc->dim, layers_num = perc->layers_num, * N = perc->N;
	double tparam, param[2], ** w = perc->w;

	fmll_try;

		fmll_throw_if(rnd->dist != FMLL_RANDOM_DISTRIBUTION_UNIFORM);
		memcpy(param, rnd->param, sizeof(double) * 2);

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			tparam = sqrt(3. / prev_num);
			rnd->param[0] = - tparam;
			rnd->param[1] = tparam;

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					w[t][q] = fmll_random_generate(rnd);
		}

		memcpy(rnd->param, param, sizeof(double) * 2);

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

int fmll_perceptron_weight_init_nguyen_widrow(fmll_perceptron * perc, fmll_random * rnd)
{
	int ret = 0;
	unsigned u, v, t, q, old_t, N_u, prev_num, dim = perc->dim, layers_num = perc->layers_num, * N = perc->N;
	double n, beta, ** w = perc->w;

	fmll_try;

		fmll_throw_if(fmll_perceptron_weight_init_random(perc, rnd));

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];
		
			beta = 0.7 * pow(N_u, 1. / prev_num);

			for(v = 0, n = 0, old_t = t; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					n += w[t][q] * w[t][q];

			n = sqrt(n);

			for(v = 0, t = old_t; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					w[t][q] = beta * w[t][q] / n;
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

