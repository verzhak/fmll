
#include "ann/svm/svm.h"

fmll_svm * fmll_svm_init(unsigned dim, double (* K)(const double *, const double *, unsigned))
{
	fmll_svm * svm = NULL;
		
	fmll_try;

		fmll_throw_null((svm = fmll_alloc(sizeof(fmll_svm), 1, 1)));
		svm->dim = dim;
		svm->w = NULL;
		svm->s = NULL;
		svm->K = K;

	fmll_catch;

	/* Нет смысла
		fmll_svm_destroy(svm);
		svm = NULL;
	*/

	fmll_finally;

	return svm;
}

void fmll_svm_destroy(fmll_svm * svm)
{
	if(svm != NULL)
	{
		fmll_free(svm->w);
		fmll_free(svm->s);
		fmll_free(svm);
	}
}

int fmll_svm_save(fmll_svm * svm, const char * fname_prefix)
{
	int ret = 0;
	mxml_node_t * content_node, * main_node = NULL;
		
	fmll_try;

		fmll_throw((xml_create(TYPE_SVM, & main_node, & content_node)));
		fmll_throw((fmll_svm_save_main(svm, content_node)));
		fmll_throw((xml_save(fname_prefix, main_node)));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

int fmll_svm_save_main(fmll_svm * svm, mxml_node_t * content_node)
{
	int ret = 0;
	char node_name[4096];
	unsigned u, v, dim = svm->dim, num = svm->num;
	double * w, ** s;
	mxml_node_t * sub_node, * node;
		
	fmll_try;

		fmll_throw_null((w = svm->w));
		fmll_throw_null((s = svm->s));
		fmll_throw((xml_set_int(content_node, "dim", dim)));
		fmll_throw((xml_set_int(content_node, "num", num)));
		fmll_throw((xml_set_double(content_node, "b", svm->b)));

		fmll_throw_null((node = mxmlNewElement(content_node, "W")));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw((xml_set_double(node, node_name, w[u])));
		}

		fmll_throw_null((node = mxmlNewElement(content_node, "S")));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "s_%u", u);
			fmll_throw_null((sub_node = mxmlNewElement(node, node_name)));

			for(v = 0; v < dim; v++)
			{
				sprintf(node_name, "%u", v);
				fmll_throw((xml_set_double(sub_node, node_name, s[u][v])));
			}
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

fmll_svm * fmll_svm_load(const char * fname_prefix, double (* K)(const double *, const double *, unsigned))
{
	fmll_svm * svm = NULL;
	mxml_node_t * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw((xml_load(fname_prefix, TYPE_SVM, & main_node, & content_node)));
		fmll_throw_null((svm = fmll_svm_load_main(content_node, K)));

	fmll_catch;

		fmll_svm_destroy(svm);
		svm = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return svm;
}

fmll_svm * fmll_svm_load_main(mxml_node_t * content_node, double (* K)(const double *, const double *, unsigned))
{
	char node_name[4096];
	int dim, num;
	unsigned u, v;
	double * w, ** s;
	fmll_svm * svm = NULL;
	mxml_node_t * sub_sub_node, * sub_node, * node;

	fmll_try;

		fmll_throw((xml_get_int(content_node, "dim", & dim)));
		fmll_throw((xml_get_int(content_node, "num", & num)));

		fmll_throw_null((svm = fmll_svm_init(dim, K)));

		svm->num = num;

		fmll_throw_null((w = svm->w = fmll_alloc(sizeof(double), 1, num)));
		fmll_throw_null((s = svm->s = (double **) fmll_alloc(sizeof(double), 2, num, dim)));
		fmll_throw((xml_get_double(content_node, "b", & svm->b)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < num; u++)
		{
			fmll_throw_null((sub_node));
			w[u] = sub_node->child->value.real;
			sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
		}

		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "S", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "s_%u", u);
			fmll_throw_null((sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST)));

			for(v = 0, sub_sub_node = mxmlFindElement(sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); v < dim; v++)
			{
				fmll_throw_null((sub_sub_node));
				s[u][v] = sub_sub_node->child->value.real;
				sub_sub_node = mxmlFindElement(sub_sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND);
			}
		}

	fmll_catch;

		fmll_svm_destroy(svm);
		svm = NULL;

	fmll_finally;

	return svm;
}

double fmll_svm_run(fmll_svm * svm, const double * vec)
{
	unsigned u, dim = svm->dim, num = svm->num;
	double sum = - svm->b, * w = svm->w, ** s = svm->s;
	double (* K)(const double *, const double *, unsigned) = svm->K;

	for(u = 0; u < num; u++)
		sum += w[u] * (* K)(s[u], vec, dim);

	if(sum > 1 - 1E-10)
		sum = 1;
	else if(sum < -1 + 1E-10)
		sum = -1;
	else if(fabs(sum) < 1E-10)
		sum = 0;

	return sum;
}

unsigned fmll_svm_test(fmll_svm * svm, double ** vec, char * d, unsigned vec_num,
		void (* st_func)(fmll_svm *, double *, char, double, unsigned, bool, void *), void * st_param)
{
	bool is_right;
	double res;
	unsigned u, yes = 0;

	for(u = 0; u < vec_num; u++)
	{
		res = fmll_svm_run(svm, vec[u]);
		
		if(
			(res > 0 && d[u] == 1)
			||
			(res < 0 && d[u] == -1)
		  )
		{
			yes++;
			is_right = true;
		}
		else
			is_right = false;

		if(st_func != NULL)
			(* st_func)(svm, vec[u], d[u], res, vec_num, is_right, st_param);
	}

	return yes;
}

int fmll_svm_teach_smo(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, double C, double tau,
		int (* selector)(fmll_svm *, double **, char *, unsigned, int *, int *, double, double, double, double *, double *, double **),
		unsigned max_iter, double epsilon)
{
	int i, j, ret = 0;
	unsigned u, v, iter, num, dim = svm->dim;
	double a, b, sum, prev_lambda[2], * w, ** s, * lambda = NULL, * grad = NULL, ** Q = NULL;
	double (* K)(const double *, const double *, unsigned) = svm->K;

	fmll_try;

		fmll_throw((tau <= 0));
		fmll_throw((epsilon <= 0));
		fmll_throw_null((lambda = fmll_alloc(sizeof(double), 1, vec_num)));
		fmll_throw_null((grad = fmll_alloc(sizeof(double), 1, vec_num)));
		fmll_throw_null((Q = (double **) fmll_alloc(sizeof(double), 2, vec_num, vec_num)));

		/* ############################################################################  */

		for(u = 0; u < vec_num; u++)
			lambda[u] = 0;

		for(u = 0; u < vec_num; u++)
		{
			grad[u] = -1;

			for(v = 0; v < u; v++)
				grad[u] += lambda[v] * Q[u][v];

			Q[u][u] = d[u] * d[u] * (* K)(vec[u], vec[u], dim);
			grad[u] += lambda[u] * Q[u][u];

			for(v = u + 1; v < vec_num; v++)
			{
				Q[v][u] = Q[u][v] = d[v] * d[u] * (* K)(vec[v], vec[u], dim);
				grad[u] += lambda[v] * Q[u][v];
			}
		}

		/* ############################################################################  */

		for(iter = 0; iter < max_iter; iter++)
		{
			/* ############################################################################  */
			/* Выбор */

			if((* selector)(svm, vec, d, vec_num, & i, & j, C, tau, epsilon, lambda, grad, Q))
				break;

			/* ############################################################################  */
			/* Оптимизация */

			prev_lambda[0] = lambda[i];
			prev_lambda[1] = lambda[j];

			if((a = Q[i][i] + Q[j][j] - 2 * Q[i][j] * d[i] * d[j]) <= 0)
				a = tau;

			b = - d[i] * grad[i] + d[j] * grad[j];

			/*

			Задача: минимизировать квадратичную форму F(lambda) = (1 / 2 * lambdaT * Q * lambda + lambda) по переменным lambda[i] и lambda[j].

			Решение:

			1) Введем переменную t[i] = - t[j] = p[i]d[i] = - p[j]d[j]

			Равенство t[i] = - t[j] следует из необходимости соблюдения одного из условий ККТ - условия дополняющей нежесткости: sum(p[i]d[i]) = 0.
			Здесь p[i] и p[j] - искомые приращения, взятые с обратным знаком, для lambda[i] и lambda[j] соответственно;

			2) F(lambda) = 1 / 2 * a * t[i]^2 + b * t[i]

			См. Fan, Chen, Lin - стр. 1894 (6)

			3) F'(lambda) = a * t[i] + b = 0

			=>

			t[i]opt = - b / a

			4) p[i] = d[i] * (- b / a) = - d[i] * b / a
			p[j] = - d[j] * (- b / a) = d[j] * b / a

			Так как мы движемся в направлении анти-градиента, имеем:

			lambda[i] -= p[i] => lambda[i] += d[i] * b / a
			lambda[j] -= p[j] => lambda[i] -= d[j] * b / a

			5) Условие дополняющей нежесткости: sum(d[i] * lambda[i]) = 0 (т.е. переменная sum == 0).

			Изначально: выполняется, так как lambda инициализируется нулями.

			lambda_new_new[j] = d[j] * (sum - d[i] * lambda_new[i]) = d[j] * (d[j] * lambda[j] + d[i] * lambda[i] - d[i] * (lambda[i] + d[i] * b / a)) =
				lambda[j] + d[j] * d[i] * lambda[i] - d[j] * d[i] * lambda[i] + d[j] * b / a = lambda[j] + d[j] * b / a

			d[j] * lambda_new_new[j] = d[j] * lambda[j] + b / a

			lambda_new_new[i] = d[i] * (d[i] * lambda[i] + d[j] * lambda[j] - d[j] * lambda_new_new[j]) =
				lambda[i] + d[i] * d[j] * lambda[j] - d[i] * d[j] * lambda[j] - d[i] * b / a = lambda[i] - d[i] * b / a

			d[i] * lambda_new_new[i] = d[i] * lambda[i] - b / a

			=>

			sum_new_new = d[i] * lambda_new_new[i] + d[j] * lambda_new_new[j] = d[i] * lambda[i] + d[j] * lambda[j] = 0

			*/

			sum = d[i] * lambda[i] + d[j] * lambda[j];
			lambda[i] += d[i] * b / a;
			lambda[j] -= d[j] * b / a;

			if(lambda[i] > C)
				lambda[i] = C;
			else if(lambda[i] < 0)
				lambda[i] = 0;

			lambda[j] = d[j] * (sum - d[i] * lambda[i]);

			if(lambda[j] > C)
				lambda[j] = C;
			else if(lambda[j] < 0)
				lambda[j] = 0;

			lambda[i] = d[i] * (sum - d[j] * lambda[j]);

			/* ############################################################################  */
			/* Пересчет градиента */

			for(u = 0; u < vec_num; u++)
				grad[u] += (lambda[i] - prev_lambda[0]) * Q[u][i] + (lambda[j] - prev_lambda[1]) * Q[u][j];

			/* ############################################################################  */

			fmll_print("Iteration = %u from %u (%lf %%)\n", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter);
		}

		/* ############################################################################  */
		/* Расчет w, b и s */

		num = 0;

		for(u = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
				num++;

		svm->num = num;

		fmll_throw_null((w = svm->w = fmll_alloc(sizeof(double), 1, num)));
		fmll_throw_null((s = svm->s = (double **) fmll_alloc(sizeof(double), 2, num, dim)));

		for(u = 0, v = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
			{
				w[v] = lambda[u] * d[u];
				memcpy(s[v], vec[u], dim * sizeof(double));

				v++;
			}

		/* Рекомендация по рассчету b - в "Лекции по методу опорных векторов" К.В. Воронцов (страница 4) */
		for(u = 0, b = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
			{
				for(v = 0; v < num; v++)
					b += w[v] * (* K)(s[v], vec[u], dim);

				b -= d[u];
			}

		svm->b = b / num;
		
		/* ############################################################################  */
		
	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(lambda);
		fmll_free(grad);
		fmll_free(Q);

	return ret;
}

int fmll_svm_teach_smo_selector_keerthi(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, int * ri, int * rj,
		double C, double tau, double epsilon, double * lambda, double * grad, double ** Q)
{
	int i, j;
	unsigned u;
	double max, min;

	i = -1;

	for(u = 0; u < vec_num; u++)
		if(
			(
				(lambda[u] < C && d[u] == 1)
				||
				(lambda[u] > 0 && d[u] == -1)
			)
			&&
			(
				i == -1
				||
				- d[u] * grad[u] > max
			)
		  )
		{
			i = u;
			max = - d[u] * grad[u];
		}

	if(i == -1)
		return -1;

	j = -1;

	for(u = 0; u < vec_num; u++)
		if(
			u != i
			&&
			(
				(lambda[u] < C && d[u] == -1)
				||
				(lambda[u] > 0 && d[u] == 1)
			)
			&&
			(
				j == -1
				||
				- d[u] * grad[u] < min
			)
			&&
			max > - d[u] * grad[u]
		  )
		{
			j = u;
			min = - d[u] * grad[u];
		}

	if(j == -1 || min >= max || max - min < epsilon)
		return -1;

	(* ri) = i;
	(* rj) = j;

	return 0;
}

int fmll_svm_teach_smo_selector_fan_chen_lin(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, int * ri, int * rj,
		double C, double tau, double epsilon, double * lambda, double * grad, double ** Q)
{
	int i, j;
	unsigned u;
	double a, b, max, min;

	i = -1;

	for(u = 0; u < vec_num; u++)
		if(
			(
				(lambda[u] < C && d[u] == 1)
				||
				(lambda[u] > 0 && d[u] == -1)
			)
			&&
			(
				i == -1
				||
				- d[u] * grad[u] > max
			)
		  )
		{
			i = u;
			max = - d[u] * grad[u];
		}

	if(i == -1)
		return -1;

	j = -1;

	for(u = 0; u < vec_num; u++)
		if(u != i)
		{
			if((a = Q[u][u] + Q[i][i] - 2 * Q[u][i] * d[u] * d[i]) <= 0)
				a = tau;

			b = max + d[u] * grad[u];

			if(
				(
					(lambda[u] < C && d[u] == -1)
					||
					(lambda[u] > 0 && d[u] == 1)
				)
				&&
				(
					j == -1
					||
					- b * b / a < min
				)
				&&
				max > - d[u] * grad[u]
			  )
			{
				j = u;
				min = - b * b / a;
			}
		}

	if(j == -1 || min >= max || max - min < epsilon)
		return -1;

	(* ri) = i;
	(* rj) = j;

	return 0;
}

