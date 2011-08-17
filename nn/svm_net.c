
#include "svm_net.h"

fmll_svm_net * fmll_svm_net_init(unsigned num, unsigned dim, double (** K)(const double *, const double *, unsigned))
{
	fmll_try;

		unsigned u;
		fmll_svm ** svm;
		fmll_svm_net * svm_net = NULL;

		fmll_throw_null((svm_net = fmll_alloc_1D(1, sizeof(fmll_svm_net))));
		fmll_throw_null((svm = svm_net->svm = (fmll_svm **) fmll_alloc_1D(num, sizeof(fmll_svm *))));
		svm_net->num = num;

		for(u = 0; u < num; u++)
			svm[u] = NULL;

		for(u = 0; u < num; u++)
			fmll_throw_null((svm[u] = fmll_svm_init(dim, K[u])));
	
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
			unsigned u, num = svm_net->num;

			for(u = 0; u < num; u++)
				fmll_svm_destroy(svm[u]);

			fmll_free_ND(svm);
		}

		fmll_free_ND(svm_net);
	}
}

int fmll_svm_net_save(fmll_svm_net * svm_net, const char * fname_prefix)
{
	fmll_try;

		int ret = 0;
		char node_name[4096];
		unsigned u, num = svm_net->num;
		fmll_svm ** svm = svm_net->svm;
		mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;
		
		fmll_throw((xml_create(TYPE_SVM_NET, & main_node, & content_node)));
		fmll_throw((xml_set_int(content_node, "num", num)));
		fmll_throw_null((node = mxmlNewElement(content_node, "SVM")));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "svm_%u", u);
			fmll_throw_null((sub_node = mxmlNewElement(node, node_name)));
			fmll_throw((fmll_svm_save_main(svm[u], sub_node)));
		}

		fmll_throw((xml_save(fname_prefix, main_node)));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_svm_net * fmll_svm_net_load(const char * fname_prefix, double (** K)(const double *, const double *, unsigned))
{
	fmll_try;

		int num;
		unsigned u;
		char node_name[4096];
		fmll_svm ** svm = NULL;
		fmll_svm_net * svm_net = NULL;
		mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;

		fmll_throw_null((svm_net = fmll_alloc_1D(1, sizeof(fmll_svm_net))));
		fmll_throw((xml_load(fname_prefix, TYPE_SVM_NET, & main_node, & content_node)));
		fmll_throw((xml_get_int(content_node, "num", & num)));

		svm_net->num = num;

		fmll_throw_null((svm = svm_net->svm = (fmll_svm **) fmll_alloc_1D(num, sizeof(fmll_svm *))));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "SVM", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0; u < num; u++)
			svm[u] = NULL;

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "svm_%u", u);
			fmll_throw_null((sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST)));
			fmll_throw_null((svm[u] = fmll_svm_load_main(sub_node, K[u])));
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

unsigned fmll_svm_net_test(fmll_svm_net * svm_net, double ** vec, unsigned * d, unsigned vec_num,
		void (* st_func)(fmll_svm_net *, double *, unsigned, int, const double *, unsigned, bool, void *), void * st_param)
{
	bool is_right;
	int res;
	unsigned u, no = 0;
	double y[svm_net->num];

	for(u = 0; u < vec_num; u++)
	{
		if((res = fmll_svm_net_run(svm_net, vec[u], y)) != d[u])
		{
			no++;
			is_right = false;
		}

		if(st_func != NULL)
			(* st_func)(svm_net, vec[u], d[u], res, y, vec_num, is_right, st_param);
	}

	return vec_num - no;
}

int fmll_svm_net_teach_smo(fmll_svm_net * svm_net, double ** vec, unsigned * d, unsigned vec_num, double * C,
		int (** selector)(fmll_svm *, double **, char *, unsigned, int *, int *, double, double, double, double *, double *, double **),
		double * tau, unsigned * max_iter, double * epsilon)
{
	fmll_try;

		int ret = 0;
		char * rd;
		unsigned u, v, num = svm_net->num;
		fmll_svm ** svm = svm_net->svm;

		fmll_throw_null((rd = fmll_alloc_1D(vec_num, sizeof(char))));

		for(u = 0; u < num; u++)
		{
			for(v = 0; v < vec_num; v++)
			{
				if(d[v] == u)
					rd[v] = 1;
				else
					rd[v] = -1;
			}

			// TODO Распараллеливание в зависимости от ядра

			printf("\nМашина опорных векторов: %u из %u (%lf %%)\n", u + 1, num, (100 * (u + 1.0)) / num);

			fmll_throw((fmll_svm_teach_smo(svm[u], vec, rd, vec_num, C[u], selector[u], tau[u], max_iter[u], epsilon[u])));
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free_ND(rd);

	return ret;
}

