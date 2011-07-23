
#include "distance.h"

double fmll_distance_square_euclid(const double *v1, const double *v2, uint8_t dim)
{
	double d, sum = 0;
	uint16_t u;

	for(u = 0; u < dim; u++)
	{
		d = v1[u] - v2[u];
		sum += d * d;
	}

	return sum;
}

double fmll_distance_euclid(const double *v1, const double *v2, uint8_t dim)
{
	return sqrt(fmll_distance_square_euclid(v1, v2, dim));
}

double fmll_distance_scalar(const double *v1, const double *v2, uint8_t dim)
{
	double sum = 0;
	uint16_t u;

	for(u = 0; u < dim; u++)
		sum += v1[u] * v2[u];

	return sum / dim;
}

double fmll_distance_manhattan(const double *v1, const double *v2, uint8_t dim)
{
	double sum = 0;
	uint16_t u;

	for(u = 0; u < dim; u++)
		sum += fabs(v1[u] - v2[u]);

	return sum / dim;
}

double fmll_distance_chebyshev(const double *v1, const double *v2, uint8_t dim)
{
	double d, max = 0;
	uint16_t u;

	for(u = 0; u < dim; u++)
	{
		d = fabs(v1[u] - v2[u]);

		if(d > max)
			max = d;
	}

	return max;
}

