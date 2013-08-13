
#include "ann/rbm/rbm.h"

void activate(fmll_rbm * rbm, const unsigned layer, const unsigned ind, const double T)
{
	/* 0 - слой видимых нейронов */
	/* 1 - слой скрытых нейронов */

	const unsigned input_num = layer ? rbm->dim : rbm->hidden_num;
	const double * input_state = (const double *) (layer ? rbm->state : rbm->hidden_state), ** w = (const double **) rbm->w;
	double sum = 0, * current_state = (layer ? rbm->hidden_state : rbm->state);
	unsigned v;

	for(v = 0; v < input_num; v++)
		sum += (layer ? w[v][ind] : w[ind][v]) * input_state[v];

	/* TODO threshold */

	sum = 1 / (1 + exp(- sum / T));
	current_state[ind] = sum < 0.5 ? -1 : 1;
}

void activate_layer(fmll_rbm * rbm, const unsigned layer, const double T)
{
	const unsigned current_num = layer ? rbm->hidden_num : rbm->dim;
	const unsigned neuro_per_thread = current_num / omp_get_max_threads();
	unsigned v, t_ind, from_v, to_v;

	#pragma omp parallel private(t_ind, v, from_v, to_v) default(shared)
	{
		t_ind = omp_get_thread_num();
		from_v = t_ind * neuro_per_thread;
		to_v = (t_ind + 1) * neuro_per_thread;
		
		if(to_v > current_num)
			to_v = current_num;

		for(v = from_v; v < to_v; v++)
			activate(rbm, layer, v, T);
	}
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
	int ret = 0;
	const unsigned dim = rbm->dim, hidden_num = rbm->hidden_num;
	const double ** w = (const double **) rbm->w;
	unsigned u, v;
	char node_name[4096];
	mxml_node_t * sub_node, * node, * main_node = NULL, * content_node;
		
	fmll_try;

		fmll_throw_if(xml_create(TYPE_RBM, & main_node, & content_node));
		fmll_throw_if(xml_set_int(content_node, "dim", dim));
		fmll_throw_if(xml_set_int(content_node, "hidden_num", hidden_num));
		fmll_throw_if(xml_set_double(content_node, "T_min", rbm->T_min));

		fmll_throw_null(node = mxmlNewElement(content_node, "W"));

		for(u = 0; u < dim; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null(sub_node = mxmlNewElement(node, node_name));

			for(v = 0; v < hidden_num; v++)
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

fmll_rbm * fmll_rbm_load(const char * fname_prefix)
{
	int dim, hidden_num;
	char node_name[4096];
	unsigned u, v;
	double T_min, ** w;
	fmll_rbm * rbm = NULL;
	mxml_node_t * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw_if(xml_load(fname_prefix, TYPE_RBM, & main_node, & content_node));

		fmll_throw_if(xml_get_int(content_node, "dim", & dim));
		fmll_throw_if(xml_get_int(content_node, "hidden_num", & hidden_num));
		fmll_throw_if(xml_get_double(content_node, "T_min", & T_min));

		fmll_throw_null(rbm = fmll_rbm_init(dim, hidden_num, T_min));
		fmll_throw_null(node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST));

		for(u = 0, w = rbm->w; u < dim; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

			for(v = 0, sub_sub_node = mxmlFindElement(sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); v < hidden_num; v++)
			{
				w[u][v] = sub_sub_node->child->value.real;
				sub_sub_node = mxmlFindElement(sub_sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND);
			}
		}

	fmll_catch;

		fmll_rbm_destroy(rbm);
		rbm = NULL;

	fmll_finally;

		xml_destroy(main_node);

	return rbm;
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
	double T, T_step, beta, * state = rbm->state, * hidden_state = rbm->hidden_state, ** w = rbm->w, ** t_w = NULL, ** delta_w = NULL;
	fmll_random * rnd = NULL;

	fmll_try;

		fmll_throw_if(T_min >= T_max || ! T_step_count || ! max_iter);
		fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, NULL));
		fmll_throw_null(ind = fmll_alloc(sizeof(unsigned), 1, vec_num));
		fmll_throw_null(t_w = fmll_alloc(sizeof(double), 2, dim, hidden_num));
		fmll_throw_null(delta_w = fmll_alloc(sizeof(double), 2, dim, hidden_num));

		T_step = (T_max - T_min) / T_step_count;

		for(v = 0; v < vec_num; v++)
			ind[v] = v;
	
		for(T = T_max; T >= T_min; T -= T_step)
		{
			for(iter = 0, beta = beta_0; iter < max_iter; iter++, beta = (* next_beta)(beta))
			{
				fmll_throw_if(fmll_random_shuffle(rnd, (char *) ind, vec_num, sizeof(unsigned)));
				fmll_throw_if(fmll_math_matrix_init_fill(delta_w, 0, dim, hidden_num));

				for(v = 0; v < vec_num; v++)
				{
					memcpy(state, vec[ind[v]], sizeof(double) * dim);

					#define ITER(layer, coef) \
						activate_layer(rbm, layer, T);\
						fmll_throw_if(fmll_math_vector_transpose_mult_vector(state, hidden_state, t_w, dim, hidden_num));\
						fmll_throw_if(fmll_math_matrix_sum(1, (const double **) delta_w, coef, (const double **) t_w, delta_w, dim, hidden_num));

					ITER(1, 1);
					ITER(0, -1);
				}

				fmll_throw_if(fmll_math_matrix_sum(1, (const double **) w, beta / (T * vec_num), (const double **) delta_w, w, dim, hidden_num));
			}

			fmll_print("Temperature = %lf (%lf -> %lf with step = %lf)\n", T, T_max, T_min, T_step);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_random_destroy(rnd);
		fmll_free(ind);
		fmll_free(t_w);
		fmll_free(delta_w);

	return ret;
}

