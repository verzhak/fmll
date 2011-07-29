
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
		uint32_t u, v, t, q, num, t_num = omp_get_max_threads(), size = layers_num * sizeof(double (*)(double));

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
		fmll_throw_null((perc->y = (double ***) fmll_alloc_3D(t_num, layers_num + 1, max_N, sizeof(double))));
		fmll_throw_null((perc->net = (double **) fmll_alloc_2D(t_num, num, sizeof(double))));
		
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

int8_t fmll_perceptron_save(fmll_perceptron * perc, const char * fname_prefix)
{
	fmll_try;

		int8_t ret = 0;
		uint8_t layers_num = perc->layers_num, dim = perc->dim;
		uint16_t c_N, p_N, * N = perc->N;
		uint32_t u, i, j, k, t;
		char node_name[4096];
		double ** w = perc->w;
		mxml_node_t * node, * content_node, * main_node = NULL;
		
		fmll_throw((xml_create(TYPE_PERCEPTRON, & main_node, & content_node)));
		fmll_throw((xml_set_int(content_node, "dim", dim)));
		fmll_throw((xml_set_int(content_node, "layers_num", layers_num)));

		fmll_throw_null((node = mxmlNewElement(content_node, "N")));

		for(u = 0; u < layers_num; u++)
		{
			sprintf(node_name, "N_%u", u);
			fmll_throw((xml_set_int(node, node_name, N[u])));
		}

		fmll_throw_null((node = mxmlNewElement(content_node, "W")));

		for(i = 0, t = 0, c_N = dim; i < layers_num; i++)
		{
			p_N = c_N;
			c_N = N[i];

			for(j = 0; j < c_N; j++, t++)
				for(k = 0; k <= p_N; k++)
				{
					sprintf(node_name, "w_%u_%u", t, k);
					fmll_throw((xml_set_double(node, node_name, w[t][k])));
				}
		}

		fmll_throw(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_perceptron * fmll_perceptron_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double))
{
	fmll_try;

		fmll_perceptron * perc = NULL;
		mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;
		int dim, layers_num;
		uint16_t c_N, p_N, * N = NULL;
		uint32_t u, i, j, k, t;
		double ** w;

		fmll_throw((xml_load(fname_prefix, TYPE_PERCEPTRON, & main_node, & content_node)));

		fmll_throw((xml_get_int(content_node, "dim", & dim)));
		fmll_throw((xml_get_int(content_node, "layers_num", & layers_num)));
		
		fmll_throw_null((N = fmll_alloc_1D(layers_num, sizeof(uint16_t))));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "N", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < layers_num; u++)
		{
			fmll_throw_null((sub_node));
			N[u] = sub_node->child->value.integer;
			sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
		}

		fmll_throw_null((perc = fmll_perceptron_init(dim, layers_num, N, & fmll_weight_init_null, fun, d_fun)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST)));

		for(i = 0, t = 0, c_N = dim, w = perc->w, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); i < layers_num; i++)
		{
			p_N = c_N;
			c_N = N[i];

			for(j = 0; j < c_N; j++, t++)
				for(k = 0; k <= p_N; k++)
				{
					fmll_throw_null((sub_node));
					w[t][k] = sub_node->child->value.real;
					sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
				}
		}

	fmll_catch;

		fmll_perceptron_destroy(perc);
		perc = NULL;

	fmll_finally;

		fmll_free_ND(N);
		xml_destroy(main_node);

	return perc;
}

double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec)
{
	uint8_t layers_num = perc->layers_num, dim = perc->dim;
	uint16_t prev_num, N_u, * N = perc->N;
	uint32_t u, v, q, t, t_ind = omp_get_thread_num();
	double t_net, * c_y, ** y = perc->y[t_ind], * net = perc->net[t_ind], ** w = perc->w;
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
		uint32_t u, iter, t_ind, t_num = omp_get_max_threads(), num = perc->num;
		double E, prev_E, beta, delta, t_E[t_num], ** net = perc->net, *** y = perc->y;
		double ** sum = NULL, ** prev_sum = NULL, ** w = perc->w, ** d_w_moment = NULL, ** d_w = NULL, *** t_d_w = NULL;
		double (** d_fun)(double) = perc->d_fun;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(! max_iter);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((sum = (double **) fmll_alloc_2D(t_num, max_N, sizeof(double))));
		fmll_throw_null((prev_sum = (double **) fmll_alloc_2D(t_num, max_N, sizeof(double))));
		fmll_throw_null((d_w = (double **) fmll_alloc_2D(num, max_N + 1, sizeof(double))));
		fmll_throw_null((t_d_w = (double ***) fmll_alloc_3D(t_num, num, max_N + 1, sizeof(double))));
		fmll_throw_null((d_w_moment = (double **) fmll_alloc_2D(num, max_N + 1, sizeof(double))));

		for(t = 0, p_N = max_N + 1; t < num; t++)
			for(k = 0; k < p_N; k++)
				d_w_moment[t][k] = 0;

		for(iter = 0, beta = (beta_0 > 0) ? beta_0 : (* next_beta)(0), E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 100); iter++)
		{
			if((beta = (* next_beta)(beta)) > 1.00000001)
				beta = 1;

			prev_E = E;
			E = 0;

			for(t = 0, p_N = max_N + 1; t < num; t++)
				for(k = 0; k < p_N; k++)
					d_w[t][k] = 0;

			#pragma omp parallel private(u, i, j, k, t, delta, c_N, p_N, t_ind) default(shared)
			{
				t_ind = omp_get_thread_num();

				t_E[t_ind] = 0;

				for(t = 0, p_N = max_N + 1; t < num; t++)
					for(k = 0; k < p_N; k++)
						t_d_w[t_ind][t][k] = 0;

				for(u = t_ind; u < vec_num; u += t_num)
				{
					fmll_perceptron_run(perc, vec[u]);

					for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
					{
						sum[t_ind][j] = delta = (d[u][j] - y[t_ind][layers_num][j]);
						t_E[t_ind] += delta * delta;
					}

					for(i = layers_num - 1, t = num - 1; i >= 0; i--)
					{
						c_N = p_N;
						p_N = i ? N[i - 1] : dim;

						memcpy(prev_sum[t_ind], sum[t_ind], c_N * sizeof(double));

						for(k = 0; k < p_N; k++)
							sum[t_ind][k] = 0;

						for(j = c_N - 1; j >= 0; j--, t--)
						{
							delta = prev_sum[t_ind][j] * ((* d_fun[i])(net[t_ind][t]));

							for(k = 0; k < p_N; k++)
							{
								t_d_w[t_ind][t][k] += delta * y[t_ind][i][k];
								sum[t_ind][k] += delta * w[t][k];
							}

							t_d_w[t_ind][t][p_N] -= delta;
						}
					}
				}

				#pragma omp critical
				{
					E += t_E[t_ind];

					for(t = 0, p_N = max_N + 1; t < num; t++)
						for(k = 0; k < p_N; k++)
							d_w[t][k] += t_d_w[t_ind][t][k];
				}
			}

			for(t = 0, p_N = max_N + 1; t < num; t++)
				for(k = 0; k < p_N; k++)
					w[t][k] += (d_w_moment[t][k] = coef_moment * d_w_moment[t][k] + beta * d_w[t][k] / vec_num);

			E /=  2 * vec_num * N[layers_num - 1];

			printf("Итерация = %u из %u (%lf %%), beta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					beta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free_ND(sum);
		fmll_free_ND(prev_sum);
		fmll_free_ND(d_w);
		fmll_free_ND(t_d_w);
		fmll_free_ND(d_w_moment);

	return ret;
}

int8_t fmll_perceptron_teach_lm(fmll_perceptron * perc, double ** vec, double ** d, uint32_t vec_num,
		double eta_mult, double eta_coef, uint32_t max_iter, double E_thres, double d_E_thres)
{
	fmll_try;

		int8_t ret = 0;
		int signum;
		bool is_eta_not_init = true;
		uint8_t dim = perc->dim, layers_num = perc->layers_num;
		uint16_t c_N, p_N, max_N = perc->max_N, * N = perc->N;
		int32_t i, j, k, t_weight, t_w;
		uint32_t u, iter, num_weight, num = perc->num, t_ind, t_num = omp_get_max_threads();
		double eta, delta, E, prev_E, t_prev_E, * t_y, ** net = perc->net, *** y = perc->y;
		double * t_t_d_w, * t_sum, * t_prev_sum, * t_net, * r_y;
		double ** sum = NULL, ** prev_sum = NULL, ** t_d_w = NULL, * sum_d_w = NULL, * eigen_val = NULL, * d_w = NULL;
		double ** J = NULL, ** JJ = NULL, ** JJInv = NULL, ** w = perc->w;
		double (** d_fun)(double) = perc->d_fun;
		gsl_permutation * perm = NULL;
		gsl_matrix_view J_mat, JJ_mat, JJInv_mat;
		gsl_vector_view sum_d_w_vec, eigen_val_vec, d_w_vec;

		for(i = 1, num_weight = (dim + 1) * N[0]; i < layers_num; i++)
			num_weight += (N[i - 1] + 1) * N[i];

		fmll_throw(eta_mult < 1);
		fmll_throw(eta_coef <= 1);
		fmll_throw(! max_iter);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((sum = (double **) fmll_alloc_2D(t_num, max_N, sizeof(double))));
		fmll_throw_null((prev_sum = (double **) fmll_alloc_2D(t_num, max_N, sizeof(double))));
		fmll_throw_null((t_d_w = (double **) fmll_alloc_2D(t_num, num_weight, sizeof(double))));
		fmll_throw_null((sum_d_w = fmll_alloc_1D(num_weight, sizeof(double))));
		fmll_throw_null((d_w = fmll_alloc_1D(num_weight, sizeof(double))));
		fmll_throw_null((J = (double **) fmll_alloc_2D(vec_num, num_weight, sizeof(double))));
		fmll_throw_null((JJ = (double **) fmll_alloc_2D(num_weight, num_weight, sizeof(double))));
		fmll_throw_null((JJInv = (double **) fmll_alloc_2D(num_weight, num_weight, sizeof(double))));
		fmll_throw_null((eigen_val = fmll_alloc_1D(num_weight, sizeof(double))));
		fmll_throw_null((perm = gsl_permutation_alloc(num_weight)));

		sum_d_w_vec = gsl_vector_view_array(sum_d_w, num_weight); 
		d_w_vec = gsl_vector_view_array(d_w, num_weight); 
		J_mat = gsl_matrix_view_array((double *) (J + vec_num), vec_num, num_weight);
		JJ_mat = gsl_matrix_view_array((double *) (JJ + num_weight), num_weight, num_weight);
		JJInv_mat = gsl_matrix_view_array((double *) (JJInv + num_weight), num_weight, num_weight);
		eigen_val_vec = gsl_vector_view_array(eigen_val, num_weight); 

		for(iter = 0, E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 10); iter++)
		{
			prev_E = 0;

			for(t_weight = 0; t_weight < num_weight; t_weight++)
				sum_d_w[t_weight] = 0;

			#pragma omp parallel private(u, i, j, k, t_weight, t_w, delta, c_N, p_N, t_ind, t_prev_E, t_y, t_t_d_w, t_sum, t_prev_sum, t_net) default(shared)
			{
				t_ind = omp_get_thread_num();

				t_t_d_w = t_d_w[t_ind];
				t_sum = sum[t_ind];
				t_prev_sum = prev_sum[t_ind];
				t_net = net[t_ind];

				for(t_weight = 0; t_weight < num_weight; t_weight++)
					t_t_d_w[t_weight] = 0;
				
				for(u = t_ind, t_prev_E = 0; u < vec_num; u += t_num)
				{
					t_y = fmll_perceptron_run(perc, vec[u]);

					for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
					{
						t_sum[j] = delta = t_y[j] - d[u][j];
						t_prev_E += delta * delta;
					}

					for(i = layers_num - 1, t_weight = num_weight - 1, t_w = num - 1; i >= 0; i--)
					{
						c_N = p_N;
						p_N = i ? N[i - 1] : dim;
						t_y = y[t_ind][i];

						memcpy(t_prev_sum, t_sum, c_N * sizeof(double));

						for(k = 0; k < p_N; k++)
							t_sum[k] = 0;

						for(j = c_N - 1; j >= 0; j--, t_w--)
						{
							delta = t_prev_sum[j] * ((* d_fun[i])(t_net[t_w]));

							J[u][t_weight] = - delta;
							t_t_d_w[t_weight] -= delta;
							t_weight--;

							for(k = p_N - 1; k >= 0; k--, t_weight--)
							{
								J[u][t_weight] = delta * t_y[k];
								t_t_d_w[t_weight] += J[u][t_weight];
								t_sum[k] += delta * w[t_w][k];
							}
						}
					}
				}

				#pragma omp critical
				{
					prev_E += t_prev_E;

					for(t_weight = 0; t_weight < num_weight; t_weight++)
						sum_d_w[t_weight] += t_t_d_w[t_weight];
				}
			}

			// ############################################################################ 

			for(t_weight = 0; t_weight < num_weight; t_weight++)
				sum_d_w[t_weight] /= vec_num;

			for(t_weight = 0; t_weight < num_weight; t_weight++)
				d_w[t_weight] = 0;

			if(is_eta_not_init)
			{
				gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1, & J_mat.matrix, & J_mat.matrix, 0, & JJ_mat.matrix);

				gsl_eigen_symmv_workspace * eigen_workspace = gsl_eigen_symmv_alloc(num_weight);
				gsl_eigen_symmv(& JJ_mat.matrix, & eigen_val_vec.vector, & JJInv_mat.matrix, eigen_workspace);
				gsl_eigen_symmv_free(eigen_workspace);

				eta = eigen_val[0] * eta_mult;

				fmll_free_ND(eigen_val);
				eigen_val = NULL;

				is_eta_not_init = false;
			}
			else
				eta /= eta_coef;

			do
			{
				for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
				{
					p_N = c_N;
					c_N = N[i];

					for(j = 0; j < c_N; j++, t_w++)
						for(k = 0; k <= p_N; k++, t_weight++)
							w[t_w][k] -= d_w[t_weight];
				}

				for(i = 0; i < num_weight; i++)
				{
					for(j = 0; j < i; j++)
						JJ[i][j] = 0;

					JJ[i][i] = 1;

					for(j = i + 1; j < num_weight; j++)
						JJ[i][j] = 0;
				}

				gsl_permutation_init(perm);

				gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1, & J_mat.matrix, & J_mat.matrix, eta, & JJ_mat.matrix);
				gsl_linalg_LU_decomp(& JJ_mat.matrix, perm, & signum);
				gsl_linalg_LU_invert(& JJ_mat.matrix, perm, & JJInv_mat.matrix);
				gsl_blas_dgemv(CblasNoTrans, -1, & JJInv_mat.matrix, & sum_d_w_vec.vector, 0, & d_w_vec.vector);

				for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
				{
					p_N = c_N;
					c_N = N[i];

					for(j = 0; j < c_N; j++, t_w++)
						for(k = 0; k <= p_N; k++, t_weight++)
							w[t_w][k] += d_w[t_weight];
				}

				E = 0;

				#pragma omp parallel private(u, t_prev_E, r_y, j, p_N, t_ind) default(shared)
				{
					t_ind = omp_get_thread_num();
					r_y = y[t_ind][layers_num];

					for(u = t_ind, t_prev_E = 0; u < vec_num; u += t_num)
					{
						fmll_perceptron_run(perc, vec[u]);

						for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
						{
							delta = d[u][j] - r_y[j];
							t_prev_E += delta * delta;
						}
					}

					#pragma omp critical
						E += t_prev_E;
				}

				if(E >= prev_E)
					eta *= eta_coef;
			}
			while(E >= prev_E);

			// ############################################################################ 

			E /=  2 * vec_num * N[layers_num - 1];
			prev_E /=  2 * vec_num * N[layers_num - 1];

			printf("Итерация = %u из %u (%lf %%), eta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					eta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free_ND(sum);
		fmll_free_ND(prev_sum);
		fmll_free_ND(d_w);
		fmll_free_ND(t_d_w);
		fmll_free_ND(sum_d_w);
		fmll_free_ND(J);
		fmll_free_ND(JJ);
		fmll_free_ND(JJInv);
		fmll_free_ND(eigen_val);

		if(perm != NULL)
			gsl_permutation_free(perm);

	return ret;
}

