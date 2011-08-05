
#include "pca.h"

fmll_pca * fmll_pca_init(uint8_t dim, uint8_t num, double (* weight_init)())
{
	fmll_try;

		fmll_pca * pca = NULL;
		double ** w;
		uint32_t u, v;

		fmll_throw_null((pca = fmll_alloc_1D(1, sizeof(fmll_pca))));

		pca->w = NULL;
		pca->y = NULL;

		fmll_throw_null((w = pca->w = (double **) fmll_alloc_2D(num, dim, sizeof(double))));
		fmll_throw_null((pca->y = (double *) fmll_alloc_1D(num, sizeof(double))));

		pca->dim = dim;
		pca->num = num;

		for(u = 0; u < num; u++)
			for(v = 0; v < dim; v++)
				w[u][v] = (* weight_init)();

	fmll_catch;

		fmll_pca_destroy(pca);
		pca = NULL;

	fmll_finally;

	return pca;
}

void fmll_pca_destroy(fmll_pca * pca)
{
	if(pca != NULL)
	{
		fmll_free_ND(pca->w);
		fmll_free_ND(pca->y);
		fmll_free_ND(pca);
	}
}

int8_t fmll_pca_save(fmll_pca * pca, const char * fname_prefix)
{
	fmll_try;

		int8_t ret = 0;
		uint8_t dim = pca->dim, num = pca->num;
		uint32_t u, v;
		char node_name[4096];
		double ** w = pca->w;
		mxml_node_t * node, * main_node = NULL, * content_node;
		
		fmll_throw((xml_create(TYPE_PCA, & main_node, & content_node)));
		fmll_throw((xml_set_int(content_node, "dim", dim)));
		fmll_throw((xml_set_int(content_node, "num", num)));

		fmll_throw_null((node = mxmlNewElement(content_node, "W")));

		for(u = 0; u < num; u++)
			for(v = 0; v < dim; v++)
			{
				sprintf(node_name, "w_%u_%u", u, v);
				fmll_throw((xml_set_double(node, node_name, w[u][v])));
			}

		fmll_throw(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_pca * fmll_pca_load(const char * fname_prefix)
{
	fmll_try;

		fmll_pca * pca = NULL;
		mxml_node_t * sub_node, * node, * main_node = NULL, * content_node;
		int dim, num;
		uint32_t u, v;
		double ** w;

		fmll_throw((xml_load(fname_prefix, TYPE_PCA, & main_node, & content_node)));

		fmll_throw((xml_get_int(content_node, "dim", & dim)));
		fmll_throw((xml_get_int(content_node, "num", & num)));

		fmll_throw_null((pca = fmll_pca_init(dim, num, & fmll_weight_init_null)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST)));

		w = pca->w;

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < num; u++)
			for(v = 0; v < dim; v++)
			{
				w[u][v] = sub_node->child->value.real;
				sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
			}

	fmll_catch;

		fmll_pca_destroy(pca);
		pca = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return pca;
}

const double * fmll_pca_run(fmll_pca * pca, const double * vec)
{
	double sum, * y = pca->y, ** w = pca->w;
	uint8_t dim = pca->dim, num = pca->num;
	uint32_t u, v;

	for(u = 0; u < num; u++)
	{
		for(v = 0, sum = 0; v < dim; v++)
			sum += w[u][v] * vec[v];

		y[u] = sum;
	}

	return y;
}

int8_t fmll_pca_so(fmll_pca * pca, double ** vec, uint32_t vec_num, double max_d)
{
	fmll_try;

		int8_t ret = 0;
		uint8_t dim = pca->dim, num = pca->num;
		double ** w = pca->w;

		// TODO

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

