
#include "ann/ff/ff.h"

struct t_fmll_ff_neuro
{

	fmll_ff_neuro ** in, ** out;
	unsigned ind, in_counter, out_counter, in_num, out_num;
	double net, y, E_out;
	double (* fun)(double);
	double (* d_fun)(double);

};

int fmll_ff_neuro_init(fmll_ff * ff);
void fmll_ff_neuro_destroy(fmll_ff * ff);
void fmll_ff_neuro_reset(fmll_ff * ff);
void fmll_ff_neuro_run(fmll_ff * ff, fmll_ff_neuro * neuro);
double fmll_ff_neuro_grad(fmll_ff * ff, double * d, double ** grad, double * grad_b);
void fmll_ff_neuro_grad_main(fmll_ff * ff, fmll_ff_neuro * neuro, double ** grad, double * grad_b);

int fmll_ff_neuro_init(fmll_ff * ff)
{
	int ret = 0;
	fmll_ff_neuro * neuro, ** in_neuro, ** out_neuro;
	unsigned char ** connect = ff->connect;
	unsigned v, u, in_counter, out_counter, in_num, out_num, in_dim = ff->in_dim, out_dim = ff->out_dim, num = ff->num, * in = ff->in, * out = ff->out;
	double (** fun)(double) = ff->fun;
	double (** d_fun)(double) = ff->d_fun

	fmll_try;

		ff->neuro = NULL;
		ff->in_neuro = ff->out_neuro = NULL;

		fmll_throw_null(neuro = ff->neuro = fmll_alloc(sizeof(fmll_ff_neuro), 1, num));
		fmll_throw_null(in_neuro = ff->in_neuro = fmll_alloc(sizeof(fmll_ff_neuro *), 1, in_dim));
		fmll_throw_null(out_neuro = ff->out_neuro = fmll_alloc(sizeof(fmll_ff_neuro *), 1, out_dim));

		for(v = 0; v < num; v++)
		{
			neuro[v].ind = v;
			neuro[v].in = neuro[v].out = NULL;
			neuro[v].in_num = neuro[v].out_num = neuro[v].in_counter = neuro[v].out_counter = 0;
			neuro[v].fun = fun[v];
			neuro[v].d_fun = d_fun[v];
		}

		for(v = 0; v < in_dim; v++)
			in_neuro[v] = NULL;

		for(v = 0; v < out_dim; v++)
			out_neuro[v] = NULL;

		for(v = 0, in_counter = 0, out_counter = 0; v < num; v++)
		{
			if(in_counter < in_dim && v == in[in_counter])
			{
				in_neuro[in_counter] = neuro + v;
				in_counter ++;
			}

			if(out_counter < out_dim && v == out[out_counter])
			{
				out_neuro[out_counter] = neuro + v;
				out_counter ++;
			}

			for(u = 0, in_num = 0, out_num = 0; u < num; u++)
			{
				if(connect[u][v])
					in_num ++;

				if(connect[v][u])
					out_num ++;
			}

			neuro[v].in_num = in_num;
			neuro[v].out_num = out_num;

			if(in_num)
				fmll_throw_null(neuro[v].in = fmll_alloc(sizeof(fmll_ff_neuro *), 1, in_num));

			if(out_num)
				fmll_throw_null(neuro[v].out = fmll_alloc(sizeof(fmll_ff_neuro *), 1, out_num));

			for(u = 0, in_num = 0, out_num = 0; u < num; u++)
			{
				if(connect[u][v])
				{
					neuro[v].in[in_num] = neuro + u;
					in_num ++;
				}

				if(connect[v][u])
				{
					neuro[v].out[out_num] = neuro + u;
					out_num ++;
				}
			}
		}

		fmll_ff_neuro_reset(ff);

	fmll_catch;

		fmll_ff_neuro_destroy(ff);
		ret = -1;

	fmll_finally;

	return ret;
}

void fmll_ff_neuro_destroy(fmll_ff * ff)
{
	unsigned v, num = ff->num;
	fmll_ff_neuro * neuro = ff->neuro;

	if(neuro != NULL)
	{
		for(v = 0; v < num; v++)
		{
			fmll_free(neuro[v].in);
			fmll_free(neuro[v].out);
		}
	}

	fmll_free(neuro);
	ff->neuro = NULL;

	fmll_free(ff->in_neuro);
	ff->in_neuro = NULL;

	fmll_free(ff->out_neuro);
	ff->out_neuro = NULL;
}

void fmll_ff_neuro_reset(fmll_ff * ff)
{
	unsigned v, num = ff->num;
	fmll_ff_neuro * neuro = ff->neuro;

	for(v = 0; v < num; v++, neuro++)
	{
		neuro->in_counter = neuro->out_counter = 0;
		neuro->net = neuro->y = neuro->E_out = 0;
	}
}

void fmll_ff_neuro_run(fmll_ff * ff, fmll_ff_neuro * neuro)
{
	fmll_ff_neuro ** out = neuro->out;
	unsigned v, ind = neuro->ind, out_num = neuro->out_num;
	double y, * w = ff->w[ind];

	/* Входные нейроны транслируют соответствующую компоненту входного вектора на свой выход, применяя к ней функцию активации */
	if(neuro->in_num)
		neuro->net -= ff->b[ind];
	
	y = neuro->y = (* neuro->fun)(neuro->net);

	for(v = 0; v < out_num; v++)
	{
		out[v]->net += w[out[v]->ind] * y;

		if(++ out[v]->in_counter == out[v]->in_num)
			fmll_ff_neuro_run(ff, out[v]);
	}
}

double fmll_ff_neuro_grad(fmll_ff * ff, double * d, double ** grad, double * grad_b)
{
	unsigned v, out_dim = ff->out_dim;
	double E, t_E;
	fmll_ff_neuro ** out_neuro = ff->out_neuro;

	for(v = 0, E = 0; v < out_dim; v++)
	{
		out_neuro[v]->E_out = t_E = d[v] - out_neuro[v]->y;
		fmll_ff_neuro_grad_main(ff, out_neuro[v], grad, grad_b);

		E += t_E * t_E;
	}

	return E / (2 * out_dim);
}

void fmll_ff_neuro_grad_main(fmll_ff * ff, fmll_ff_neuro * neuro, double ** grad, double * grad_b)
{
	unsigned v, in_ind, in_num = neuro->in_num, ind = neuro->ind;
	double E_out_net, ** w = ff->w;
	fmll_ff_neuro ** in = neuro->in;

	E_out_net = neuro->E_out * (* neuro->d_fun)(neuro->net);
	grad_b[ind] -= E_out_net;

	for(v = 0; v < in_num; v++)
	{
		in_ind = in[v]->ind;

		in[v]->E_out += E_out_net * w[in_ind][ind];
		grad[in_ind][ind] += E_out_net * in[v]->y;

		if(++ in[v]->out_counter == in[v]->out_num)
			fmll_ff_neuro_grad_main(ff, in[v], grad, grad_b);
	}
}

/* ############################################################################ */

fmll_ff * fmll_ff_init(unsigned num, unsigned char ** connect, double (** fun)(double), double (** d_fun)(double))
{
	fmll_ff * ff = NULL;
	unsigned char ** t_connect;
	unsigned v, u, * in, * out;
	double ** w;

	fmll_try;

		fmll_throw_null(ff = fmll_alloc(sizeof(fmll_ff), 1, 1));

		ff->connect = NULL;
		ff->num = num;
		ff->in_dim = ff->out_dim = 0;
		ff->in = ff->out = NULL;
		ff->y = ff->b = NULL;
		ff->w = NULL;
		ff->fun = ff->d_fun = NULL;
		ff->neuro = NULL;
		ff->in_neuro = ff->out_neuro = NULL;

		fmll_throw_null(t_connect = ff->connect = fmll_alloc(sizeof(unsigned char), 2, num, num));
		fmll_throw_null(in = ff->in = fmll_alloc(sizeof(unsigned), 1, num));
		fmll_throw_null(out = ff->out = fmll_alloc(sizeof(unsigned), 1, num));
		fmll_throw_null(ff->y = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(ff->b = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(w = ff->w = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(ff->fun = fmll_alloc(sizeof(double (*)(double)), 1, num));
		fmll_throw_null(ff->d_fun = fmll_alloc(sizeof(double (*)(double)), 1, num));

		for(v = 0; v < num; v++)
			for(u = 0; u < num; u++)
			{
				t_connect[v][u] = 0;
				w[v][u] = 0;
			}

		memcpy(ff->fun, fun, num * sizeof(double (*)(double)));
		memcpy(ff->d_fun, d_fun, num * sizeof(double (*)(double)));

		if(connect != NULL)
		{
			for(v = 0; v < num; v++)
				memcpy(t_connect[v], connect[v], num);

			fmll_throw_if(fmll_ff_init_2(ff));
		}

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
		fmll_ff_neuro_destroy(ff);
		fmll_free(ff->connect);
		fmll_free(ff->in);
		fmll_free(ff->out);
		fmll_free(ff->y);
		fmll_free(ff->b);
		fmll_free(ff->w);
		fmll_free(ff->fun);
		fmll_free(ff->d_fun);

		fmll_free(ff);
	}
}

int fmll_ff_init_2(fmll_ff * ff)
{
	int ret = 0;
	char is_in, is_out;
	unsigned char ** warsh = NULL, ** connect = ff->connect;
	unsigned v, u, in_dim, out_dim, num = ff->num, * in = ff->in, * out = ff->out;

	fmll_try;

		/* ############################################################################ */
		/* Поиск циклов */

		fmll_throw_null(warsh = fmll_alloc(sizeof(unsigned char), 2, num, num));
		fmll_throw_if(fmll_math_graph_warshall(connect, warsh, num));

		for(v = 0; v < num; v++)
			fmll_throw_if(warsh[v][v]);

		/* ############################################################################ */
		/* Инициализация in и out */

		in_dim = out_dim = 0;

		for(v = 0; v < num; v++)
		{
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

		fmll_throw_if(! (in_dim && out_dim));

		ff->in_dim = in_dim;
		ff->out_dim = out_dim;

		/* ############################################################################ */
		/* Инициализация кэша */

		fmll_throw_if(fmll_ff_neuro_init(ff));

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
	unsigned v, u;
	double * b, ** w;
	unsigned char ** connect;
	fmll_ff * ff = NULL;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_if(xml_load(fname_prefix, TYPE_FF, & main_node, & content_node));
		fmll_throw_if(xml_get_int(content_node, "num", & num));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "neuro", NULL, NULL, MXML_DESCEND_FIRST));

		fmll_throw_null(ff = fmll_ff_init(num, NULL, fun, d_fun));

		b = ff->b;
		w = ff->w;
		connect = ff->connect;

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

		fmll_throw_if(fmll_ff_init_2(ff));

	fmll_catch;

		fmll_ff_destroy(ff);
		ff = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return ff;
}

const double * fmll_ff_run(fmll_ff * ff, const double * vec)
{
	unsigned v, in_dim = ff->in_dim, out_dim = ff->out_dim;
	double * y = ff->y;
	fmll_ff_neuro ** in_neuro = ff->in_neuro, ** out_neuro = ff->out_neuro;

	fmll_ff_neuro_reset(ff);

	for(v = 0; v < in_dim; v++)
	{
		in_neuro[v]->net = vec[v];
		fmll_ff_neuro_run(ff, in_neuro[v]);
	}

	for(v = 0; v < out_dim; v++)
		y[v] = out_neuro[v]->y;

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

/* TODO Пакетное обратное распространение ошибки - распараллелить */
int fmll_ff_teach_gradient_batch(fmll_ff * ff, double ** vec, double ** d, unsigned vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, unsigned max_iter, double E_thres, double d_E_thres)
{
	int ret = 0;
	unsigned char ** connect = ff->connect;
	unsigned v, u, t, iter, num = ff->num, in_dim = ff->in_dim, * in = ff->in;
	double beta, E, prev_E, * b = ff->b, * moment_b = NULL, * grad_b = NULL, ** grad = NULL, ** moment = NULL, ** w = ff->w;

	fmll_try;

		fmll_throw_if(beta_0 < 0 || beta_0 > 1 || (! max_iter) || E_thres < 0 || d_E_thres < 0);
		fmll_throw_null(grad = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(moment = fmll_alloc(sizeof(double), 2, num, num));
		fmll_throw_null(grad_b = fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(moment_b = fmll_alloc(sizeof(double), 1, num));

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

				E += fmll_ff_neuro_grad(ff, d[t], grad, grad_b);
			}

			for(v = 0, t = 0; v < num; v++)
			{
				if(t < in_dim && in[t] == v) /* Входные нейроны игнорируются */
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

			E /= vec_num;

			fmll_print("Iteration = %u from %u (%.5f %%), beta = %.7f, E = %.7f, E' = %.7f\n", iter, max_iter, (100.0 * iter) / max_iter,
					beta, E, E - prev_E);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(grad);
		fmll_free(moment);
		fmll_free(grad_b);
		fmll_free(moment_b);

	return ret;
}

