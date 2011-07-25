
#include "perceptron.h"

fmll_perceptron * fmll_perceptron_init(uint8_t dim, uint8_t layers_num, const uint16_t * N,
		double (* weight_init)(), double (** fun)(double), double (** d_fun)(double))
{
	fmll_try;

		fmll_perceptron * perc = NULL;
		double (** t_fun)(double);
		double (** t_d_fun)(double);
		double ** w;
		uint16_t max_N, N_u, prev_num, * t_N;
		uint32_t u, v, t, q, num, size = layers_num * sizeof(double (*)(double));

		fmll_throw_null((perc = fmll_alloc_1D(1, sizeof(fmll_perceptron))));

		perc->fun = perc->d_fun = NULL;
		perc->N = NULL;
		perc->y = NULL;
		perc->t_y = NULL;
		perc->w = NULL;

		fmll_throw_null((t_fun = perc->fun = fmll_alloc_1D(layers_num, sizeof(double (*)(double)))));
		fmll_throw_null((t_d_fun = perc->d_fun = fmll_alloc_1D(layers_num, sizeof(double (*)(double)))));
		fmll_throw_null((t_N = perc->N = fmll_alloc_1D(layers_num, sizeof(uint16_t))));

		for(u = 1, num = max_N = N[0]; u < layers_num; u++)
		{
			num += N[u];

			if(max_N < N[u])
				max_N = N[u];
		}

		fmll_throw(! (perc->num = num));
		fmll_throw_null((w = perc->w = (double **) fmll_alloc_2D(num, max_N, sizeof(double))));
		fmll_throw_null((perc->y = fmll_alloc_1D(max_N, sizeof(double))));
		fmll_throw_null((perc->t_y = fmll_alloc_1D(max_N, sizeof(double))));
		
		memcpy(t_fun, fun, size);
		memcpy(t_d_fun, d_fun, size);
		memcpy(t_N, N, layers_num * sizeof(uint16_t));

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q < prev_num; q++)
					w[t][q] = (* weight_init)();
		}

		perc->dim = dim;
		perc->layers_num = layers_num;

	fmll_catch;

		fmll_perceptron_destroy(perc);
		perc = NULL;

	fmll_finally;
	
	return perc;
}

void fmll_perceptron_destroy(fmll_perceptron * perc)
{
	if(perc != NULL)
	{
		fmll_free_ND(perc->w);
		fmll_free_ND(perc->y);
		fmll_free_ND(perc->N);
		fmll_free_ND(perc->fun);
		fmll_free_ND(perc->d_fun);
		fmll_free_ND(perc);
	}
}

double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec)
{
	uint8_t layers_num = perc->layers_num, dim = perc->dim;
	uint16_t prev_num, N_u, * N = perc->N;
	uint32_t u, v, q, t;
	double net, * y = perc->y, * t_y = perc->t_y, ** w = perc->w;
	double (** fun)(double) = perc->fun;

	memcpy(y, vec, dim * sizeof(double));

	for(u = t = 0, N_u = dim; u < layers_num; u++)
	{
		prev_num = N_u;
		N_u = N[u];

		for(v = 0; v < N_u; v++, t++)
		{
			for(q = 0, net = 0; q < prev_num; q++)
				net += w[t][q] * y[q];

			t_y[v] = (* fun[u])(net);
		}

		memcpy(y, t_y, N_u * sizeof(double));
	}

	return y;
}

int8_t fmll_perceptron_teach_gradient(fmll_perceptron * perc, double ** vec, uint32_t vec_num, double beta_0, double (* next_beta)(double))
{
	fmll_try;

		int8_t ret = 0;

		fmll_throw(beta_0 < 0 || beta_0 > 1);

		// TODO

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

