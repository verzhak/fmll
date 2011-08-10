
#include "distance.h"

double fmll_distance_square_euclid(const double * vec_1, const double * vec_2, unsigned dim)
{
	double d, sum = 0;
	unsigned u;

	for(u = 0; u < dim; u++)
	{
		d = vec_1[u] - vec_2[u];
		sum += d * d;
	}

	return sum;
}

double fmll_distance_euclid(const double * vec_1, const double * vec_2, unsigned dim)
{
	return sqrt(fmll_distance_square_euclid(vec_1, vec_2, dim));
}

double fmll_distance_scalar(const double * vec_1, const double * vec_2, unsigned dim)
{
	double sum = 0;
	unsigned u;

	for(u = 0; u < dim; u++)
		sum += vec_1[u] * vec_2[u];

	return sum / dim;
}

double fmll_distance_manhattan(const double * vec_1, const double * vec_2, unsigned dim)
{
	double sum = 0;
	unsigned u;

	for(u = 0; u < dim; u++)
		sum += fabs(vec_1[u] - vec_2[u]);

	return sum / dim;
}

double fmll_distance_chebyshev(const double * vec_1, const double * vec_2, unsigned dim)
{
	double d, max = 0;
	unsigned u;

	for(u = 0; u < dim; u++)
	{
		d = fabs(vec_1[u] - vec_2[u]);

		if(d > max)
			max = d;
	}

	return max;
}

