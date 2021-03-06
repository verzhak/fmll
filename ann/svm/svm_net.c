
#include "ann/svm/svm_net.h"

fmll_svm_net * fmll_svm_net_init(const unsigned num, const unsigned dim, double (** K)(const double *, const double *, const unsigned))
{
	unsigned u;
	fmll_svm ** svm;
	fmll_svm_net * svm_net = NULL;

	fmll_try;

		fmll_throw_null(svm_net = fmll_alloc(sizeof(fmll_svm_net), 1, 1));
		fmll_throw_null(svm = svm_net->svm = (fmll_svm **) fmll_alloc(sizeof(fmll_svm *), 1, num));
		svm_net->num = num;

		for(u = 0; u < num; u++)
			svm[u] = NULL;

		for(u = 0; u < num; u++)
			fmll_throw_null(svm[u] = fmll_svm_init(dim, K[u]));
	
	fmll_catch;

		fmll_svm_net_destroy(svm_net);
		svm_net = NULL;

	fmll_finally;

	return svm_net;
}

void fmll_svm_net_destroy(fmll_svm_net * svm_net)
{
	if(svm_net != NULL)
	{
		fmll_svm ** svm = svm_net->svm;

		if(svm != NULL)
		{
			const unsigned num = svm_net->num;
			unsigned u;

			for(u = 0; u < num; u++)
				fmll_svm_destroy(svm[u]);

			fmll_free(svm);
		}

		fmll_free(svm_net);
	}
}

int fmll_svm_net_save(const fmll_svm_net * svm_net, const char * fname_prefix)
{
	int ret = 0;
	const unsigned num = svm_net->num;
	unsigned u;
	char node_name[4096];
	const fmll_svm ** svm = (const fmll_svm **) svm_net->svm;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;
		
	fmll_try;

		fmll_throw_if(xml_create(TYPE_SVM_NET, & main_node, & content_node));
		fmll_throw_if(xml_set_int(content_node, "num", num));
		fmll_throw_null(node = mxmlNewElement(content_node, "SVM"));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "svm_%u", u);
			fmll_throw_null(sub_node = mxmlNewElement(node, node_name));
			fmll_throw_if(fmll_svm_save_main(svm[u], sub_node));
		}

		fmll_throw_if(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_svm_net * fmll_svm_net_load(const char * fname_prefix, double (** K)(const double *, const double *, const unsigned))
{
	int num;
	unsigned u;
	char node_name[4096];
	fmll_svm ** svm = NULL;
	fmll_svm_net * svm_net = NULL;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_null(svm_net = fmll_alloc(sizeof(fmll_svm_net), 1, 1));
		fmll_throw_if(xml_load(fname_prefix, TYPE_SVM_NET, & main_node, & content_node));
		fmll_throw_if(xml_get_int(content_node, "num", & num));

		svm_net->num = num;

		fmll_throw_null(svm = svm_net->svm = (fmll_svm **) fmll_alloc(sizeof(fmll_svm *), 1, num));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "SVM", NULL, NULL, MXML_DESCEND_FIRST));

		for(u = 0; u < num; u++)
			svm[u] = NULL;

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "svm_%u", u);
			fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));
			fmll_throw_null(svm[u] = fmll_svm_load_main(sub_node, K[u]));
		}
	
	fmll_catch;

		fmll_svm_net_destroy(svm_net);
		svm_net = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return svm_net;
}

int fmll_svm_net_run(fmll_svm_net * svm_net, const double * vec, double * y)
{
	/*
	   Возвращаемые значения:

			- номер машины опорных векторов, давшей результат, равный 1, если таковая машина опорных векторов одна;
			- -1 - если ни одна из машин опорных векторов не дала результат, равный 1;
			- -2 - если несколько машин опорных векторов дали результат, равный 1;
			- < -2 - в случае невозможности корректно завершить прогон машины опорных векторов над целевым вектором.
	*/

	int ind = -1;
	unsigned u, num = svm_net->num;
	double ty;
	fmll_svm ** svm = svm_net->svm;

	for(u = 0; u < num; u++)
	{
		ty = fmll_svm_run(svm[u], vec);

		if(y != NULL)
			y[u] = ty;

		if(ty > 0 && ind != -2)
		{
			if(ind == -1)
				ind = u;
			else
				ind = -2;
		}
	}

	return ind;
}

unsigned fmll_svm_net_test(fmll_svm_net * svm_net, const double ** vec, const unsigned * d, const unsigned vec_num,
		void (* st_func)(const fmll_svm_net *, const double *, const unsigned, const int, const double *, const unsigned, const bool, void *), void * st_param)
{
	bool is_right;
	int res;
	unsigned u, yes = 0;
	double * y;
	
	fmll_try;

		fmll_throw_null(y = fmll_alloc_a(sizeof(double) * svm_net->num));

		for(u = 0; u < vec_num; u++)
		{
			if((res = fmll_svm_net_run(svm_net, vec[u], y)) == d[u])
			{
				yes++;
				is_right = true;
			}
			else
				is_right = false;

			if(st_func != NULL)
				(* st_func)(svm_net, vec[u], d[u], res, y, vec_num, is_right, st_param);
		}

	fmll_catch;

		yes = 0;

	fmll_finally;

	return yes;
}

int fmll_svm_net_teach_smo(fmll_svm_net * svm_net, const double ** vec, const unsigned * d, const unsigned vec_num, const double * C, const double * tau,
		int (** selector)(const fmll_svm *, const double **, const char *, const unsigned, int *, int *, const double, const double, const double, const double *, const double *, const double **),
		const unsigned * max_iter, const double * epsilon)
{
	int ret = 0;
	const unsigned num = svm_net->num, t_num = omp_get_max_threads();
	unsigned u, v, t_ind;
	char * t_rd, ** rd;
	fmll_svm ** svm = svm_net->svm;

	fmll_try;

		fmll_throw_null(rd = (char **) fmll_alloc(sizeof(char), 2, t_num, vec_num));

		#pragma omp parallel private(u, v, t_ind, t_rd) default(shared)
		{
			t_ind = omp_get_thread_num();
			t_rd = rd[t_ind];

			for(u = t_ind; u < num; u += t_num)
			{
				for(v = 0; v < vec_num; v++)
				{
					if(d[v] == u)
						t_rd[v] = 1;
					else
						t_rd[v] = -1;
				}

				fmll_print("\nSupport vector machine: %u from %u (%.5f %%)\n", u + 1, num, (100 * (u + 1.0)) / num);

				fmll_svm_teach_smo(svm[u], (const double **) vec, t_rd, vec_num, C[u], tau[u], selector[u], max_iter[u], epsilon[u]);
			}
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(rd);

	return ret;
}

