
/*!

\file

\en \brief TRANSLATE \ru \brief Расписание обучения нейронных сетей \endlang

*/

#ifndef FMLL_TIMING_H
#define FMLL_TIMING_H

#ifdef FMLL_BUILD

	#include "all.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Уменьшение коэффициента скорости обучения на 0.1

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_minus_0_1(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Уменьшение коэффициента скорости обучения на 0.01

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_minus_0_01(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Уменьшение коэффициента скорости обучения на 0.001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_minus_0_001(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Уменьшение коэффициента скорости обучения на 0.0001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_minus_0_0001(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Уменьшение коэффициента скорости обучения на 0.00001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_minus_0_00001(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Коэффициент скорости обучения остается неизменным

\param beta - текущее значение скорости обучения.

\return beta.

\endlang

*/
double fmll_timing_next_beta_step_0(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Увеличение коэффициента скорости обучения на 0.1

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_plus_0_1(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Увеличение коэффициента скорости обучения на 0.01

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_plus_0_01(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Увеличение коэффициента скорости обучения на 0.001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_plus_0_001(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Увеличение коэффициента скорости обучения на 0.0001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_plus_0_0001(const double beta);

/*!

\en

\brief TRANSLATE

\param beta - TRANSLATE.

\return TRANSLATE.

\ru

\brief Увеличение коэффициента скорости обучения на 0.00001

\param beta - текущее значение скорости обучения.

\return новое значение скорости обучения.

\endlang

*/
double fmll_timing_next_beta_step_plus_0_00001(const double beta);

#ifdef __cplusplus
}
#endif

#endif

