
#include "ann/base/timing.h"

#define SIMPLE_STEP_MINUS(step) return beta - (step);
#define SIMPLE_STEP_PLUS(step) return beta + (step);

double fmll_timing_next_beta_step_minus_0_1(const double beta)
{
	SIMPLE_STEP_MINUS(0.1);
}

double fmll_timing_next_beta_step_minus_0_01(const double beta)
{
	SIMPLE_STEP_MINUS(0.01);
}

double fmll_timing_next_beta_step_minus_0_001(const double beta)
{
	SIMPLE_STEP_MINUS(0.001);
}

double fmll_timing_next_beta_step_minus_0_0001(const double beta)
{
	SIMPLE_STEP_MINUS(0.0001);
}

double fmll_timing_next_beta_step_minus_0_00001(const double beta)
{
	SIMPLE_STEP_MINUS(0.00001);
}

double fmll_timing_next_beta_step_0(const double beta)
{
	return beta;
}

double fmll_timing_next_beta_step_plus_0_1(const double beta)
{
	SIMPLE_STEP_PLUS(0.1);
}

double fmll_timing_next_beta_step_plus_0_01(const double beta)
{
	SIMPLE_STEP_PLUS(0.01);
}

double fmll_timing_next_beta_step_plus_0_001(const double beta)
{
	SIMPLE_STEP_PLUS(0.001);
}

double fmll_timing_next_beta_step_plus_0_0001(const double beta)
{
	SIMPLE_STEP_PLUS(0.0001);
}

double fmll_timing_next_beta_step_plus_0_00001(const double beta)
{
	SIMPLE_STEP_PLUS(0.00001);
}

