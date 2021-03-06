
#include "ann/mlp/mlp.h"

/* ############################################################################ */
/* Вспомогательные функции */

/* Расчет градиента и матрицы Якоби методом обратного распространения ошибки */
double grad_Jacobian(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num, double * grad, double ** J)
{
	const unsigned max_N = mlp->max_N, * N = mlp->N, dim = mlp->dim, layers_num = mlp->layers_num, num_weight = mlp->num_weight, num = mlp->num, t_num = omp_get_max_threads();
	const double * t_y, * t_net, ** net = (const double **) mlp->net, *** y = (const double ***) mlp->y, ** w = (const double **) mlp->w;
	int i, j, k, t_weight, t_w;
	unsigned c_N, p_N, u, t_ind;
	double delta, E, t_E, * t_t_grad, * t_sum, * t_prev_sum, ** sum = NULL, ** prev_sum = NULL, ** t_grad = NULL;
	double (** d_fun)(double) = mlp->d_fun;

	fmll_try;

		fmll_throw_null(sum = (double **) fmll_alloc(sizeof(double), 2, t_num, max_N));
		fmll_throw_null(prev_sum = (double **) fmll_alloc(sizeof(double), 2, t_num, max_N));
		fmll_throw_null(t_grad = (double **) fmll_alloc(sizeof(double), 2, t_num, num_weight));

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
				t_y = fmll_mlp_run(mlp, vec[u]);

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

fmll_mlp * fmll_mlp_init(const unsigned dim, const unsigned layers_num, const unsigned * N, double (** fun)(const double), double (** d_fun)(const double))
{
	fmll_mlp * mlp = NULL;
	const unsigned size = layers_num * sizeof(double (*)(double)), t_num = omp_get_max_threads();
	double (** t_fun)(double);
	double (** t_d_fun)(double);
	double ** w;
	unsigned v, u, num_weight, num, max_N, max_N_1, * t_N;

	fmll_try;

		fmll_throw_null(mlp = fmll_alloc(sizeof(fmll_mlp), 1, 1));

		mlp->fun = mlp->d_fun = NULL;
		mlp->N = NULL;
		mlp->y = NULL;
		mlp->w = NULL;

		fmll_throw_null(t_fun = mlp->fun = fmll_alloc(sizeof(double (*)(double)), 1, layers_num));
		fmll_throw_null(t_d_fun = mlp->d_fun = fmll_alloc(sizeof(double (*)(double)), 1, layers_num));
		fmll_throw_null(t_N = mlp->N = fmll_alloc(sizeof(unsigned), 1, layers_num));

		max_N = N[0] > dim ? N[0] : dim;
		num_weight = (dim + 1) * N[0];

		for(u = 1, num = N[0]; u < layers_num; u++)
		{
			num += N[u];
			num_weight += (N[u - 1] + 1) * N[u];

			if(max_N < N[u])
				max_N = N[u];
		}
		
		max_N_1 = max_N + 1;

		fmll_throw_if(! (mlp->num = num));
		fmll_throw_null(w = mlp->w = (double **) fmll_alloc(sizeof(double), 2, num, max_N_1));
		fmll_throw_null(mlp->y = (double ***) fmll_alloc(sizeof(double), 3, t_num, layers_num + 1, max_N));
		fmll_throw_null(mlp->net = (double **) fmll_alloc(sizeof(double), 2, t_num, num));
		
		memcpy(t_fun, fun, size);
		memcpy(t_d_fun, d_fun, size);
		memcpy(t_N, N, layers_num * sizeof(unsigned));

		mlp->dim = dim;
		mlp->layers_num = layers_num;
		mlp->max_N = max_N;
		mlp->num_weight = num_weight;

		fmll_throw_if(fmll_math_matrix_init_fill(w, 0, num, max_N_1));

	fmll_catch;

		fmll_mlp_destroy(mlp);
		mlp = NULL;

	fmll_finally;
	
	return mlp;
}

void fmll_mlp_destroy(fmll_mlp * mlp)
{
	if(mlp != NULL)
	{
		fmll_free(mlp->w);
		fmll_free(mlp->y);
		fmll_free(mlp->N);
		fmll_free(mlp->fun);
		fmll_free(mlp->d_fun);
		fmll_free(mlp->net);
		fmll_free(mlp);
	}
}

int fmll_mlp_save(const fmll_mlp * mlp, const char * fname_prefix)
{
	int ret = 0;
	const unsigned * N = mlp->N, layers_num = mlp->layers_num, dim = mlp->dim;
	const double ** w = (const double **) mlp->w;
	char node_name[4096];
	unsigned u, i, j, k, t, c_N, p_N;
	mxml_node_t * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;
		
	fmll_try;

		fmll_throw_if(xml_create(TYPE_MLP, & main_node, & content_node));
		fmll_throw_if(xml_set_int(content_node, "dim", dim));
		fmll_throw_if(xml_set_int(content_node, "layers_num", layers_num));

		fmll_throw_null(node = mxmlNewElement(content_node, "N"));

		for(u = 0; u < layers_num; u++)
		{
			sprintf(node_name, "N_%u", u);
			fmll_throw_if(xml_set_int(node, node_name, N[u]));
		}

		fmll_throw_null(node = mxmlNewElement(content_node, "W"));

		for(i = 0, t = 0, c_N = dim; i < layers_num; i++)
		{
			p_N = c_N;
			c_N = N[i];

			sprintf(node_name, "layer_%u", i);
			fmll_throw_null(sub_node = mxmlNewElement(node, node_name));

			for(j = 0; j < c_N; j++, t++)
			{
				sprintf(node_name, "w_%u", j);
				fmll_throw_null(sub_sub_node = mxmlNewElement(sub_node, node_name));

				for(k = 0; k <= p_N; k++)
				{
					sprintf(node_name, "%u", k);
					fmll_throw_if(xml_set_double(sub_sub_node, node_name, w[t][k]));
				}
			}
		}

		fmll_throw_if(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_mlp * fmll_mlp_load(const char * fname_prefix, double (** fun)(const double), double (** d_fun)(const double))
{
	int dim, layers_num;
	char node_name[4096];
	unsigned u, i, j, k, t, c_N, p_N, * N = NULL;
	double ** w;
	fmll_mlp * mlp = NULL;
	mxml_node_t * sub_sub_sub_node, * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_if(xml_load(fname_prefix, TYPE_MLP, & main_node, & content_node));

		fmll_throw_if(xml_get_int(content_node, "dim", & dim));
		fmll_throw_if(xml_get_int(content_node, "layers_num", & layers_num));
		
		fmll_throw_null(N = fmll_alloc(sizeof(unsigned), 1, layers_num));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "N", NULL, NULL, MXML_DESCEND_FIRST));

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < layers_num; u++)
		{
			fmll_throw_null(sub_node);
			N[u] = sub_node->child->value.integer;
			sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
		}

		fmll_throw_null(mlp = fmll_mlp_init(dim, layers_num, N, fun, d_fun));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST));

		for(i = 0, t = 0, c_N = dim, w = mlp->w; i < layers_num; i++)
		{
			p_N = c_N;
			c_N = N[i];

			sprintf(node_name, "layer_%u", i);
			fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

			for(j = 0; j < c_N; j++, t++)
			{
				sprintf(node_name, "w_%u", j);
				fmll_throw_null(sub_sub_node = mxmlFindElement(sub_node, sub_node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

				for(k = 0, sub_sub_sub_node = mxmlFindElement(sub_sub_node, sub_sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); k <= p_N; k++)
				{
					fmll_throw_null(sub_sub_sub_node);
					w[t][k] = sub_sub_sub_node->child->value.real;
					sub_sub_sub_node = mxmlFindElement(sub_sub_sub_node, sub_sub_node, NULL, NULL, NULL, MXML_DESCEND);
				}
			}
		}

	fmll_catch;

		fmll_mlp_destroy(mlp);
		mlp = NULL;

	fmll_finally;

		fmll_free(N);
		xml_destroy(main_node);

	return mlp;
}

const double * fmll_mlp_run(fmll_mlp * mlp, const double * vec)
{
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, * N = mlp->N, t_ind = omp_get_thread_num();
	const double ** w = (const double **) mlp->w;
	unsigned u, v, q, t, prev_num, N_u;
	double t_net, * c_y, ** y = mlp->y[t_ind], * net = mlp->net[t_ind];
	double * n_y = y[0];
	double (** fun)(double) = mlp->fun;

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

unsigned fmll_mlp_test(fmll_mlp * mlp, const double ** vec, const double ** d, const double * deviation, const unsigned vec_num,
		void (* st_func)(const fmll_mlp *, const double *, const double *, const double *, const unsigned, const bool, void *), void * st_param)
{
	const unsigned last_N = mlp->N[mlp->layers_num - 1];
	const double * y;
	bool is_right;
	unsigned u, v, no = 0;

	for(u = 0; u < vec_num; u++)
	{
		y = fmll_mlp_run(mlp, vec[u]);

		for(v = 0, is_right = true; v < last_N && is_right; v++)
			if(fabs(y[v] - d[u][v]) > deviation[v])
			{
				no++;
				is_right = false;
			}

		if(st_func != NULL)
			(* st_func)(mlp, vec[u], d[u], y, vec_num, is_right, st_param);
	}

	return vec_num - no;
}

int fmll_mlp_teach_gradient_batch(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num,
		const double beta_0, double (* next_beta)(const double), const double coef_moment, const unsigned max_iter, const double E_thres, const double d_E_thres)
{
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, num_weight = mlp->num_weight, * N = mlp->N;
	const double norm_E = 2 * vec_num * N[layers_num - 1];
	int i, j, k, ret = 0;
	unsigned iter, t_weight, t_w, c_N, p_N;
	double E, prev_E, beta, ** w = mlp->w, * moment = NULL, * grad = NULL;

	fmll_try;

		fmll_throw_if(beta_0 < 0 || beta_0 > 1 || (! max_iter) || E_thres < 0 || d_E_thres < 0);
		fmll_throw_null(grad = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(moment = fmll_alloc(sizeof(double), 1, num_weight));

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
			E = grad_Jacobian(mlp, vec, d, vec_num, grad, NULL) / norm_E;

			/* Прибавить к весам нейронов уточняющие коэффициенты с учитыванием инерции (уточняющих весов на предыдущей итерации алгоритма обучения) */
			for(i = 0, c_N = dim, t_weight = 0, t_w = 0; i < layers_num; i++)
			{
				p_N = c_N;
				c_N = N[i];

				for(j = 0; j < c_N; j++, t_w++)
					for(k = 0; k <= p_N; k++, t_weight++)
						w[t_w][k] += (moment[t_weight] = coef_moment * moment[t_weight] + beta * grad[t_weight]);
			}

			fmll_print("Iteration = %u from %u (%.5f %%), beta = %.7f, E = %.7f, E' = %.7f\n", iter + 1, max_iter, (100.0 * (iter + 1)) / max_iter, beta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(grad);
		fmll_free(moment);

	return ret;
}

int fmll_mlp_teach_Levenberg_Marquardt(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num,
		const double eta_0, const double eta_coef, const unsigned max_iter, const double E_thres, const double d_E_thres)
{
	const unsigned dim = mlp->dim, layers_num = mlp->layers_num, num_weight = mlp->num_weight, * N = mlp->N, t_num = omp_get_max_threads();
	const double norm_E = 2 * vec_num * N[layers_num - 1];
	int i, j, k, t_weight, t_w, ret = 0;
	unsigned c_N, p_N, u, iter, t_ind;
	double eta, delta, E, prev_E, t_E, * t_y, *** y = mlp->y, * grad = NULL, * eigen_real = NULL, * eigen_complex = NULL, * d_w = NULL;
	double ** J = NULL, ** JT = NULL, ** I = NULL, ** JJ = NULL, ** JJInv = NULL, ** w = mlp->w;

	fmll_try;

		fmll_throw_if(eta_0 < 1 || eta_coef <= 1 || (! max_iter) || E_thres < 0 || d_E_thres < 0);
		fmll_throw_null(grad = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(d_w = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(J = (double **) fmll_alloc(sizeof(double), 2, vec_num, num_weight));
		fmll_throw_null(JT = (double **) fmll_alloc(sizeof(double), 2, num_weight, vec_num));
		fmll_throw_null(I = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight));
		fmll_throw_null(JJ = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight));
		fmll_throw_null(JJInv = (double **) fmll_alloc(sizeof(double), 2, num_weight, num_weight));
		fmll_throw_null(eigen_real = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(eigen_complex = fmll_alloc(sizeof(double), 1, num_weight));

		for(iter = 0, eta = 1, E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter < max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter < 10) && eta != 0; iter++)
		{
			/*
			 * ############################################################################ 
			 *
			 * Расчет градиента и матрицы Якоби методом обратного распространения ошибки
			 */

			prev_E = grad_Jacobian(mlp, vec, d, vec_num, grad, J);

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

				fmll_throw_if(fmll_math_matrix_transpose((const double **) J, JT, vec_num, num_weight));
				fmll_throw_if(fmll_math_matrix_mult((const double **) JT, (const double **) J, JJ, num_weight, vec_num, num_weight));
				fmll_throw_if(fmll_math_matrix_eigen((const double **) JJ, eigen_real, eigen_complex, num_weight, 1E-15));

				eta = 100 * (eigen_real[0] * eigen_real[0] + eigen_complex[0] * eigen_complex[0]) * eta_0;

				fmll_free(eigen_real);
				fmll_free(eigen_complex);

				eigen_real = NULL;
				eigen_complex = NULL;
			}

			if(eta != 0)
			{
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
					fmll_throw_if(fmll_math_matrix_transpose((const double **) J, JT, vec_num, num_weight));
					fmll_throw_if(fmll_math_matrix_init_main_diag(I, eta, num_weight, num_weight));
					fmll_throw_if(fmll_math_matrix_mult((const double **) JT, (const double **) J, JJ, num_weight, vec_num, num_weight));
					fmll_throw_if(fmll_math_matrix_sum(1, (const double **) I, 1, (const double **) JJ, JJ, num_weight, num_weight));

					/* Вычисление обратной матрицы для матрицы JJ */
					fmll_throw_if(fmll_math_matrix_inv((const double **) JJ, JJInv, num_weight));

					/*
					 * Вычисление корректировки весов нейронов перцептрона: d_w = - JJInv * grad
					 * (минус учитывается далее при корректировке весов нейронов перцептрона)
					 */
					fmll_throw_if(fmll_math_matrix_mult_vector((const double **) JJInv, grad, d_w, num_weight, num_weight));

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
							fmll_mlp_run(mlp, vec[u]);

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
			}

			fmll_print("Iteration = %u from %u (%.5f %%), eta = %.7f, E = %.7f, E' = %.7f\n", iter + 1, max_iter, (100.0 * (iter + 1)) / max_iter,
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

int fmll_mlp_teach_conjugate_gradient(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num, const unsigned max_iter,
		const double coef_E, const double E_thres, const double d_E_thres)
{
	const unsigned num_weight = mlp->num_weight, dim = mlp->dim, layers_num = mlp->layers_num, * N = mlp->N, t_num = omp_get_max_threads();
	const double norm_E = 2 * vec_num * N[layers_num - 1];
	bool first_run;
	int i, j, k, t_weight, t_w, ret = 0;
	unsigned c_N, p_N, u, v, iter, t_ind;
	double delta, E, E_coef_E, prev_E, t_E, beta_1, beta_2, beta, eta_1, eta[3], tE[3];
	double * t_y, *** y = mlp->y, * s = NULL, * prev_s = NULL,* grad = NULL, * prev_grad = NULL, * d_w = NULL, ** w = mlp->w;
	fmll_random * rnd = NULL;

	fmll_try;

		fmll_throw_if(coef_E <= 0 || coef_E >= 1 || E_thres < 0 || d_E_thres < 0);
		fmll_throw_null(grad = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(prev_grad = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(s = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(prev_s = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(d_w = fmll_alloc(sizeof(double), 1, num_weight));
		fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, NULL));

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
			E_coef_E = coef_E * grad_Jacobian(mlp, vec, d, vec_num, grad, NULL);

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
					eta[0] = fmll_random_generate(rnd) - 0.5;
					eta[2] = fmll_random_generate(rnd) - 0.5 + 0.0000000001;
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
							fmll_mlp_run(mlp, vec[v]);

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

			fmll_print("Iteration = %u from %u (%.5f %%), beta = %.7f, eta = %.7f, E = %.7f, E' = %.7f\n", iter + 1, max_iter,
					(100.0 * (iter + 1)) / max_iter, iter ? beta : 0, eta_1, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_random_destroy(rnd);
		fmll_free(d_w);
		fmll_free(grad);
		fmll_free(prev_grad);
		fmll_free(s);
		fmll_free(prev_s);

	return ret;
}

