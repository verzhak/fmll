
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

double fmll_som_neighbor_wta(fmll_som * som, double beta, double gamma, uint8_t index_winner, uint8_t index)
{
	if(index_winner == index)
		return gamma;

	return 0;
}

double fmll_som_neighbor_radial(fmll_som * som, double beta, double gamma, uint8_t index_winner, uint8_t index)
{
	return gamma * exp(- (* som->distance_w)(som->w[index_winner], som->w[index], som->map_dim));
}

// ############################################################################ 

fmll_som * fmll_som_init(const uint8_t * N, uint8_t map_dim, uint8_t dim,
		double (*weight_init)(), double (*distance_w)(const double *, const double *, uint8_t), double (*distance)(const double *, const double *, uint8_t))
{
	fmll_try;

		double ** w = NULL;
		uint8_t ** coord = NULL;
		uint8_t * tN = NULL;

		fmll_som * som = alloc_1D(1, sizeof(fmll_som));

		fmll_throw_null(som);

		uint16_t u;
		int32_t v;
		uint32_t num;

		for(u = 0, num = 1; u < map_dim; u++)
			num *= N[u];

		w = som->w = (double **) alloc_2D(num, dim, sizeof(double));
		coord = som->coord = (uint8_t **) alloc_2D(num, map_dim, sizeof(uint8_t)); // Не помню, гарантируется ли sizeof(uint8_t) == 1 в C99
		tN = som->N = alloc_1D(map_dim, sizeof(uint8_t));

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

		free_ND(som);
		free_ND(w);
		free_ND(coord);
		free_ND(tN);

		som = NULL;

	fmll_finally;

	return som;
}

void fmll_som_destroy(fmll_som * som)
{
	if(som != NULL)
	{
		free_ND(som->w);
		free_ND(som->coord);
		free_ND(som->N);
		free_ND(som);
	}
}

int16_t fmll_som_run(fmll_som * som, const double * v)
{
	// TODO
	return 0;
}

int8_t fmll_som_teach(fmll_som * som, const double ** v, uint32_t v_num, double beta_0, double (*next_beta)(double),
		double gamma, double (*neighbor)(fmll_som *, double, double, uint8_t, uint8_t))
{
	// TODO
	return 0;
}

// ############################################################################ 

