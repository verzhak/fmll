
#include "ann/perceptron/perceptron.h"

/* ############################################################################ */
/* Вспомогательные функции */

/* Расчет градиента и матрицы Якоби методом обратного распространения ошибки */
double grad_Jacobian(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num, double * grad, double ** J)
{
	int i, j, k, t_weight, t_w;
	unsigned c_N, p_N, max_N = perc->max_N, * N = perc->N, dim = perc->dim, layers_num = perc->layers_num;
	unsigned u, num_weight = perc->num_weight, num = perc->num, t_ind, t_num = omp_get_max_threads();
	double delta, E, t_E, ** net = perc->net, *** y = perc->y;
	const double * t_y;
	double * t_t_grad, * t_sum, * t_prev_sum, * t_net;
	double ** w = perc->w, ** sum = NULL, ** prev_sum = NULL, ** t_grad = NULL;
	double (** d_fun)(double) = perc->d_fun;

	fmll_try;

		fmll_throw_null((sum = (double **) fmll_alloc(sizeof(double), 2, t_num, max_N)));
		fmll_throw_null((prev_sum = (double **) fmll_alloc(sizeof(double), 2, t_num, max_N)));
		fmll_throw_null((t_grad = (double **) fmll_alloc(sizeof(double), 2, t_num, num_weight)));

		E = 0;

		for(t_weight = 0; t_weight < num_weight; t_weight++)
			grad[t_weight] = 0;

		#pragma omp parallel private(u, i, j, k, t_weight, t_w, delta, c_N, p_N, t_ind, t_E, t_y, t_t_grad, t_sum, t_prev_sum, t_net) default(shared)
		{
			t_ind = omp_get_thread_num();

			t_t_grad = t_grad[t_ind];
			t_sum = sum[t_ind];
			t_prev_sum = prev_sum[t_ind];
			t_net = net[t_ind];

			for(t_weight = 0; t_weight < num_weight; t_weight++)
				t_t_grad[t_weight] = 0;

			/* Для каждого вектора из обучающего множества векторов повторять */
			for(u = t_ind, t_E = 0; u < vec_num; u += t_num)
			{
				/* Прогнать перцептрон над вектором */
				t_y = fmll_perceptron_run(perc, vec[u]);

				/*
				 * Вычислить d f для всех f таких, что E = sum(f)
				 *
				 * Таким образом, fi соответствует i-му обучающему вектору,
				 * d fi - отклонение выхода перцептрона для i-го обучающего вектора
				 * от действительного значения аппроксимируемой функции для данного обучающего вектора
				 * */
				for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
				{
					t_sum[j] = delta = d[u][j] - t_y[j];
					t_E += delta * delta;
				}

				/* Для каждого скрытого слоя перцептрона, начиная с последнего, повторять */
				for(i = layers_num - 1, t_weight = num_weight - 1, t_w = num - 1; i >= 0; i--)
				{
					c_N = p_N;
					p_N = i ? N[i - 1] : dim;
					t_y = y[t_ind][i];

					memcpy(t_prev_sum, t_sum, c_N * sizeof(double));

					for(k = 0; k < p_N; k++)
						t_sum[k] = 0;

					/* Для каждого нейрона обрабатываемого скрытого слоя повторять */
					for(j = c_N - 1; j >= 0; j--, t_w--)
					{
						/* Вычислить delta для обрабатываемого нейрона */
						delta = t_prev_sum[j] * ((* d_fun[i])(t_net[t_w]));

						/* Рассчитать очередной элемент матрицы Якоби, если матрицу Якоби требуется вычислить */
						if(J != NULL)
							J[u][t_weight] = - delta;

						/* Учесть вклад очередного обучающего вектора в градиент порога обрабатываемого нейрона */
						t_t_grad[t_weight] -= delta;
						t_weight--;

						/*
						 * Учесть вклад очередного обучающего вектора в градиенты весов обрабатываемого нейрона
						 *
						 * Суммы произведений delta на веса нейронов обрабатываемого слоя учитывается при распространении
						 * ошибки на предыдущий слой перцептрона
						 * */
						for(k = p_N - 1; k >= 0; k--, t_weight--)
						{
							/* Рассчитать очередной элемент матрицы Якоби, если матрицу Якоби требуется вычислить */
							if(J != NULL)
							{
								J[u][t_weight] = delta * t_y[k];
								t_t_grad[t_weight] += J[u][t_weight];
							}
							else
								t_t_grad[t_weight] += delta * t_y[k];

							t_sum[k] += delta * w[t_w][k];
						}
					}
				}
			}

			#pragma omp critical
			{
				E += t_E;
				
				/* Суммировать слагаемые градиентов весов нейронов перцептрона, вычисленные различными потоками */
				for(t_weight = 0; t_weight < num_weight; t_weight++)
					grad[t_weight] += t_t_grad[t_weight];
			}
		}

		for(t_weight = 0; t_weight < num_weight; t_weight++)
			grad[t_weight] /= vec_num;

	fmll_catch;

		E = -1;

	fmll_finally;

		fmll_free(sum);
		fmll_free(prev_sum);
		fmll_free(t_grad);

	return E;
}

/* ############################################################################ */

fmll_perceptron * fmll_perceptron_init(unsigned dim, unsigned layers_num, const unsigned * N,
		double (* weight_init)(), double (** fun)(double), double (** d_fun)(double))
{
	fmll_perceptron * perc = NULL;
	double (** t_fun)(double);
	double (** t_d_fun)(double);
	double ** w;
	unsigned u, v, t, q, num_weight, num, max_N, N_u, prev_num, * t_N, t_num = omp_get_max_threads(), size = layers_num * sizeof(double (*)(double));

	fmll_try;

		fmll_throw_null((perc = fmll_alloc(sizeof(fmll_perceptron), 1, 1)));

		perc->fun = perc->d_fun = NULL;
		perc->N = NULL;
		perc->y = NULL;
		perc->w = NULL;

		fmll_throw_null((t_fun = perc->fun = fmll_alloc(sizeof(double (*)(double)), 1, layers_num)));
		fmll_throw_null((t_d_fun = perc->d_fun = fmll_alloc(sizeof(double (*)(double)), 1, layers_num)));
		fmll_throw_null((t_N = perc->N = fmll_alloc(sizeof(unsigned), 1, layers_num)));

		max_N = N[0] > dim ? N[0] : dim;

		for(u = 1, num = N[0]; u < layers_num; u++)
		{
			num += N[u];

			if(max_N < N[u])
				max_N = N[u];
		}

		fmll_throw(! (perc->num = num));
		fmll_throw_null((w = perc->w = (double **) fmll_alloc(sizeof(double), 2, num, max_N + 1)));
		fmll_throw_null((perc->y = (double ***) fmll_alloc(sizeof(double), 3, t_num, layers_num + 1, max_N)));
		fmll_throw_null((perc->net = (double **) fmll_alloc(sizeof(double), 2, t_num, num)));
		
		memcpy(t_fun, fun, size);
		memcpy(t_d_fun, d_fun, size);
		memcpy(t_N, N, layers_num * sizeof(unsigned));

		for(u = t = 0, N_u = dim, num_weight = 0; u < layers_num; u++)
		{
			prev_num = N_u;
			N_u = N[u];

			for(v = 0; v < N_u; v++, t++)
				for(q = 0; q <= prev_num; q++, num_weight++)
					w[t][q] = (* weight_init)();
		}

		perc->dim = dim;
		perc->layers_num = layers_num;
		perc->max_N = max_N;
		perc->num_weight = num_weight;

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
		fmll_free(perc->w);
		fmll_free(perc->y);
		fmll_free(perc->N);
		fmll_free(perc->fun);
		fmll_free(perc->d_fun);
		fmll_free(perc->net);
		fmll_free(perc);
	}
}

int fmll_perceptron_save(fmll_perceptron * perc, const char * fname_prefix)
{
	int ret = 0;
	char node_name[4096];
	unsigned u, i, j, k, t, c_N, p_N, * N = perc->N, layers_num = perc->layers_num, dim = perc->dim;
	double ** w = perc->w;
	mxml_node_t * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;
		
	fmll_try;

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

			sprintf(node_name, "layer_%u", i);
			fmll_throw_null((sub_node = mxmlNewElement(node, node_name)));

			for(j = 0; j < c_N; j++, t++)
			{
				sprintf(node_name, "w_%u", j);
				fmll_throw_null((sub_sub_node = mxmlNewElement(sub_node, node_name)));

				for(k = 0; k <= p_N; k++)
				{
					sprintf(node_name, "%u", k);
					fmll_throw((xml_set_double(sub_sub_node, node_name, w[t][k])));
				}
			}
		}

		fmll_throw((xml_save(fname_prefix, main_node)));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_perceptron * fmll_perceptron_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double))
{
	int dim, layers_num;
	char node_name[4096];
	unsigned u, i, j, k, t, c_N, p_N, * N = NULL;
	double ** w;
	fmll_perceptron * perc = NULL;
	mxml_node_t * sub_sub_sub_node, * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw((xml_load(fname_prefix, TYPE_PERCEPTRON, & main_node, & content_node)));

		fmll_throw((xml_get_int(content_node, "dim", & dim)));
		fmll_throw((xml_get_int(content_node, "layers_num", & layers_num)));
		
		fmll_throw_null((N = fmll_alloc(sizeof(unsigned), 1, layers_num)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "N", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < layers_num; u++)
		{
			fmll_throw_null((sub_node));
			N[u] = sub_node->child->value.integer;
			sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
		}

		fmll_throw_null((perc = fmll_perceptron_init(dim, layers_num, N, & fmll_weight_init_null, fun, d_fun)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST)));

		for(i = 0, t = 0, c_N = dim, w = perc->w; i < layers_num; i++)
		{
			p_N = c_N;
			c_N = N[i];

			sprintf(node_name, "layer_%u", i);
			fmll_throw_null((sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST)));

			for(j = 0; j < c_N; j++, t++)
			{
				sprintf(node_name, "w_%u", j);
				fmll_throw_null((sub_sub_node = mxmlFindElement(sub_node, sub_node, node_name, NULL, NULL, MXML_DESCEND_FIRST)));

				for(k = 0, sub_sub_sub_node = mxmlFindElement(sub_sub_node, sub_sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); k <= p_N; k++)
				{
					fmll_throw_null((sub_sub_sub_node));
					w[t][k] = sub_sub_sub_node->child->value.real;
					sub_sub_sub_node = mxmlFindElement(sub_sub_sub_node, sub_sub_node, NULL, NULL, NULL, MXML_DESCEND);
				}
			}
		}

	fmll_catch;

		fmll_perceptron_destroy(perc);
		perc = NULL;

	fmll_finally;

		fmll_free(N);
		xml_destroy(main_node);

	return perc;
}

const double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec)
{
	unsigned u, v, q, t, prev_num, N_u, dim = perc->dim, layers_num = perc->layers_num, * N = perc->N, t_ind = omp_get_thread_num();
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

unsigned fmll_perceptron_test(fmll_perceptron * perc, double ** vec, double ** d, double ** deviation, unsigned vec_num,
		void (* st_func)(fmll_perceptron *, double *, double *, const double *, unsigned, bool, void *), void * st_param)
{
	bool is_right;
	unsigned u, v, no = 0, last_N = perc->N[perc->layers_num - 1];
	const double * y;
	double norm;

	for(u = 0; u < vec_num; u++)
	{
		y = fmll_perceptron_run(perc, vec[u]);

		for(v = 0, is_right = true; v < last_N && is_right; v++)
		{
			norm = y[v] - d[u][v];

			if(norm < - deviation[v][0] || norm > deviation[v][1])
			{
				no++;
				is_right = false;
			}
		}

		if(st_func != NULL)
			(* st_func)(perc, vec[u], d[u], y, vec_num, is_right, st_param);
	}

	return vec_num - no;
}

int fmll_perceptron_teach_gradient_batch(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, unsigned max_iter, double E_thres, double d_E_thres)
{
	int i, j, k, ret = 0;
	unsigned iter, t_weight, t_w, c_N, p_N, dim = perc->dim, layers_num = perc->layers_num, num_weight = perc->num_weight, * N = perc->N;
	double E, prev_E, beta, norm_E = 2 * vec_num * N[layers_num - 1], ** w = perc->w, * moment = NULL, * grad = NULL;

	fmll_try;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(! max_iter);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((grad = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((moment = fmll_alloc(sizeof(double), 1, num_weight)));

		for(t_weight = 0; t_weight < num_weight; t_weight++)
				moment[t_weight] = 0;

		/* Главный цикл алгоритма */
		for(iter = 0, beta = (beta_0 > 0) ? beta_0 : (* next_beta)(0), E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 100); iter++)
		{
			if((beta = (* next_beta)(beta)) > 1.00000001)
				beta = 1;

			/*
			 * ############################################################################ 
			 *
			 * Расчет градиента и матрицы Якоби методом обратного распространения ошибки
			 */

			prev_E = E;
			E = grad_Jacobian(perc, vec, d, vec_num, grad, NULL) / norm_E;

			/* Прибавить к весам нейронов уточняющие коэффициенты с учитыванием инерции (уточняющих весов на предыдущей итерации алгоритма обучения) */
			for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
			{
				p_N = c_N;
				c_N = N[i];

				for(j = 0; j < c_N; j++, t_w++)
					for(k = 0; k <= p_N; k++, t_weight++)
						w[t_w][k] += (moment[t_weight] = coef_moment * moment[t_weight] + beta * grad[t_weight]);
			}

			fmll_print("Iteration = %u from %u (%lf %%), beta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					beta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(grad);
		fmll_free(moment);

	return ret;
}

int fmll_perceptron_teach_Levenberg_Marquardt(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		double eta_mult, double eta_coef, unsigned max_iter, double E_thres, double d_E_thres)
{
	int i, j, k, t_weight, t_w, ret = 0;
	unsigned c_N, p_N, u, iter, t_ind, dim = perc->dim, layers_num = perc->layers_num, num_weight = perc->num_weight, * N = perc->N;
	unsigned t_num = omp_get_max_threads();
	double eta, delta, E, prev_E, t_E, norm_E = 2 * vec_num * N[layers_num - 1], * t_y, *** y = perc->y;
	double * grad = NULL, * eigen_real = NULL, * eigen_complex = NULL, * d_w = NULL;
	double ** J = NULL, ** JT = NULL, ** I = NULL, ** JJ = NULL, ** JJInv = NULL, ** w = perc->w;

	fmll_try;

		fmll_throw(eta_mult < 1);
		fmll_throw(eta_coef <= 1);
		fmll_throw(! max_iter);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((grad = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((d_w = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((J = (double **) fmll_alloc(sizeof(double), 2, vec_num, num_weight)));
		fmll_throw_null((JT = (double **) fmll_alloc(sizeof(double), 2, num_weight, vec_num)));
		fmll_throw_null((I = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight)));
		fmll_throw_null((JJ = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight)));
		fmll_throw_null((JJInv = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight)));
		fmll_throw_null((eigen_real = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((eigen_complex = fmll_alloc(sizeof(double), 1, num_weight)));

		for(iter = 0, E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 10); iter++)
		{
			/*
			 * ############################################################################ 
			 *
			 * Расчет градиента и матрицы Якоби методом обратного распространения ошибки
			 */

			prev_E = grad_Jacobian(perc, vec, d, vec_num, grad, J);

			/*
			 * ############################################################################
			 *
			 * Оценка матрицы Гессе, расчет уточняющих коэффициентов и корректировка весов нейронов перцептрона
			 */

			for(t_weight = 0; t_weight < num_weight; t_weight++)
				d_w[t_weight] = 0;

			if(iter)
				/*
				 * На последующих итерациях алгоритма обучения начальное значение коэффициента eta
				 * устанавливается в eta / eta_coef, где eta_coef > 1
				 */
				eta /= eta_coef;
			else
			{
				/*
				 * Начальное значение коэффициента eta на первой итерации алгоритма обучения
				 * должно быть много больше наибольшего собственного значения матрицы Якоби
				 */

				fmll_throw(fmll_math_matrix_transpose(J, JT, vec_num, num_weight));
				fmll_throw(fmll_math_matrix_mult(JT, J, JJ, num_weight, vec_num, num_weight));
				fmll_throw(fmll_math_matrix_eigen(JJ, eigen_real, eigen_complex, num_weight, 1E-15));

				eta = 100 * (eigen_real[0] * eigen_real[0] + eigen_complex[0] * eigen_complex[0]) * eta_mult;

				fmll_free(eigen_real);
				fmll_free(eigen_complex);

				eigen_real = NULL;
				eigen_complex = NULL;
			}

			/* До тех пор, пока ошибка не уменьшится, повторять */
			do
			{
				/* Убрать предыдущую корректировку весов нейронов перцептрона */
				for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
				{
					p_N = c_N;
					c_N = N[i];

					for(j = 0; j < c_N; j++, t_w++)
						for(k = 0; k <= p_N; k++, t_weight++)
							w[t_w][k] -= d_w[t_weight];
				}

				/* Создание единичной матрицы */
				for(i = 0; i < num_weight; i++)
				{
					for(j = 0; j < i; j++)
						JJ[i][j] = 0;

					JJ[i][i] = 1;

					for(j = i + 1; j < num_weight; j++)
						JJ[i][j] = 0;
				}

				/* Вычисление аппроксимации матрицы Гессе: JJ = J' * J + eta * E */
				fmll_throw(fmll_math_matrix_transpose(J, JT, vec_num, num_weight));
				fmll_throw(fmll_math_matrix_init_main_diag(I, eta, num_weight, num_weight));
				fmll_throw(fmll_math_matrix_mult(JT, J, JJ, num_weight, vec_num, num_weight));
				fmll_throw(fmll_math_matrix_sum(1, I, 1, JJ, JJ, num_weight, num_weight));

				/* Вычисление обратной матрицы для матрицы JJ */
				fmll_throw(fmll_math_matrix_inv(JJ, JJInv, num_weight));

				/*
				 * Вычисление корректировки весов нейронов перцептрона: d_w = - JJInv * grad
				 * (минус учитывается далее при корректировке весов нейронов перцептрона)
				 */
				fmll_throw(fmll_math_matrix_mult_vector(JJInv, grad, d_w, num_weight, num_weight));

				/* Корректировка весов нейронов перцептрона */
				for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
				{
					p_N = c_N;
					c_N = N[i];

					for(j = 0; j < c_N; j++, t_w++)
						for(k = 0; k <= p_N; k++, t_weight++)
							w[t_w][k] += d_w[t_weight];
				}

				E = 0;

				/* Оценка ошибки */
				#pragma omp parallel private(u, t_E, t_y, j, p_N, t_ind) default(shared)
				{
					t_ind = omp_get_thread_num();
					t_y = y[t_ind][layers_num];

					for(u = t_ind, t_E = 0; u < vec_num; u += t_num)
					{
						fmll_perceptron_run(perc, vec[u]);

						for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
						{
							delta = d[u][j] - t_y[j];
							t_E += delta * delta;
						}
					}

					#pragma omp critical
						E += t_E;
				}

				/* Если ошибка увеличилась, то коэффициент eta увеличивается в eta_coef раз */
				if(E >= prev_E)
					eta *= eta_coef;
			}
			while(E >= prev_E);

			/* ############################################################################ */

			E /= norm_E;
			prev_E /= norm_E;

			fmll_print("Iteration = %u from %u (%lf %%), eta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					eta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(d_w);
		fmll_free(grad);
		fmll_free(J);
		fmll_free(JT);
		fmll_free(I);
		fmll_free(JJ);
		fmll_free(JJInv);
		fmll_free(eigen_real);
		fmll_free(eigen_complex);

	return ret;
}

int fmll_perceptron_teach_conjugate_gradient(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		unsigned max_iter, double coef_E, double E_thres, double d_E_thres)
{
	bool first_run;
	int i, j, k, t_weight, t_w, ret = 0;
	unsigned c_N, p_N, u, v, iter, t_ind, num_weight = perc->num_weight, dim = perc->dim, layers_num = perc->layers_num;
	unsigned * N = perc->N, t_num = omp_get_max_threads();
	double delta, E, E_coef_E, prev_E, t_E, beta_1, beta_2, beta, eta_1, eta[3], tE[3], norm_E = 2 * vec_num * N[layers_num - 1];
	double * t_y, *** y = perc->y, * s = NULL, * prev_s = NULL,* grad = NULL, * prev_grad = NULL, * d_w = NULL, ** w = perc->w;

	fmll_try;

		fmll_throw(coef_E <= 0);
		fmll_throw(coef_E >= 1);
		fmll_throw(E_thres < 0);
		fmll_throw(d_E_thres < 0);
		fmll_throw_null((grad = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((prev_grad = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((s = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((prev_s = fmll_alloc(sizeof(double), 1, num_weight)));
		fmll_throw_null((d_w = fmll_alloc(sizeof(double), 1, num_weight)));

		for(iter = 0, E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 10); iter++)
		{
			if(iter)
			{
				memcpy(prev_grad, grad, num_weight * sizeof(double));
				memcpy(prev_s, s, num_weight * sizeof(double));
			}

			/*
			 * ############################################################################ 
			 *
			 * Расчет градиента (резидуальной ошибки) методом обратного распространения ошибки
			 */

			prev_E = E;
			E_coef_E = coef_E * grad_Jacobian(perc, vec, d, vec_num, grad, NULL);

			/* Расчет очередного вектора базиса */
			if(iter)
			{
				/* 
				 * Для не первой итерации существует предыдущий вектор базиса =>
				 * необходимо рассчитать коэффициент beta, с которым предыдущий вектор базиса будет учтен
				 * в рассчитываемом векторе
				 *
				 * Коэффициент beta рассчитывается по методу Полака - Рибьера
				 */
				for(t_weight = 0, beta_1 = 0, beta_2 = 0; t_weight < num_weight; t_weight++)
				{
					beta_1 += grad[t_weight] * (grad[t_weight] - prev_grad[t_weight]);
					beta_2 += prev_grad[t_weight] * prev_grad[t_weight];
				}

				if(
					(beta = beta_1 / beta_2) < 0
				  )
					beta = 0;

				for(t_weight = 0, beta_1 = 0, beta_2 = 0; t_weight < num_weight; t_weight++)
				{
					s[t_weight] = grad[t_weight] + beta * prev_s[t_weight];
					d_w[t_weight] = 0;
				}
			}
			else
				/* Для первой итерации вектор базиса равен обратному градиенту (резидуальной ошибке) */
				for(t_weight = 0, beta_1 = 0, beta_2 = 0; t_weight < num_weight; t_weight++)
				{
					s[t_weight] = grad[t_weight];
					d_w[t_weight] = 0;
				}

			first_run = true;

			do
			{
				/* Поиск оптимального коэффициента eta - линейный поиск по параболе минимизируемой квадратичной формы */
				if(first_run)
				{
					/* Первая итерация алгоритма - инициализация массива eta[] */
					eta[0] = 0; /* \\TODO drand48() - 0.5 */
					eta[2] = 0; /* \\TODO drand48() + 0.5 + 0.0000000001 */
					eta[1] = (eta[0] + eta[2]) / 2;

					first_run = false;
				}
				else
				{
					/* 
					 * Не первая итерация алгоритма - пересчет eta[] в соответствии с расположением минимума
					 * относительно предыдущих значений eta[]
					 */
					if(tE[0] > tE[1] && tE[2] > tE[1])
					{
						/* Внутри */

						eta[0] = (eta[0] + eta[1]) / 2;
						eta[2] = (eta[2] + eta[1]) / 2;
					}
					else if(tE[1] > tE[2])
					{
						/* Справа от eta[2] */

						eta[0] = eta[1];
						eta[1] = eta[2];
						eta[2] += 1;
					}
					else
					{
						/* Слева от eta[0] */

						eta[2] = eta[1];
						eta[1] = eta[0];
						eta[0] -= 1;
					}
				}

				/* Расчет ошибки для каждой из eta[] */
				for(u = 0; u < 3; u++)
				{
					for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
					{
						p_N = c_N;
						c_N = N[i];

						for(j = 0; j < c_N; j++, t_w++)
							for(k = 0; k <= p_N; k++, t_weight++)
							{
								w[t_w][k] -= d_w[t_weight];
								d_w[t_weight] = eta[u] * s[t_weight];
								w[t_w][k] += d_w[t_weight];
							}
					}

					tE[u] = 0;

					#pragma omp parallel private(v, t_E, t_y, j, p_N, t_ind) default(shared)
					{
						t_ind = omp_get_thread_num();
						t_y = y[t_ind][layers_num];

						for(v = t_ind, t_E = 0; v < vec_num; v += t_num)
						{
							fmll_perceptron_run(perc, vec[v]);

							for(j = 0, p_N = N[layers_num - 1]; j < p_N; j++)
							{
								delta = d[v][j] - t_y[j];
								t_E += delta * delta;
							}
						}

						#pragma omp critical
							tE[u] += t_E;
					}
				}
			}
			while(fabs(tE[0] - tE[2]) > E_coef_E);

			/* Окончательная коррекция весов нейронной сети */
			for(i = 0, c_N = dim, t_weight = 0, t_w = 0, eta_1 = eta[1]; i < layers_num; i++)
			{
				p_N = c_N;
				c_N = N[i];

				for(j = 0; j < c_N; j++, t_w++)
					for(k = 0; k <= p_N; k++, t_weight++)
						w[t_w][k] = w[t_w][k] - d_w[t_weight] + eta_1 * s[t_weight];
			}

			/* ############################################################################ */

			E = tE[1] / norm_E;

			fmll_print("Iteration = %u from %u (%lf %%), beta = %lf, eta = %lf, E = %.7lf, E' = %.7lf\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter,
					iter ? beta : 0, eta_1, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(d_w);
		fmll_free(grad);
		fmll_free(prev_grad);
		fmll_free(s);
		fmll_free(prev_s);

	return ret;
}

