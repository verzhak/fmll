
#include "various.h"

int8_t fmll_normalization(double ** vec, uint32_t vec_num, uint8_t dim)
{
	uint32_t u, v;
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

int8_t fmll_centering(double ** vec, uint32_t vec_num, uint8_t dim)
{
	fmll_try;

		int8_t ret = 0;
		uint32_t u, v;
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

