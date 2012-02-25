
/*!

\file

\en

	\brief TODO

\ru

	\brief Функции инициализации весов нейронов нейронных сетей

\endlang

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/random/random.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param not_used - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети нулями

	\param not_used - не используется (может принимать любые значения - в том числе и NULL).

	\return 0

\endlang

*/
double fmll_weight_init_null(fmll_random * not_used);

/*!

\en

	\brief TODO

	\param not_used - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети значением 0.5

	\param not_used - не используется (может принимать любые значения - в том числе и NULL).

	\return 0.5

\endlang

*/
double fmll_weight_init_0_5(fmll_random * not_used);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 1)

	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return случайное число из диапазона [0, 1).

\endlang

*/
double fmll_weight_init_random_0_1(fmll_random * rnd);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.1)

	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return случайное число из диапазона [0, 0.1).

\endlang

*/
double fmll_weight_init_random_0_01(fmll_random * rnd);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.01)

	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return случайное число из диапазона [0, 0.01).

\endlang

*/
double fmll_weight_init_random_0_001(fmll_random * rnd);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [-0.1, 0.1)

	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return случайное число из диапазона [-0.1, 0.1).

\endlang

*/
double fmll_weight_init_random_m0_1_0_1(fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

