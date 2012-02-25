
#include "math/random/random.h"

/* ############################################################################ */

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL
#define MT19937_UPPER_MASK 0x80000000UL
#define MT19937_LOWER_MASK 0x7fffffffUL
#define MT19937_DEFAULT_SEED 5489UL

unsigned mt19937_mti = MT19937_N + 1;

int mt19937_init(fmll_random * rnd, unsigned long seed)
{
	int ret = 0;
	unsigned v;
	unsigned * state;

	fmll_try;

		fmll_throw_null(rnd->state = fmll_alloc(sizeof(unsigned), 1, MT19937_N + 1));
		state = (unsigned *) rnd->state;

		state[0] = seed & 0xffffffffUL;

		for(v = 1; v < MT19937_N; v++)
			state[v] = (1812433253UL * (state[v - 1] ^ (state[v - 1] >> 30)) + v);

		state[MT19937_N] = MT19937_N;
		
		/* state[v] &= 0xffffffffUL; \\TODO Зачем? */

	fmll_catch;

		fmll_free(rnd->state);
		rnd->state = NULL;

		ret = -1;

	fmll_finally;

	return ret;
}

unsigned mt19937_unsigned(fmll_random * rnd)
{
	unsigned v, value, * state;
	static unsigned mag01[2] = {0x0UL, MT19937_MATRIX_A};

	state = (unsigned *) rnd->state;

    if(state[MT19937_N] == MT19937_N)
	{
        for(v = 0; v < MT19937_N - MT19937_M; v++)
		{
            value = (state[v] & MT19937_UPPER_MASK) | (state[v + 1] & MT19937_LOWER_MASK);
            state[v] = state[v + MT19937_M] ^ (value >> 1) ^ mag01[value & 0x1UL];
        }

        for(; v < MT19937_N - 1; v++)
		{
			value = (state[v] & MT19937_UPPER_MASK) | (state[v + 1] & MT19937_LOWER_MASK);
			state[v] = state[v + (MT19937_M - MT19937_N)] ^ (value >> 1) ^ mag01[value & 0x1UL];
        }
		
		value = (state[MT19937_N - 1] & MT19937_UPPER_MASK) | (state[0] & MT19937_LOWER_MASK);
		state[MT19937_N - 1] = state[MT19937_M - 1] ^ (value >> 1) ^ mag01[value & 0x1UL];

		state[MT19937_N] = 0;
    }
	
	value = state[state[MT19937_N] ++];

    value ^= (value >> 11);
    value ^= (value << 7) & 0x9d2c5680UL;
    value ^= (value << 15) & 0xefc60000UL;
    value ^= (value >> 18);

    return value;
}

double mt19937_double_0_1(fmll_random * rnd)
{
	return mt19937_unsigned(rnd) * (1.0 / 4294967296.0);
}

/* ############################################################################ */

fmll_random * fmll_random_init(fmll_random_algorithm algo, unsigned long seed)
{
	fmll_random * rnd = NULL;

	fmll_try;

		fmll_throw_null(rnd = fmll_alloc(sizeof(fmll_random), 1, 1));

		rnd->algo = algo;
		rnd->state = NULL;

		switch(algo)
		{
			case FMLL_RANDOM_MT19937:
			{
				fmll_throw_if(mt19937_init(rnd, seed));

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		fmll_free(rnd);
		rnd = NULL;

	fmll_finally;

	return rnd;
}

fmll_random * fmll_random_init_default_seed(fmll_random_algorithm algo)
{
	fmll_random * rnd = NULL;

	fmll_try;

		switch(algo)
		{
			case FMLL_RANDOM_MT19937:
			{
				fmll_throw_null(rnd = fmll_random_init(algo, MT19937_DEFAULT_SEED));

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		fmll_free(rnd);
		rnd = NULL;

	fmll_finally;

	return rnd;
}

void fmll_random_destroy(fmll_random * rnd)
{
	if(rnd != NULL)
	{
		if(rnd->state != NULL)
			fmll_free(rnd->state);

		fmll_free(rnd);
	}
}

unsigned fmll_random_unsigned(fmll_random * rnd)
{
	unsigned value;

	fmll_try;

		switch(rnd->algo)
		{
			case FMLL_RANDOM_MT19937:
			{
				value = mt19937_unsigned(rnd);

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		value = 0;

	fmll_finally;
	
	return value;
}

double fmll_random_double_0_1(fmll_random * rnd)
{
	double value;

	fmll_try;

		switch(rnd->algo)
		{
			case FMLL_RANDOM_MT19937:
			{
				value = mt19937_double_0_1(rnd);

				break;
			}

			default:
				fmll_throw;
		};

	fmll_catch;

		value = 0;

	fmll_finally;
	
	return value;
}

