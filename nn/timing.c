
#include "timing.h"

double fmll_timing_next_beta_step_0_1(double beta)
{
	return beta + 0.1;
}

double fmll_timing_next_beta_step_0_01(double beta)
{
	return beta + 0.01;
}

double fmll_timing_next_beta_step_0_001(double beta)
{
	return beta + 0.001;
}
