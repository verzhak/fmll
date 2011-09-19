
#include "kernel.h"

double fmll_kernel_radial_sigma = 1;
double fmll_kernel_tanh_a = 1;
double fmll_kernel_tanh_b = 0;

double fmll_kernel_polynomial_homogeneous_1(const double * v1, const double * v2, unsigned dim)
{
	unsigned u;
	double sum = 0;

	for(u = 0; u < dim; u++)
		sum += v1[u] * v2[u];

	return sum;
}

double fmll_kernel_polynomial_homogeneous_2(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_polynomial_homogeneous_1(v1, v2, dim);

	return s_1 * s_1;
}

double fmll_kernel_polynomial_homogeneous_3(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_polynomial_homogeneous_1(v1, v2, dim);

	return s_1 * s_1 * s_1;
}

double fmll_kernel_polynomial_homogeneous_4(const double * v1, const double * v2, unsigned dim)
{
	double s_2 = fmll_kernel_polynomial_homogeneous_2(v1, v2, dim);

	return s_2 * s_2;
}

double fmll_kernel_polynomial_inhomogeneous_1(const double * v1, const double * v2, unsigned dim)
{
	return fmll_kernel_polynomial_homogeneous_1(v1, v2, dim) + 1;
}

double fmll_kernel_polynomial_inhomogeneous_2(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_polynomial_homogeneous_1(v1, v2, dim) + 1;

	return s_1 * s_1;
}

double fmll_kernel_polynomial_inhomogeneous_3(const double * v1, const double * v2, unsigned dim)
{
	double s_1 = fmll_kernel_polynomial_homogeneous_1(v1, v2, dim) + 1;

	return s_1 * s_1 * s_1;
}

double fmll_kernel_polynomial_inhomogeneous_4(const double * v1, const double * v2, unsigned dim)
{
	double s_2 = fmll_kernel_polynomial_inhomogeneous_2(v1, v2, dim);

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

double fmll_kernel_intersection(const double * v1, const double * v2, unsigned dim)
{
	unsigned u;
	double sum;

	for(u = 0, sum = 0; u < dim; u++)
		sum += v1[u] > v2[u] ? v2[u] : v1[u];

	return sum;
}

