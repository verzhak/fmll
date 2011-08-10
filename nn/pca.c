
#include "pca.h"

fmll_pca * fmll_pca_init(unsigned dim, unsigned num, double (* weight_init)())
{
	fmll_try;

		double ** w;
		unsigned u, v;
		fmll_pca * pca = NULL;

		fmll_throw((! dim || ! num || dim < num));
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

int fmll_pca_save(fmll_pca * pca, const char * fname_prefix)
{
	fmll_try;

		int ret = 0;
		unsigned u, v, dim = pca->dim, num = pca->num;
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

		int dim, num;
		unsigned u, v;
		double ** w;
		fmll_pca * pca = NULL;
		mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;

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
	unsigned u, v, dim = pca->dim, num = pca->num;
	double sum, * y = pca->y, ** w = pca->w;

	for(u = 0; u < num; u++)
	{
		for(v = 0, sum = 0; v < dim; v++)
			sum += w[u][v] * vec[v];

		y[u] = sum;
	}

	return y;
}

int fmll_pca_so(fmll_pca * pca, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double), double max_d)
{
	fmll_try;

		int ret = 0;
		unsigned t, u, v, q, iter = 0, dim = pca->dim, num = pca->num;
		double d, sum, max = 0, beta = beta_0, * y = pca->y, ** w = pca->w, ** pw = NULL, ** tril_yy = NULL, ** tril_yy_w = NULL;

		fmll_throw(beta <= 0);
		fmll_throw(beta >= 1);
		fmll_throw_null((pw = (double **) fmll_alloc_2D(num, dim, sizeof(double))));
		fmll_throw_null((tril_yy = (double **) fmll_alloc_2D(num, num, sizeof(double))));
		fmll_throw_null((tril_yy_w = (double **) fmll_alloc_2D(num, dim, sizeof(double))));

		do
		{
			printf("Итерация = %u, beta = %.7lf, max = %.7lf\n", iter, beta, max);

			memcpy((void *) (pw + num), (void *) (w + num), num * dim * sizeof(double));

			for(t = 0; t < vec_num; t ++)
			{
				fmll_pca_run(pca, vec[t]);

				for(u = 0; u < num; u ++)
					for(v = 0; v < num; v ++)
						tril_yy[u][v] = u < v ? 0 : y[u] * y[v];

				for(u = 0; u < num; u ++)
					for(v = 0; v < dim; v ++)
					{
						for(q = 0, sum = 0; q < num; q ++)
							sum += tril_yy[u][q] * w[q][v];

						tril_yy_w[u][v] = sum;
					}

				for(u = 0; u < num; u ++)
					for(v = 0; v < dim; v ++)
						w[u][v] += beta * (y[u] * vec[t][v] - tril_yy_w[u][v]);
			}

			for(u = 0, max = 0; u < num; u ++)
				for(v = 0; v < dim; v ++)
					if(max < (d = fabs(w[u][v] - pw[u][v])))
						max = d;

			iter ++;
			beta = (* next_beta)(beta);
		}
		while(max > max_d && beta > 0);

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free_ND(pw);
		fmll_free_ND(tril_yy);
		fmll_free_ND(tril_yy_w);

	return ret;
}

