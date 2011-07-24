
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

