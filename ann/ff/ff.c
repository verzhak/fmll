
#include "ann/ff/ff.h"

fmll_ff * fmll_ff_init(unsigned num, double (** fun)(double), double (** d_fun)(double))
{
	fmll_ff * ff = NULL;
	unsigned char ** connect;
	unsigned v, u;
	double ** w;

	fmll_try;

		fmll_throw_null(ff = fmll_alloc(sizeof(fmll_ff), 1, 1));

		ff->connect = NULL;
		ff->num = num;
		ff->in_dim = ff->out_dim = 0;
		ff->in = ff->out = NULL;
		ff->ry = ff->y = ff->b = NULL;
		ff->w = NULL;
		ff->fun = ff->d_fun = NULL;
		ff->is_run = NULL;
		ff->net = NULL;

		fmll_throw_null(ff->is_run = fmll_alloc(sizeof(unsigned char), 1, num));
		fmll_throw_null(connect = ff->connect = fmll_alloc(sizeof(unsigned char), 2, num, num));
		fmll_throw_null(ff->in = fmll_alloc(sizeof(unsigned), 1, num));
		fmll_throw_null(ff->out = fmll_alloc(sizeof(unsigned), 1, num));
		fmll_throw_null(ff->ry = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(ff->y = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(ff->net = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(ff->b = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(w = ff->w = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(ff->fun = fmll_alloc(sizeof(double (*)(double)), 1, num));
		fmll_throw_null(ff->d_fun = fmll_alloc(sizeof(double (*)(double)), 1, num));

		for(v = 0; v < num; v++)
			for(u = 0; u < num; u++)
			{
				connect[v][u] = 0;
				w[v][u] = 0;
			}

		memcpy(ff->fun, fun, num * sizeof(double (*)(double)));
		memcpy(ff->d_fun, d_fun, num * sizeof(double (*)(double)));

	fmll_catch;

		fmll_ff_destroy(ff);

		ff = NULL;

	fmll_finally;

	return ff;
}

void fmll_ff_destroy(fmll_ff * ff)
{
	if(ff != NULL)
	{
		fmll_free(ff->is_run);
		fmll_free(ff->connect);
		fmll_free(ff->in);
		fmll_free(ff->out);
		fmll_free(ff->ry);
		fmll_free(ff->y);
		fmll_free(ff->net);
		fmll_free(ff->b);
		fmll_free(ff->w);
		fmll_free(ff->fun);
		fmll_free(ff->d_fun);

		fmll_free(ff);
	}
}

/* TODO Матрица смежности -> указательный граф */
int fmll_ff_set_connect(fmll_ff * ff, unsigned char ** connect)
{
	int ret = 0;
	char is_in, is_out;
	unsigned char ** warsh = NULL, ** t_connect = ff->connect;
	unsigned v, u, in_dim, out_dim, num = ff->num, * in = ff->in, * out = ff->out;

	fmll_try;

		/* ############################################################################ */
		/* Поиск циклов */

		fmll_throw_null(warsh = fmll_alloc(sizeof(unsigned char), 2, num, num));
		fmll_throw_if(fmll_math_graph_warshall(connect, warsh, num));

		for(v = 0; v < num; v++)
			fmll_throw_if(warsh[v][v]);

		/* ############################################################################ */
		/* Инициализация ff->connect, in и out */

		in_dim = out_dim = 0;

		for(v = 0; v < num; v++)
		{
			memcpy(t_connect[v], connect[v], num * sizeof(unsigned char));

			/* Строка - из, стобец - в */
			for(u = 0, is_in = 1, is_out = 1; u < num && (is_in || is_out); u++)
			{
				if(connect[v][u])
					is_out = 0;

				if(connect[u][v])
					is_in = 0;
			}

			/* Допустимо, так как тип is_in и is_out - char и true кодируется как 1
			 * (в случае bool - неизвестно, как ведет себя побитовый XOR с (макросами) true и false) */
			if(is_in ^ is_out)
			{
				if(is_in)
				{
					in[in_dim] = v;
					in_dim ++;
				}

				if(is_out)
				{
					out[out_dim] = v;
					out_dim ++;
				}
			}
		}

		fmll_throw_if(! (in_dim && out_dim))

		ff->in_dim = in_dim;
		ff->out_dim = out_dim;

	fmll_catch;

		ff->in_dim = 0;
		ff->out_dim = 0;

		ret = -1;

	fmll_finally;

		fmll_free(warsh);

	return ret;
}

int fmll_ff_save(fmll_ff * ff, const char * fname_prefix)
{
	int ret = 0;
	char node_name[4096];
	unsigned char ** connect = ff->connect;
	unsigned v, u, num = ff->num;
	double * b = ff->b, ** w = ff->w;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;
		
	fmll_try;

		fmll_throw_if(xml_create(TYPE_FF, & main_node, & content_node));
		fmll_throw_if(xml_set_int(content_node, "num", num));

		fmll_throw_null(node = mxmlNewElement(content_node, "neuro"));

		for(v = 0; v < num; v++)
		{
			sprintf(node_name, "neuro_%u", v);
			fmll_throw_null(sub_node = mxmlNewElement(node, node_name));

			sprintf(node_name, "b");
			fmll_throw_if(xml_set_double(sub_node, node_name, b[v]));

			for(u = 0; u < num; u++)
			{
				sprintf(node_name, "w_%u", u);
				fmll_throw_if(xml_set_double(sub_node, node_name, w[v][u]));

				sprintf(node_name, "connect_%u", u);
				fmll_throw_if(xml_set_uchar(sub_node, node_name, connect[v][u]));
			}
		}

		fmll_throw_if(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_ff * fmll_ff_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double))
{
	int num;
	char node_name[4096];
	unsigned char ** connect = NULL;
	unsigned v, u;
	double * b, ** w;
	fmll_ff * ff = NULL;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_if(xml_load(fname_prefix, TYPE_FF, & main_node, & content_node));
		fmll_throw_if(xml_get_int(content_node, "num", & num));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "neuro", NULL, NULL, MXML_DESCEND_FIRST));

		fmll_throw_null(ff = fmll_ff_init(num, fun, d_fun));
		fmll_throw_null(connect = fmll_alloc(sizeof(unsigned char), 2, num, num));

		b = ff->b;
		w = ff->w;

		for(v = 0; v < num; v++)
		{
			sprintf(node_name, "neuro_%u", v);
			fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

			sprintf(node_name, "b");
			fmll_throw_if(xml_get_double(sub_node, node_name, b + v));

			for(u = 0; u < num; u++)
			{
				sprintf(node_name, "w_%u", u);
				fmll_throw_if(xml_get_double(sub_node, node_name, w[v] + u));

				sprintf(node_name, "connect_%u", u);
				fmll_throw_if(xml_get_uchar(sub_node, node_name, connect[v] + u));
			}
		}

		fmll_throw_if(fmll_ff_set_connect(ff, connect));

	fmll_catch;

		fmll_ff_destroy(ff);
		ff = NULL;

	fmll_finally;

		fmll_free(connect);
		xml_destroy(main_node);

	return ff;
}

const double * fmll_ff_run(fmll_ff * ff, const double * vec)
{
	unsigned char * is_run = ff->is_run, ** connect = ff->connect;
	unsigned v, u, run_counter = 0, num = ff->num, in_dim = ff->in_dim, out_dim = ff->out_dim, * in = ff->in, * out = ff->out;
	double t_ry, * net = ff->net, * y = ff->y, * ry = ff->ry, * b = ff->b, ** w = ff->w;
	double (** fun)(double) = ff->fun;

	for(v = 0; v < num; v++)
	{
		is_run[v] = 0;
		net[v] = - b[v];
	}

	/* Входные нейроны транслируют соответствующую компоненту входного вектора на свой выход, применяя к ней функцию активации */
	for(v = 0; v < in_dim; v++)
		net[in[v]] = vec[v];

	while(run_counter < num)
	{
		/* Строка - из, стобец - в */
		for(v = 0; v < num; v++)
			if(! is_run[v])
			{
				for(u = 0; u < num; u++)
					if(connect[u][v] && (! is_run[u]))
						break;

				if(u == num)
				{
					ry[v] = t_ry = (* fun[v])(net[v]);
					is_run[v] = 1;
					run_counter++;

					for(u = 0; u < num; u++)
						if(connect[v][u])
							net[u] += t_ry * w[v][u];
				}
			}
	}

	for(v = 0; v < out_dim; v++)
		y[v] = ry[out[v]];

	return y;
}

unsigned fmll_ff_test(fmll_ff * ff, double ** vec, double ** d, double * deviation, unsigned vec_num,
		void (* st_func)(fmll_ff *, double *, double *, const double *, unsigned, bool, void *), void * st_param)
{
	bool is_right;
	unsigned u, v, no = 0, out_dim = ff->out_dim;
	const double * y;

	for(u = 0; u < vec_num; u++)
	{
		y = fmll_ff_run(ff, vec[u]);

		for(v = 0, is_right = true; v < out_dim && is_right; v++)
			if(fabs(y[v] - d[u][v]) > deviation[v])
			{
				no++;
				is_right = false;
			}

		if(st_func != NULL)
			(* st_func)(ff, vec[u], d[u], y, vec_num, is_right, st_param);
	}

	return vec_num - no;
}

/* TODO Однослойная сеть */
/* TODO Обратное распространение ошибки - в отдельную функцию */
/* TODO Пакетное обратное распространение ошибки - распараллелить */
int fmll_ff_teach_gradient_batch(fmll_ff * ff, double ** vec, double ** d, unsigned vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, unsigned max_iter, double E_thres, double d_E_thres)
{
	int ret = 0;
	unsigned char * is_acc = NULL, ** connect = ff->connect;
	unsigned v, u, t, iter, acc_counter, num = ff->num, out_dim = ff->out_dim, * in = ff->in, * out = ff->out;
	double beta, E, prev_E, norm_E = 2 * vec_num * out_dim, t_E, E_out_net, * E_out = NULL, * net = ff->net, * b = ff->b, * moment_b = NULL, * grad_b = NULL,
		   * ry = ff->ry, * y = ff->y, ** grad = NULL, ** moment = NULL, ** w = ff->w;
	double (** d_fun)(double) = ff->d_fun;

	fmll_try;

		fmll_throw_if(beta_0 < 0 || beta_0 > 1 || (! max_iter) || E_thres < 0 || d_E_thres < 0);
		fmll_throw_null(grad = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(moment = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(grad_b = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(moment_b = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(E_out = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(is_acc = fmll_alloc(sizeof(unsigned char), 1, num));

		for(v = 0; v < num; v++)
		{
			grad_b[v] = moment_b[v] = 0;

			for(u = 0; u < num; u++)
				if(connect[v][u])
					grad[v][u] = moment[v][u] = 0;
		}

		for(iter = 1, beta = (beta_0 > 0) ? beta_0 : (* next_beta)(0), E = E_thres + 1, prev_E = E_thres + 1 + 2 * d_E_thres;
				iter <= max_iter && E > E_thres && (fabs(E - prev_E) > d_E_thres || iter <= 100); iter++)
		{
			if((beta = (* next_beta)(beta)) > 1.00000001)
				beta = 1;

			prev_E = E;
			E = 0;

			for(t = 0; t < vec_num; t++)
			{
				fmll_throw_null(fmll_ff_run(ff, vec[t]));

				acc_counter = 0;
				memset(is_acc, 0, sizeof(unsigned char) * num);

				for(v = 0; v < num; v++)
					E_out[v] = 0;

				for(v = 0; v < out_dim; v++)
				{
					E_out[out[v]] = t_E = d[t][v] - y[v];
					E += t_E * t_E;
				}

				while(acc_counter < num)
					for(v = 0; v < num; v++) /* Строка - из, стобец - в */
						if(! is_acc[v])
						{
							for(u = 0; u < num; u++)
								if(connect[v][u] && (! is_acc[u]))
									break;

							if(u == num)
							{
								E_out_net = E_out[v] * d_fun[v](net[v]);
								grad_b[v] -= E_out_net;
								is_acc[v] = 1;
								acc_counter ++;

								for(u = 0; u < num; u++)
									if(connect[u][v])
									{
										grad[u][v] += E_out_net * ry[u];
										E_out[u] += E_out_net * w[u][v];
									}
							}
						}
			}

			for(v = 0, t = 0; v < num; v++)
			{
				if(in[t] == v) /* Входные нейроны игнорируются */
					t++;
				else
				{
					moment_b[v] = coef_moment * moment_b[v] + beta * grad_b[v] / vec_num;
					b[v] += moment_b[v];
					grad_b[v] = 0;

					for(u = 0; u < num; u++)
						if(connect[u][v])
						{
							moment[u][v] = coef_moment * moment[u][v] + beta * grad[u][v] / vec_num;
							w[u][v] += moment[u][v];
							grad[u][v] = 0;
						}
				}
			}

			E /= norm_E;

			fmll_print("Iteration = %u from %u (%.5f %%), beta = %.7f, E = %.7f, E' = %.7f\n", iter, max_iter, (100.0 * iter) / max_iter,
					beta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(E_out);
		fmll_free(grad);
		fmll_free(moment);
		fmll_free(grad_b);
		fmll_free(moment_b);
		fmll_free(is_acc);

	return ret;
}

