
#include "ann/som/som.h"

/* ############################################################################  */

double fmll_som_neighbor_wta(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index)
{
	if(index_winner == index)
		return 1;

	return 0;
}

double fmll_som_neighbor_radial(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index)
{
	double d = (* som->distance_w)(som->w[index_winner], som->w[index], som->map_dim);

	return gamma_mult * exp(- d * d / gamma_add);
}

/* ############################################################################  */

fmll_som * fmll_som_init(const unsigned * N, unsigned map_dim, unsigned dim, double (* weight_init)(fmll_random *), fmll_random * rnd,
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned))
{
	int v;
	unsigned u, num, * tN;
	double ** w, ** coord;
	fmll_som * som = NULL;

	fmll_try;

		fmll_throw_null((som = fmll_alloc(sizeof(fmll_som), 1, 1)));

		som->w = som->coord = NULL;
		som->N = NULL;

		for(u = 0, num = 1; u < map_dim; u++)
			num *= N[u];

		fmll_throw(! num);

		fmll_throw_null((w = som->w = (double **) fmll_alloc(sizeof(double), 2, num, dim)));
		fmll_throw_null((coord = som->coord = (double **) fmll_alloc(sizeof(double), 2, num, map_dim)));
		fmll_throw_null((tN = som->N = fmll_alloc(sizeof(unsigned), 1, map_dim)));

		som->num = num;
		som->map_dim = map_dim;
		som->dim = dim;
		som->distance_w = distance_w;
		som->distance = distance;

		memset(tN, 0, map_dim * sizeof(unsigned));

		for(u = 0; u < num; u++)
		{
			for(v = 0; v < dim; v++)
				w[u][v] = (* weight_init)(rnd);

			for(v = 0; v < map_dim; v++)
				coord[u][v] = tN[v];

			for(v = map_dim - 1; v >= 0; v--)
			{
				if(++ tN[v] < N[v])
					break;

				tN[v] = 0;
			}
		}

		memcpy(tN, N, map_dim * sizeof(unsigned));
		
	fmll_catch;

		fmll_som_destroy(som);
		som = NULL;

	fmll_finally;

	return som;
}

void fmll_som_destroy(fmll_som * som)
{
	if(som != NULL)
	{
		fmll_free(som->w);
		fmll_free(som->coord);
		fmll_free(som->N);
		fmll_free(som);
	}
}

int fmll_som_save(fmll_som * som, const char * fname_prefix)
{
	int ret = 0;
	char node_name[4096];
	unsigned u, v, num = som->num, map_dim = som->map_dim, dim = som->dim, * N = som->N;
	double ** w = som->w;
	mxml_node_t * sub_node, * node, * content_node, * main_node = NULL;
		
	fmll_try;

		fmll_throw((xml_create(TYPE_SOM, & main_node, & content_node)));
		fmll_throw((xml_set_int(content_node, "map_dim", map_dim)));
		fmll_throw((xml_set_int(content_node, "dim", dim)));

		fmll_throw_null((node = mxmlNewElement(content_node, "N")));

		for(u = 0; u < map_dim; u++)
		{
			sprintf(node_name, "N_%u", u);
			fmll_throw((xml_set_int(node, node_name, N[u])));
		}

		fmll_throw_null((node = mxmlNewElement(content_node, "W")));

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null((sub_node = mxmlNewElement(node, node_name)));

			for(v = 0; v < dim; v++)
			{
				sprintf(node_name, "%u", v);
				fmll_throw((xml_set_double(sub_node, node_name, w[u][v])));
			}
		}


		fmll_throw((xml_save(fname_prefix, main_node)));

	fmll_catch;

		ret = -1;

	fmll_finally;

		xml_destroy(main_node);

	return ret;
}

fmll_som * fmll_som_load(const char * fname_prefix, 
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned))
{
	char node_name[4096];
	int map_dim, dim;
	unsigned u, v, num, * N = NULL;
	double ** w;
	fmll_som * som = NULL;
	mxml_node_t * sub_sub_node, * sub_node, * node, * content_node, * main_node = NULL;

	fmll_try;

		fmll_throw((xml_load(fname_prefix, TYPE_SOM, & main_node, & content_node)));

		fmll_throw((xml_get_int(content_node, "map_dim", & map_dim)));
		fmll_throw((xml_get_int(content_node, "dim", & dim)));

		fmll_throw_null((N = fmll_alloc(sizeof(unsigned), 1, map_dim)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "N", NULL, NULL, MXML_DESCEND_FIRST)));

		for(u = 0, sub_node = mxmlFindElement(node, node, NULL, NULL, NULL, MXML_DESCEND_FIRST); u < map_dim; u++)
		{
			fmll_throw_null((sub_node));
			N[u] = sub_node->child->value.integer;
			sub_node = mxmlFindElement(sub_node, node, NULL, NULL, NULL, MXML_DESCEND);
		}

		fmll_throw_null((som = fmll_som_init(N, map_dim, dim, & fmll_weight_init_null, NULL, distance_w, distance)));
		fmll_throw_null((node = mxmlFindElement(content_node, content_node, "W", NULL, NULL, MXML_DESCEND_FIRST)));

		w = som->w;
		num = som->num;

		for(u = 0; u < num; u++)
		{
			sprintf(node_name, "w_%u", u);
			fmll_throw_null((sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST)));

			for(v = 0, sub_sub_node = mxmlFindElement(sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND_FIRST); v < dim; v++)
			{
				w[u][v] = sub_sub_node->child->value.real;
				sub_sub_node = mxmlFindElement(sub_sub_node, sub_node, NULL, NULL, NULL, MXML_DESCEND);
			}
		}


	fmll_catch;

		fmll_som_destroy(som);
		som = NULL;

	fmll_finally;

		fmll_free(N);
		xml_destroy(main_node);

	return som;
}

unsigned fmll_som_run(fmll_som * som, const double * vec)
{
	unsigned u, index_winner, num = som->num, dim = som->dim;
	double min, d, ** w = som->w;
	double (* distance)(const double *, const double *, unsigned) = som->distance;

	min = (* distance)(w[0], vec, dim);
	index_winner = 0;

	for(u = 1; u < num; u++)
	{
		d = (* distance)(w[u], vec, dim);

		if(d < min)
		{
			min = d;
			index_winner = u;
		}
	}

	return index_winner;
}

int fmll_som_so_kohonen(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned))
{
	int ret = 0;
	unsigned u, v, q, index_winner, num = som->num, dim = som->dim;
	double beta_gamma, beta = beta_0, ** w = som->w;

	fmll_try;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_mult < 0 || gamma_mult > 1));
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_add < 0));

		while(beta < 1.0000001)
		{
			fmll_print("Self - organization: beta == %.7lf\n", beta);

			for(u = 0; u < vec_num; u++)
			{
				index_winner = fmll_som_run(som, vec[u]);

				if(neighbor == & fmll_som_neighbor_wta)
					for(q = 0; q < dim; q++)
						w[index_winner][q] += beta * (vec[u][q] - w[index_winner][q]);
				else
					for(v = 0; v < num; v++)
					{
						beta_gamma = beta * neighbor(som, gamma_mult, gamma_add, index_winner, v);

						for(q = 0; q < dim; q++)
							w[v][q] += beta_gamma * (vec[u][q] - w[v][q]);
					}
			}

			beta = (* next_beta)(beta);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

int fmll_som_so_kohonen_penalty(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned), unsigned max_win, unsigned penalty)
{
	int ret = 0,* wn = NULL;
	unsigned u, v, q, index_winner, num = som->num, dim = som->dim;
	double min, d, beta_gamma, beta = beta_0, ** w = som->w;
	double (* distance)(const double *, const double *, unsigned) = som->distance;

	fmll_try;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_mult < 0 || gamma_mult > 1));
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_add < 0));
		fmll_throw_null((wn = fmll_alloc(sizeof(int), 1, num)));

		memset(wn, 0, num * sizeof(int));

		while(beta < 1.0000001)
		{
			fmll_print("Self - organization: beta == %.7lf\n", beta);

			for(u = 0, min = DBL_MAX; u < vec_num; u++)
			{
				for(v = 0; v < num; v++)
				{
					d = (* distance)(w[v], vec[u], dim);

					if(wn[v] < 0)
						wn[v]++;
					else if(d < min)
					{
						min = d;
						index_winner = v;
					}
				}

				if(++ wn[index_winner] == max_win)
					wn[index_winner] = - ((int) penalty);

				if(neighbor == & fmll_som_neighbor_wta)
					for(q = 0; q < dim; q++)
						w[index_winner][q] += beta * (vec[u][q] - w[index_winner][q]);
				else
					for(v = 0; v < num; v++)
					{
						beta_gamma = beta * neighbor(som, gamma_mult, gamma_add, index_winner, v);

						for(q = 0; q < dim; q++)
							w[v][q] += beta_gamma * (vec[u][q] - w[v][q]);
					}
			}

			beta = (* next_beta)(beta);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(wn);

	return ret;
}

/* ############################################################################  */

