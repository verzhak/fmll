
#include "math/random/random.h"

int mt19937_init(fmll_random * rnd, const unsigned long seed);
double mt19937_generate(fmll_random * rnd);

int lcg_init(fmll_random * rnd, const unsigned long seed);
double lcg_generate(fmll_random * rnd);

/* ############################################################################ */

fmll_random * fmll_random_init(const fmll_random_algorithm algo, const fmll_random_distribution dist, const double * param, const unsigned long seed)
{
	fmll_random * rnd = NULL;

	fmll_try;

		fmll_throw_null(rnd = fmll_alloc(sizeof(fmll_random), 1, 1));

		rnd->algo = algo;
		rnd->dist = dist;
		rnd->param = NULL;
		rnd->state = NULL;

		switch(algo)
		{
			case FMLL_RANDOM_ALGORITHM_MT19937:
			{
				fmll_throw_if(mt19937_init(rnd, seed));

				break;
			}

			case FMLL_RANDOM_ALGORITHM_LCG:
			{
				fmll_throw_if(lcg_init(rnd, seed));

				break;
			}

			default:
				fmll_throw;
		};

		switch(dist)
		{
			case FMLL_RANDOM_DISTRIBUTION_UNIFORM:
			{
				fmll_throw_null(rnd->param = fmll_alloc(sizeof(double), 1, 2));

				if(param == NULL)
				{
					rnd->param[0] = 0;
					rnd->param[1] = 1;
				}
				else
					memcpy(rnd->param, param, sizeof(double) * 2);

				break;
			}

			case FMLL_RANDOM_DISTRIBUTION_NORMAL:
			{
				fmll_throw_null(rnd->param = fmll_alloc(sizeof(double), 1, 2));

				if(param == NULL)
				{
					rnd->param[0] = 0;
					rnd->param[1] = 1;
				}
				else
					memcpy(rnd->param, param, sizeof(double) * 2);

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		fmll_random_destroy(rnd);
		rnd = NULL;

	fmll_finally;

	return rnd;
}

fmll_random * fmll_random_init_default_seed(const fmll_random_algorithm algo, const fmll_random_distribution dist, const double * param)
{
	fmll_random * rnd = NULL;
	unsigned long seed;

	fmll_try;

		switch(algo)
		{
			case FMLL_RANDOM_ALGORITHM_MT19937:
			{
				seed = 5489UL;

				break;
			}
			
			case FMLL_RANDOM_ALGORITHM_LCG:
			{
				seed = time(NULL);

				break;
			}

			default:
				fmll_throw;
		};

		fmll_throw_null(rnd = fmll_random_init(algo, dist, param, seed));

	fmll_catch;

		fmll_random_destroy(rnd);
		rnd = NULL;

	fmll_finally;

	return rnd;
}

void fmll_random_destroy(fmll_random * rnd)
{
	if(rnd != NULL)
	{
		if(rnd->param != NULL)
			fmll_free(rnd->param);

		if(rnd->state != NULL)
			fmll_free(rnd->state);

		fmll_free(rnd);
	}
}

double fmll_random_generate(fmll_random * rnd)
{
	double value;
	double (* generate)(fmll_random *);

	fmll_try;

		switch(rnd->algo)
		{
			case FMLL_RANDOM_ALGORITHM_MT19937:
			{
				generate = & mt19937_generate;

				break;
			}

			case FMLL_RANDOM_ALGORITHM_LCG:
			{
				generate = & lcg_generate;

				break;
			}

			default:
				fmll_throw;
		};

		switch(rnd->dist)
		{
			case FMLL_RANDOM_DISTRIBUTION_UNIFORM:
			{
				value = (rnd->param[1] - rnd->param[0]) * (* generate)(rnd) / rnd->max + rnd->param[0];

				break;
			}

			case FMLL_RANDOM_DISTRIBUTION_NORMAL:
			{
				/* Преобразование Бокса - Мюллера
				 *
				 * Для упрощения реализации вместо двух чисел от независимых датчиков используются две реализации одного и того же датчика
				 */

				double phi = (* generate)(rnd) / rnd->max, r = (* generate)(rnd) / rnd->max;

				value = cos(FMLL_2_PI * phi) * sqrt(- 2 * log(r));
				value = rnd->param[1] * value + rnd->param[0];

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		value = -1;

	fmll_finally;
	
	return value;
}

int fmll_random_shuffle(fmll_random * rnd, char * sequence, const unsigned elem_num, const unsigned elem_size)
{
	int ret = 0;
	long v;
	unsigned ind;
	char * temp = NULL;

	fmll_try;

		fmll_throw_null(temp = fmll_alloc_a(elem_size));

		for(v = elem_num - 1; v > 0; v--)
		{
			ind = fmll_random_generate(rnd) * v;

			memcpy(temp, sequence + ind * elem_size, elem_size);
			memcpy(sequence + ind * elem_size, sequence + v * elem_size, elem_size);
			memcpy(sequence + v * elem_size, temp, elem_size);
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

