
#include <stdio.h>
#include <time.h>

#include <fmll.h>

int main()
{
#define NUM 100000

	int ret = 0;
	unsigned v;
	double m, d, param[2], value[NUM];
	fmll_random * rnd = NULL;

	fmll_try;

		m = 14;
		param[0] = - sqrt(3 / m);
		param[1] = sqrt(3 / m);

		/* fmll_throw_null(rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_NORMAL, -1, 1, 5, 20, 0, 0, 0, time(NULL))); */
		fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param));

		for(v = 0, m = 0; v < NUM; v++)
		{
			value[v] = fmll_random_generate(rnd);
			m += value[v];
		}

		m /= NUM;

		for(v = 0, d = 0; v < NUM; v++)
			d += (value[v] - m) * (value[v] - m);

		d /= NUM;

		printf("m = %f\nd = %f\n", m, d);

	fmll_catch;

		ret = -1;

	fmll_finally;

	fmll_random_destroy(rnd);

	return ret;
}

