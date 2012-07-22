
#include "math/random/random.h"

/* Вихрь Мерсенна (алгоритм MT19937) */

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL
#define MT19937_UPPER_MASK 0x80000000UL
#define MT19937_LOWER_MASK 0x7fffffffUL

unsigned mt19937_mti = MT19937_N + 1;

int mt19937_init(fmll_random * rnd, unsigned long seed)
{
	int ret = 0;
	unsigned v;
	unsigned * state;

	fmll_try;

		rnd->max = 4294967296.0;

		fmll_throw_null(rnd->state = fmll_alloc(sizeof(unsigned), 1, MT19937_N + 1));
		state = (unsigned *) rnd->state;

		state[0] = seed & 0xffffffffUL;

		for(v = 1; v < MT19937_N; v++)
			state[v] = (1812433253UL * (state[v - 1] ^ (state[v - 1] >> 30)) + v);

		state[MT19937_N] = MT19937_N;
		
		/* state[v] &= 0xffffffffUL; TODO Зачем? */

	fmll_catch;

		fmll_free(rnd->state);
		rnd->state = NULL;

		ret = -1;

	fmll_finally;

	return ret;
}

double mt19937_generate(fmll_random * rnd)
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

