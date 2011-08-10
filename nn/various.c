
#include "various.h"

int fmll_normalization(double ** vec, unsigned vec_num, unsigned dim)
{
	unsigned u, v;
	double norm, * tvec;

	for(u = 0; u < vec_num; u++)
	{
		tvec = vec[u];

		for(v = 0, norm = 0; v < dim; v++)
			norm += tvec[v] * tvec[v];

		norm = sqrt(norm);

		for(v = 0; v < dim; v++)
			tvec[v] /= norm;
	}

	return 0;
}

int fmll_centering(double ** vec, unsigned vec_num, unsigned dim)
{
	fmll_try;

		int ret = 0;
		unsigned u, v;
		double * mean = NULL;
		
		fmll_throw_null((mean = fmll_alloc_1D(dim, sizeof(double))));

		for(v = 0; v < dim; v++)
			mean[v] = 0;

		for(u = 0; u < vec_num; u++)
			for(v = 0; v < dim; v++)
				mean[v] += vec[u][v];

		for(v = 0; v < dim; v++)
			mean[v] /= vec_num;

		for(u = 0; u < vec_num; u++)
			for(v = 0; v < dim; v++)
				vec[u][v] -= mean[v];

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free_ND(mean);

	return ret;
}

