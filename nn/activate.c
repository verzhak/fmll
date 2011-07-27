
#include "activate.h"

double fmll_sin_a = 1;
double fmll_sin_b = 1;
double fmll_sigmoid_a = 1;
double fmll_atan_a = 1;
double fmll_atan_b = 1;

double fmll_sin(double x)
{
	return fmll_sin_a * sin(fmll_sin_b * x);
}

double fmll_d_sin(double x)
{
	return fmll_sin_a * fmll_sin_b * cos(fmll_sin_b * x);
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

double fmll_atan(double x)
{
	return fmll_atan_a * atan(fmll_atan_b * x);
}

double fmll_d_atan(double x)
{
	double f = fmll_atan(x);

	return fmll_atan_b * (fmll_atan_a - f) * (fmll_atan_a + f) / fmll_atan_a;
}

double fmll_line(double x)
{
	return x;
}

double fmll_d_line(double x)
{
	return 1;
}

