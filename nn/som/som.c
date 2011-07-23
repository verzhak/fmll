
#include "som.h"

// ############################################################################ 

double fmll_som_weight_init_null()
{
	return 0;
}

double fmll_som_weight_init_random_0_1()
{
	return drand48();
}

// ############################################################################ 

double fmll_som_next_beta_step_0_1(double beta)
{
	return beta + 0.1;
}

double fmll_som_next_beta_step_0_01(double beta)
{
	return beta + 0.01;
}

double fmll_som_next_beta_step_0_001(double beta)
{
	return beta + 0.001;
}

// ############################################################################ 

double fmll_som_neighbor_wta(fmll_som * som, double gamma_mult, double gamma_add, uint32_t index_winner, uint32_t index)
{
	if(index_winner == index)
		return 1;

	return 0;
}

double fmll_som_neighbor_radial(fmll_som * som, double gamma_mult, double gamma_add, uint32_t index_winner, uint32_t index)
{
	double d = (* som->distance_w)(som->w[index_winner], som->w[index], som->map_dim);

	return gamma_mult * exp(- d * d / gamma_add);
}

// ############################################################################ 

fmll_som * fmll_som_init(const uint8_t * N, uint8_t map_dim, uint8_t dim,
		double (* weight_init)(), double (* distance_w)(const double *, const double *, uint8_t), double (* distance)(const double *, const double *, uint8_t))
{
	fmll_try;

		double ** w = NULL;
		double ** coord = NULL;
		uint8_t * tN = NULL;

		fmll_som * som = fmll_alloc_1D(1, sizeof(fmll_som));

		fmll_throw_null(som);

		uint16_t u;
		int32_t v;
		uint32_t num;

		for(u = 0, num = 1; u < map_dim; u++)
			num *= N[u];

		w = som->w = (double **) fmll_alloc_2D(num, dim, sizeof(double));
		coord = som->coord = (double **) fmll_alloc_2D(num, map_dim, sizeof(double));
		tN = som->N = fmll_alloc_1D(map_dim, sizeof(uint8_t));

		fmll_throw_null(w);
		fmll_throw_null(coord);
		fmll_throw_null(tN);

		som->num = num;
		som->map_dim = map_dim;
		som->dim = dim;
		som->distance_w = distance_w;
		som->distance = distance;

		memset(tN, 0, map_dim * sizeof(uint8_t));

		for(u = 0; u < num; u++)
		{
			for(v = 0; v < dim; v++)
				w[u][v] = (* weight_init)();

			for(v = 0; v < map_dim; v++)
				coord[u][v] = tN[v];

			for(v = map_dim - 1; v >= 0; v--)
			{
				if(++ tN[v] < N[v])
					break;

				tN[v] = 0;
			}
		}

		memcpy(tN, N, map_dim * sizeof(uint8_t));
		
	fmll_catch;

		fmll_free_ND(som);
		fmll_free_ND(w);
		fmll_free_ND(coord);
		fmll_free_ND(tN);

		som = NULL;

	fmll_finally;

	return som;
}

void fmll_som_destroy(fmll_som * som)
{
	if(som != NULL)
	{
		fmll_free_ND(som->w);
		fmll_free_ND(som->coord);
		fmll_free_ND(som->N);
		fmll_free_ND(som);
	}
}

uint32_t fmll_som_run(fmll_som * som, const double * vec)
{
	uint8_t dim = som->dim;
	uint32_t u, index_winner, num = som->num;
	double min, d, ** w = som->w;
	double (* distance_w)(const double *, const double *, uint8_t) = som->distance_w;

	min = (* distance_w)(w[0], vec, dim);
	index_winner = 0;

	for(u = 1; u < num; u++)
	{
		d = (* distance_w)(w[u], vec, dim);

		if(d < min)
		{
			min = d;
			index_winner = u;
		}
	}

	return index_winner;
}

int8_t fmll_som_so_kohonen(fmll_som * som, double ** vec, uint32_t vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, uint32_t, uint32_t))
{
	fmll_try;

		int8_t ret = 0;

		fmll_throw(beta_0 < 0 || beta_0 > 1);
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_mult < 0 || gamma_mult > 1));
		fmll_throw(neighbor == & fmll_som_neighbor_radial && (gamma_add < 0));

		uint8_t dim = som->dim;
		uint32_t u, v, q, index_winner, num = som->num;
		double beta_gamma, beta = beta_0, ** w = som->w;;

		while(beta < 1.0000001)
		{
			printf("Самоорганизация нейронной карты: beta == %.7lf\n", beta);

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

// ############################################################################ 

