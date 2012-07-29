
#include "ann/mlp/activate.h"

double fmll_sin_a = 1;
double fmll_sin_b = 1;
double fmll_sigmoid_a = 1;
double fmll_tanh_a = 1;
double fmll_tanh_b = 1;
double fmll_gaussian_a = 1;
double fmll_gaussian_b = 0;
double fmll_gaussian_c = FMLL_SQRT_2;

double fmll_sin(double x)
{
	double tx = fmll_sin_b * x;

	if(fabs(tx) >= FMLL_PI_2)
		return fmll_sin_a * (tx > 0 ? 1 : -1);

	return fmll_sin_a * sin(tx);
}

double fmll_d_sin(double x)
{
	double tx = fmll_sin_b * x;

	if(fabs(tx) >= FMLL_PI_2)
		return 0;

	return fmll_sin_a * fmll_sin_b * cos(tx);
}

double fmll_sigmoid(double x)
{
	return 1 / (1 + exp(- fmll_sigmoid_a * x));
}

double fmll_d_sigmoid(double x)
{
	double f = fmll_sigmoid(x);

	return fmll_sigmoid_a * f * (1 - f);
}

double fmll_tanh(double x)
{
	return fmll_tanh_a * tanh(fmll_tanh_b * x);
}

double fmll_d_tanh(double x)
{
	double f = fmll_tanh(x);

	return (fmll_tanh_a * fmll_tanh_b - f * f / fmll_tanh_a);
}

double fmll_line(double x)
{
	return x;
}

double fmll_d_line(double x)
{
	return 1;
}

double fmll_gaussian(double x)
{
	double x_b = x - fmll_gaussian_b;

	return fmll_gaussian_a * exp(- x_b * x_b / (2 * fmll_gaussian_c * fmll_gaussian_c));
}

double fmll_d_gaussian(double x)
{
	double f = fmll_gaussian(x);

	return - (x - fmll_gaussian_b) * f / (fmll_gaussian_c * fmll_gaussian_c);
}

