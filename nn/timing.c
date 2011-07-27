
#include "timing.h"

#define SIMPLE_STEP(step) return beta + (step);

double fmll_timing_next_beta_step_0_1(double beta)
{
	SIMPLE_STEP(0.1);
}

double fmll_timing_next_beta_step_0_01(double beta)
{
	SIMPLE_STEP(0.01);
}

double fmll_timing_next_beta_step_0_001(double beta)
{
	SIMPLE_STEP(0.001);
}

double fmll_timing_next_beta_step_0_0001(double beta)
{
	SIMPLE_STEP(0.0001);
}

double fmll_timing_next_beta_step_0_00001(double beta)
{
	SIMPLE_STEP(0.00001);
}

