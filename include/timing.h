
/*!

\file

\brief Расписание обучения нейронных сетей.

*/

#ifndef TIMING_H
#define TIMING_H

#include "all.h"

/*!

\brief Увеличение коэффициента скорости обучения на 0.1.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_0_1(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.01.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_0_01(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_0_001(double beta);

#endif

