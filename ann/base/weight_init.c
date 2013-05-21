
#include "ann/base/weight_init.h"

int simple_random(fmll_random * rnd, double ** w, const unsigned num, const unsigned dim)
{
	unsigned v, u;

	for(u = 0; u < num; u++)
		for(v = 0; v < dim; v++)
			w[u][v] = fmll_random_generate(rnd);

	return 0;
}

/* ############################################################################ */

int fmll_ff_weight_init_random(fmll_ff * ff, fmll_random * rnd)
{
	const unsigned char ** connect = (const unsigned char **) ff->connect;
	const unsigned num = ff->num, in_dim = ff->in_dim, * in = ff->in;
	unsigned v, u, t;
	double * b = ff->b, ** w = ff->w;

	for(v = 0, t = 0; v < num; v++)
	{
		if(t < in_dim && in[t] == v) /* Входные нейроны игнорируются */
			t++;
		else
		{
			b[v] = fmll_random_generate(rnd);

			for(u = 0; u < num; u++)
				if(connect[u][v])
					w[u][v] = fmll_random_generate(rnd);
		}
	}

	return 0;
}

int fmll_mlp_weight_init_random(fmll_mlp * mlp, fmll_random * rnd)
{
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, * N = mlp->N;
	unsigned u, v, t, q, N_u, prev_num;
	double ** w = mlp->w;

	for(u = t = 0, N_u = dim; u < layers_num; u++)
	{
		prev_num = N_u;
		N_u = N[u];

		for(v = 0; v < N_u; v++, t++)
			for(q = 0; q <= prev_num; q++)
				w[t][q] = fmll_random_generate(rnd);
	}

	return 0;
}

int fmll_som_weight_init_random(fmll_som * som, fmll_random * rnd)
{
	return simple_random(rnd, som->w, som->num, som->dim);
}

int fmll_pca_weight_init_random(fmll_pca * pca, fmll_random * rnd)
{
	return simple_random(rnd, pca->w, pca->num, pca->dim);
}

int fmll_mlp_weight_init_lecun(fmll_mlp * mlp, fmll_random * rnd)
{
	int ret = 0;
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, * N = mlp->N;
	unsigned u, v, t, q, N_u, prev_num;
	double tparam, param[2], ** w = mlp->w;

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

int fmll_mlp_weight_init_nguyen_widrow(fmll_mlp * mlp, fmll_random * rnd)
{
	int ret = 0;
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, * N = mlp->N;
	unsigned u, v, t, q, old_t, N_u, prev_num;
	double n, beta, ** w = mlp->w;

	fmll_try;

		fmll_throw_if(fmll_mlp_weight_init_random(mlp, rnd));

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

