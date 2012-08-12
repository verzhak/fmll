
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Функции инициализации весов синапсов нейронов сети прямого распространения

\endlang

*/

#ifndef FMLL_FF_WEIGHT_INIT_H
#define FMLL_FF_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/random/random.h"
	#include "ann/ff/ff.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов синапсов нейронов сети прямого распространения (псевдо) случайными значениями

	\param ff - указатель на описатель сети;
	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_ff_weight_init_random(fmll_ff * ff, fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

