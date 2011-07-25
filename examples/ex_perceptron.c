
#include <stdio.h>
#include <time.h>

#include "memory.h"
#include "various.h"
#include "activate.h"
#include "weight_init.h"
#include "timing.h"
#include "perceptron.h"

int main()
{
	// TODO

	uint16_t N[4] = {2, 3, 4, 2};
	double (* fun[4])(double);
	double (* d_fun[4])(double);

	fun[0] = fun[1] = fun[2] = fun[3] = & fmll_sigmoid;
	d_fun[0] = d_fun[1] = d_fun[2] = d_fun[3] = & fmll_d_line;

	srand48(time(NULL));

	fmll_perceptron * perc = fmll_perceptron_init(3, 4, N, & fmll_weight_init_random_0_1, fun, d_fun);

	double m;
	uint32_t u, v, t, q, prev_num;

	for(u = t = m = 0; u < perc->layers_num; u++)
	{
		prev_num = u ? perc->N[u - 1] : perc->dim;

		for(v = 0; v < perc->N[u]; v++, t++)
			for(q = 0; q < prev_num; q++, m++)
				perc->w[t][q] = m;
	}

	double vec[3] = {1, 5, 7};
	double * y = fmll_perceptron_run(perc, vec);

	printf("[%lf, %lf]\n", y[0], y[1]);

	fmll_perceptron_destroy(perc);
	
	return 0;
}

