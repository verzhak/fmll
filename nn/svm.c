
#include "svm.h"

fmll_svm * fmll_svm_init(unsigned dim, double (* K)(const double *, const double *, unsigned))
{
	fmll_try;

		fmll_svm * svm = NULL;
		
		fmll_throw_null((svm = fmll_alloc_1D(1, sizeof(fmll_svm))));
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
		fmll_free_ND(svm->w);
		fmll_free_ND(svm->s);
		fmll_free_ND(svm);
	}
}

int fmll_svm_save_main(fmll_svm * svm, mxml_node_t * content_node)
{
	fmll_try;

		int ret = 0;
		char node_name[4096];
		unsigned u, v, dim = svm->dim, num = svm->num;
		double * w, ** s;
		mxml_node_t * sub_node, * node;
		
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

int fmll_svm_save(fmll_svm * svm, const char * fname_prefix)
{
	fmll_try;

		int ret = 0;
		mxml_node_t * content_node, * main_node = NULL;
		
		fmll_throw((xml_create(TYPE_SVM, & main_node, & content_node)));
		fmll_throw((fmll_svm_save_main(svm, content_node)));
		fmll_throw((xml_save(fname_prefix, main_node)));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_svm * fmll_svm_load_main(mxml_node_t * content_node, double (* K)(const double *, const double *, unsigned))
{
	fmll_try;

		char node_name[4096];
		int dim, num;
		unsigned u, v;
		double * w, ** s;
		fmll_svm * svm = NULL;
		mxml_node_t * sub_sub_node, * sub_node, * node;

		fmll_throw((xml_get_int(content_node, "dim", & dim)));
		fmll_throw((xml_get_int(content_node, "num", & num)));

		fmll_throw_null((svm = fmll_svm_init(dim, K)));

		svm->num = num;

		fmll_throw_null((w = svm->w = fmll_alloc_1D(num, sizeof(double))));
		fmll_throw_null((s = svm->s = (double **) fmll_alloc_2D(num, dim, sizeof(double))));
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

fmll_svm * fmll_svm_load(const char * fname_prefix, double (* K)(const double *, const double *, unsigned))
{
	fmll_try;

		fmll_svm * svm = NULL;
		mxml_node_t * content_node, * main_node = NULL;

		fmll_throw((xml_load(fname_prefix, TYPE_SVM, & main_node, & content_node)));
		fmll_throw_null((svm = fmll_svm_load_main(content_node, K)));

	fmll_catch;

		fmll_svm_destroy(svm);
		svm = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return svm;
}

double fmll_svm_run(fmll_svm * svm, const double * vec)
{
	unsigned u, dim = svm->dim, num = svm->num;
	double sum = - svm->b, * w = svm->w, ** s = svm->s;
	double (* K)(const double *, const double *, unsigned) = svm->K;

	for(u = 0; u < num; u++)
		sum += w[u] * (* K)(s[u], vec, dim);

	if(sum > 1)
		sum = 1;
	else if(sum < -1)
		sum = -1;

	return sum;
}

