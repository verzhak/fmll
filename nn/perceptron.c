
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
		uint32_t u, v, t, q, num, max_threads = omp_get_max_threads(), size = layers_num * sizeof(double (*)(double));

		fmll_throw_null((perc = fmll_alloc_1D(1, sizeof(fmll_perceptron))));

		perc->fun = perc->d_fun = NULL;
		perc->N = NULL;
		perc->y = NULL;
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
		fmll_throw_null((w = perc->w = (double **) fmll_alloc_2D(num, max_N + 1, sizeof(double))));
		fmll_throw_null((perc->y = (double ***) fmll_alloc_3D(max_threads, layers_num + 1, max_N, sizeof(double))));
		fmll_throw_null((perc->net = (double **) fmll_alloc_2D(max_threads, num, sizeof(double))));
		
		memcpy(t_fun, fun, size);
		memcpy(t_d_fun, d_fun, size);
		memcpy(t_N, N, layers_num * sizeof(uint16_t));

		for(u = t = 0, N_u = dim; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++)
					w[t][q] = (* weight_init)();
		}

		perc->dim = dim;
		perc->layers_num = layers_num;
		perc->max_N = max_N;

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
		fmll_free_ND(perc->net);
		fmll_free_ND(perc);
	}
}

double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec)
{
	uint8_t layers_num = perc->layers_num, dim = perc->dim;
	uint16_t prev_num, N_u, * N = perc->N;
	uint32_t u, v, q, t, thread_num = omp_get_thread_num();
	double t_net, * c_y, ** y = perc->y[thread_num], * net = perc->net[thread_num], ** w = perc->w;
	double * n_y = y[0];
	double (** fun)(double) = perc->fun;

	memcpy(n_y, vec, dim * sizeof(double));

	for(u = t = 0, N_u = dim; u < layers_num; u++)
	{
		prev_num = N_u;
		N_u = N[u];
		c_y = n_y;
		n_y = y[u + 1];

		for(v = 0; v < N_u; v++, t++)
		{
			for(q = 0, t_net = 0; q < prev_num; q++)
				t_net += w[t][q] * c_y[q];

			t_net -= w[t][prev_num];
			net[t] = t_net;
			n_y[v] = (* fun[u])(t_net);
		}
	}

	return n_y;
}

int8_t fmll_perceptron_teach_gradient_batch(fmll_perceptron * perc, double ** vec, double ** d, uint32_t vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, uint32_t max_iter, double E_thres, double d_E_thres)
{
	fmll_try;

		int8_t ret = 0;
		uint8_t dim = perc->dim, layers_num = perc->layers_num;
		uint16_t c_N, p_N, max_N = perc->max_N, * N = perc->N;
		int32_t i, j, k, t;
		uint32_t u, iter, thread_num, max_threads = omp_get_max_threads(), num = perc->num;
		double r_E, prev_E, beta, t_sum, delta, t_d_w, E[max_threads], ** net = perc->net, *** y = perc->y;
		double ** sum = NULL, ** prev_sum = NULL, ** w = perc->w, ** d_w_moment = NULL, *** d_w = NULL;
		double (** d_fun)(double) = perc->d_fun;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((sum = (double **) fmll_alloc_2D(max_threads, max_N, sizeof(double))));
		fmll_throw_null((prev_sum = (double **) fmll_alloc_2D(max_threads, max_N, sizeof(double))));
		fmll_throw_null((d_w = (double ***) fmll_alloc_3D(max_threads, num, max_N + 1, sizeof(double))));
		fmll_throw_null((d_w_moment = (double **) fmll_alloc_2D(num, max_N + 1, sizeof(double))));

		for(t = 0, p_N = max_N + 1; t < num; t++)
			for(k = 0; k < p_N; k++)
				d_w_moment[t][k] = 0;

		for(iter = 0, beta = (beta_0 > 0) ? beta_0 : (* next_beta)(0), r_E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && r_E > E_thres && (fabs(r_E - prev_E) > d_E_thres || iter < 100);
				iter++)
		{
			if((beta = (* next_beta)(beta)) > 1.00000001)
				beta = 1;

			prev_E = r_E;
			r_E = 0;

			#pragma omp parallel private(u, i, j, k, t, delta, c_N, p_N, thread_num, t_sum) default(shared)
			{
				thread_num = omp_get_thread_num();

				E[thread_num] = 0;

				for(t = 0, p_N = max_N + 1; t < num; t++)
					for(k = 0; k < p_N; k++)
						d_w[thread_num][t][k] = 0;

				for(u = thread_num; u < vec_num; u += max_threads)
				{
					fmll_perceptron_run(perc, vec[u]);

					for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
					{
						sum[thread_num][j] = t_sum = (d[u][j] - y[thread_num][layers_num][j]);
						E[thread_num] += t_sum * t_sum;
					}

					for(i = layers_num - 1, t = num - 1; i >= 0; i--)
					{
						c_N = p_N;
						p_N = i ? N[i - 1] : dim;

						memcpy(prev_sum[thread_num], sum[thread_num], c_N * sizeof(double));

						for(k = 0; k < p_N; k++)
							sum[thread_num][k] = 0;

						for(j = c_N - 1; j >= 0; j--, t--)
						{
							delta = prev_sum[thread_num][j] * ((* d_fun)(net[thread_num][t]));

							for(k = 0; k < p_N; k++)
							{
								d_w[thread_num][t][k] += delta * y[thread_num][i][k];
								sum[thread_num][k] += delta * w[t][k];
							}

							d_w[thread_num][t][p_N] += - delta;
						}
					}
				}

				#pragma omp critical
					r_E += E[thread_num];
			}

			for(t = 0, p_N = max_N + 1; t < num; t++)
				for(k = 0; k < p_N; k++)
				{
					for(u = 0, t_d_w = 0; u < max_threads; u++)
						t_d_w += d_w[u][t][k];

					d_w_moment[t][k] = coef_moment * d_w_moment[t][k] + beta * t_d_w / vec_num;
					w[t][k] += d_w_moment[t][k];
				}

			r_E /=  2 * vec_num * N[layers_num - 1];

			printf("Итерация = %u из %u (%lf %%), beta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					beta, r_E, fabs(r_E - prev_E));
		}

	fmll_catch;

		fmll_free_ND(sum);
		fmll_free_ND(prev_sum);
		fmll_free_ND(d_w);
		fmll_free_ND(d_w_moment);

		ret = -1;

	fmll_finally;

	return ret;
}

