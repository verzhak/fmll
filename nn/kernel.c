
#include "kernel.h"

double fmll_kernel_radial_sigma = 1;
double fmll_kernel_tanh_a = 1;
double fmll_kernel_tanh_b = 0;

double fmll_kernel_scalar_1(const double * v1, const double * v2, unsigned dim)
{
	unsigned u;
	double sum = 0;

	for(u = 0; u < dim; u++)
		sum += v1[u] * v2[u];

	return sum + 1;
}

double fmll_kernel_scalar_2(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_scalar_1(v1, v2, dim);

	return s_1 * s_1;
}

double fmll_kernel_scalar_3(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_scalar_1(v1, v2, dim);

	return s_1 * s_1 * s_1;
}

double fmll_kernel_scalar_4(const double * v1, const double * v2, unsigned dim)
{
	double s_2 = fmll_kernel_scalar_2(v1, v2, dim);

	return s_2 * s_2;
}

double fmll_kernel_radial(const double * v1, const double * v2, unsigned dim)
{
	unsigned u;
	double d, sum = 0;

	for(u = 0; u < dim; u++)
	{
		d = v1[u] - v2[u];
		sum += d * d;
	}

	return exp(- sum / (2 * fmll_kernel_radial_sigma * fmll_kernel_radial_sigma));
}

double fmll_kernel_tanh(const double * v1, const double * v2, unsigned dim)
{
	unsigned u;
	double sum = 0;

	for(u = 0; u < dim; u++)
		sum += v1[u] * v2[u];

	return tanh(fmll_kernel_tanh_a * sum + fmll_kernel_tanh_b);
}

