
/*!

\file

\brief Расписание обучения нейронных сетей.

*/

#ifndef TIMING_H
#define TIMING_H

#include "all.h"

/*!

\brief Уменьшение коэффициента скорости обучения на 0.1.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_minus_0_1(double beta);

/*!

\brief Уменьшение коэффициента скорости обучения на 0.01.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_minus_0_01(double beta);

/*!

\brief Уменьшение коэффициента скорости обучения на 0.001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_minus_0_001(double beta);

/*!

\brief Уменьшение коэффициента скорости обучения на 0.0001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_minus_0_0001(double beta);

/*!

\brief Уменьшение коэффициента скорости обучения на 0.00001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_minus_0_00001(double beta);

/*!

\brief Коэффициент скорости обучения остается неизменным.

\param beta - текущее значение скорости обучения.

\return beta.

*/
double fmll_timing_next_beta_step_0(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.1.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_plus_0_1(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.01.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_plus_0_01(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_plus_0_001(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.0001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_plus_0_0001(double beta);

/*!

\brief Увеличение коэффициента скорости обучения на 0.00001.

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

*/
double fmll_timing_next_beta_step_plus_0_00001(double beta);

#endif

