
#include <stdio.h>
#include <time.h>

#include <fmll.h>

int main()
{
#define NUM 100000

	int ret = 0;
	unsigned v;
	double m, d, value[NUM];
	fmll_random * rnd = NULL;

	fmll_try;

		fmll_throw_null(rnd = fmll_random_init(FMLL_RANDOM_LCG, time(NULL)));

		for(v = 0, m = 0; v < NUM; v++)
		{
			value[v] = fmll_random_double_0_1(rnd);
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
