
#include "ann/ff/weight_init.h"

int fmll_ff_weight_init_random(fmll_ff * ff, fmll_random * rnd)
{
	unsigned char ** connect = ff->connect;
	unsigned v, u, t, num = ff->num, * in = ff->in;
	double * b = ff->b, ** w = ff->w;

	for(v = 0, t = 0; v < num; v++)
	{
		if(in[t] == v) /* Входные нейроны игнорируются */
			t++;
		else
		{
			b[v] = fmll_random_generate(rnd);

			for(u = 0; u < num; u++)
				if(connect[u][v])
					w[u][v] = fmll_random_generate(rnd);
		}
	}

	return 0;
}

