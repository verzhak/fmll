
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
	unsigned v, u, ind, run_counter = 0, num = ff->num, in_dim = ff->in_dim, out_dim = ff->out_dim, * in = ff->in, * out = ff->out;
	double * net = ff->net, * y = ff->y, * ry = ff->ry, * b = ff->b, ** w = ff->w;
	double (** fun)(double) = ff->fun;

	for(v = 0; v < num; v++)
	{
		is_run[v] = 0;
		net[v] = - b[v];
	}

#define RUN_NEURO(ind) \
{\
	ry[ind] = (* fun[ind])(net[ind]);\
	is_run[ind] = 1;\
	run_counter ++;\
\
	for(u = 0; u < num; u++)\
		if(connect[ind][u])\
			net[u] += ry[ind] * w[ind][u];\
}

	for(v = 0; v < in_dim; v++)
	{
		ind = in[v];
		net[ind] += vec[v];

		RUN_NEURO(ind);
	}

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
					RUN_NEURO(v);
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

