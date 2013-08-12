
#include "ann/rbm/rbm.h"

void activate(fmll_rbm * rbm, const unsigned layer, const unsigned ind, const double T)
{
	/* 0 - слой видимых нейронов */
	/* 1 - слой скрытых нейронов */

	const unsigned dim = rbm->dim, hidden_num = rbm->hidden_num;
	const double ** w = (const double **) rbm->w;
	double sum = 0, * state = rbm->state, * hidden_state = rbm->hidden_state;
	unsigned v;

	if(layer)
	{
		for(v = 0; v < dim; v++)
			sum += w[v][ind] * state[v];
	}
	else
	{
		for(v = 0; v < hidden_num; v++)
			sum += w[ind][v] * hidden_state[v];
	}

	/* TODO threshold */

	sum = 1 / (1 + exp(- sum / T));
	sum = sum < 0.5 ? -1 : 1;

	if(layer)
		hidden_state[ind] = sum;
	else
		state[ind] = sum;
}

void activate_layer(fmll_rbm * rbm, const unsigned layer, const double T)
{
	const unsigned layer_size = layer ? rbm->hidden_num : rbm->dim;
	unsigned v;

	for(v = 0; v < layer_size; v++)
		activate(rbm, layer, v, T);
}

/* ############################################################################  */

fmll_rbm * fmll_rbm_init(const unsigned dim, const unsigned hidden_num, const double T_min)
{
	double ** w;
	unsigned u, v;
	fmll_rbm * rbm = NULL;

	fmll_try;

		fmll_throw_if(! dim || ! hidden_num);
		fmll_throw_if(T_min <= 0);
		fmll_throw_null(rbm = fmll_alloc(sizeof(fmll_rbm), 1, 1));

		rbm->w = NULL;
		rbm->state = NULL;
		rbm->hidden_state = NULL;
		rbm->dim = dim;
		rbm->hidden_num = hidden_num;
		rbm->T_min = T_min;

		fmll_throw_null(w = rbm->w = (double **) fmll_alloc(sizeof(double), 2, dim, hidden_num));
		fmll_throw_null(rbm->state = (double *) fmll_alloc(sizeof(double), 1, dim));
		fmll_throw_null(rbm->hidden_state = (double *) fmll_alloc(sizeof(double), 1, hidden_num));

		for(u = 0; u < dim; u++)
			for(v = 0; v < hidden_num; v++)
				w[u][v] = 0;

	fmll_catch;

		fmll_rbm_destroy(rbm);
		rbm = NULL;

	fmll_finally;

	return rbm;
}

void fmll_rbm_destroy(fmll_rbm * rbm)
{
	if(rbm != NULL)
	{
		fmll_free(rbm->w);
		fmll_free(rbm->state);
		fmll_free(rbm->hidden_state);
		fmll_free(rbm);
	}
}

int fmll_rbm_save(const fmll_rbm * rbm, const char * fname_prefix)
{
	/* TODO */

	return 0;

	/*
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
	*/
}

fmll_rbm * fmll_rbm_load(const char * fname_prefix)
{
	/* TODO */
	
	return NULL;

	/*
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
	*/
}

const double * fmll_rbm_run(fmll_rbm * rbm, const double * vec, const unsigned max_iter)
{
	const double T_min = rbm->T_min;
	unsigned iter;
	double * ret = rbm->state;

	fmll_try;

		fmll_throw_if(! max_iter);

		memcpy(rbm->state, vec, sizeof(double) * rbm->dim);

		for(iter = 0; iter < max_iter; iter++)
		{
			activate_layer(rbm, 1, T_min);
			activate_layer(rbm, 0, T_min);
		}

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

int fmll_rbm_CD_k(fmll_rbm * rbm, const double ** vec, const unsigned vec_num, const double T_max, const unsigned T_step_count, const unsigned max_iter, const double beta_0, double (* next_beta)(const double))
{
	int ret = 0;
	const unsigned dim = rbm->dim, hidden_num = rbm->hidden_num;
	const double T_min = rbm->T_min;
	unsigned v, iter, * ind = NULL;
	double T, T_step, beta, * state = rbm->state, ** delta_W = NULL;
	fmll_random * rnd = NULL;

	fmll_try;

	/* TODO Распараллеливание через OpenMP */

		fmll_throw_if(T_min >= T_max);
		fmll_throw_if(! T_step_count);
		fmll_throw_if(! max_iter);
		fmll_throw_null(ind = fmll_alloc(sizeof(unsigned), 1, vec_num));
		fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, NULL));
		fmll_throw_null(delta_W = fmll_alloc(sizeof(double), 2, dim, hidden_num));

		T_step = (T_max - T_min) / T_step_count;
	
		for(T = T_max; T >= T_min; T -= T_step)
		{
			for(iter = 0, beta = beta_0; iter < max_iter; iter++, beta = (* next_beta)(beta))
			{
				/* TODO randShuffle(ind); */

				/* TODO delta_W.setTo(0); */

				for(v = 0; v < vec_num; v++)
				{
					memcpy(state, vec[ind[v]], sizeof(double) * dim);

					activate_layer(rbm, 1, T);
					/* TODO delta_W += state[0].t() * state[1]; */

					activate_layer(rbm, 0, T);
					/* TODO delta_W -= state[0].t() * state[1]; */
				}

				/* TODO W += beta / T * (delta_W / sample_num); */
			}

			printf("---> %lf\n", T);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(ind);
		fmll_random_destroy(rnd);
		fmll_free(delta_W);

	return ret;
}

