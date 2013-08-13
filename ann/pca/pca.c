
#include "ann/pca/pca.h"

fmll_pca * fmll_pca_init(const unsigned dim, const unsigned num)
{
	double ** w;
	unsigned u, v;
	fmll_pca * pca = NULL;

	fmll_try;

		fmll_throw_if(! dim || ! num || dim < num);
		fmll_throw_null(pca = fmll_alloc(sizeof(fmll_pca), 1, 1));

		pca->w = NULL;
		pca->y = NULL;
		pca->dim = dim;
		pca->num = num;

		fmll_throw_null(w = pca->w = (double **) fmll_alloc(sizeof(double), 2, num, dim));
		fmll_throw_null(pca->y = (double *) fmll_alloc(sizeof(double), 1, num));

		fmll_throw_if(fmll_math_matrix_init_fill(w, 0, num, dim));

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
		fmll_free(pca->w);
		fmll_free(pca->y);
		fmll_free(pca);
	}
}

int fmll_pca_save(const fmll_pca * pca, const char * fname_prefix)
{
	int ret = 0;
	const unsigned dim = pca->dim, num = pca->num;
	const double ** w = (const double **) pca->w;
	unsigned u, v;
	char node_name[4096];
	mxml_node_t * sub_node, * node, * main_node = NULL, * content_node;
		
	fmll_try;

		fmll_throw_if(xml_create(TYPE_PCA, & main_node, & content_node));
		fmll_throw_if(xml_set_int(content_node, "dim", dim));
		fmll_throw_if(xml_set_int(content_node, "num", num));

		fmll_throw_null(node = mxmlNewElement(content_node, "W"));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null(sub_node = mxmlNewElement(node, node_name));

			for(v = 0; v < dim; v++)
			{
				sprintf(node_name, "%u", v);
				fmll_throw_if(xml_set_double(sub_node, node_name, w[u][v]));
			}
		}

		fmll_throw_if(xml_save(fname_prefix, main_node));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_pca * fmll_pca_load(const char * fname_prefix)
{
	int dim, num;
	char node_name[4096];
	unsigned u, v;
	double ** w;
	fmll_pca * pca = NULL;
	mxml_node_t * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_if(xml_load(fname_prefix, TYPE_PCA, & main_node, & content_node));

		fmll_throw_if(xml_get_int(content_node, "dim", & dim));
		fmll_throw_if(xml_get_int(content_node, "num", & num));

		fmll_throw_null(pca = fmll_pca_init(dim, num));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST));

		for(u = 0, w = pca->w; u < num; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

			for(v = 0, sub_sub_node = mxmlFindElement(sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); v < dim; v++)
			{
				w[u][v] = sub_sub_node->child->value.real;
				sub_sub_node = mxmlFindElement(sub_sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND);
			}
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
	const unsigned dim = pca->dim, num = pca->num;
	const double ** w = (const double **) pca->w;
	unsigned u, v;
	double sum, * y = pca->y;

	for(u = 0; u < num; u++)
	{
		for(v = 0, sum = 0; v < dim; v++)
			sum += w[u][v] * vec[v];

		y[u] = sum;
	}

	return y;
}

int fmll_pca_so(fmll_pca * pca, const double ** vec, const unsigned vec_num, const double beta_0, double (* next_beta)(const double), const double max_d, double * eigen)
{
	int ret = 0;
	const unsigned dim = pca->dim, num = pca->num;
	unsigned t, u, v, q, iter = 0;
	double d, sum, beta, max = 0, * y = pca->y, ** fcrow = NULL, ** w = pca->w, * pw = NULL, * yy = NULL, * yy_w = NULL;

	fmll_try;

		fmll_throw_if(beta_0 < 0 || beta_0 > 1);
		fmll_throw_null(pw = (double *) fmll_alloc(sizeof(double), 1, dim));
		fmll_throw_null(yy = (double *) fmll_alloc(sizeof(double), 1, num));
		fmll_throw_null(yy_w = (double *) fmll_alloc(sizeof(double), 1, dim));

		for(u = 0; u < num; u++)
		{
			beta = beta_0;

			do
			{
				fmll_print("Iteration = %u, component = %u, beta = %.7lf, max = %.7lf\n", iter, u + 1, beta, max);

				beta = (* next_beta)(beta);
				memcpy((void *) pw, (void *) w[u], dim * sizeof(double));

				for(t = 0; t < vec_num; t ++)
				{
					fmll_pca_run(pca, vec[t]);

					for(v = 0; v < dim; v ++)
					{
						for(q = 0, sum = 0; q <= u; q ++)
							sum += y[q] * w[q][v];

						yy_w[v] = sum;
					}

					for(v = 0; v < dim; v ++)
						w[u][v] += beta * y[u] * (vec[t][v] - yy_w[v]);
				}

				for(v = 0, max = 0; v < dim; v ++)
					if(max < (d = fabs(w[u][v] - pw[v])))
						max = d;

				iter ++;
			}
			while(max > max_d && beta > 0);
		}

		if(eigen != NULL)
		{
			fmll_throw_null(fcrow = (double **) fmll_alloc(sizeof(double), 2, dim, dim));

			for(v = 0; v < dim; v++)
				for(u = 0; u < dim; u++)
					fcrow[v][u] = 0;

			for(v = 0; v < vec_num; v++)
				for(t = 0; t < dim; t++)
				{
					fcrow[t][t] += vec[v][t] * vec[v][t];

					for(q = t + 1; q < dim; q++)
					{
						d = vec[v][t] * vec[v][q];
						fcrow[t][q] += d;
						fcrow[q][t] += d;
					}
				}

			for(v = 0; v < dim; v++)
				for(u = 0; u < dim; u++)
					fcrow[v][u] /= vec_num;

			for(t = 0; t < num; t++)
			{
				eigen[t] = 0;

				for(u = 0; u < dim; u++)
				{
					for(v = 0, sum = 0; v < dim; v++)
						sum += w[t][v] * fcrow[u][v];

					eigen[t] += sum * sum;
				}

				eigen[t] = sqrt(eigen[t]);
			}
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(fcrow);
		fmll_free(pw);
		fmll_free(yy);
		fmll_free(yy_w);

	return ret;
}

