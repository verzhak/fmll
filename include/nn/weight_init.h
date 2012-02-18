
/*!

\file

\en

	\brief TODO.

\ru

	\brief Функции инициализации весов нейронов нейронных сетей.

\endlang

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети нулями.

	\return ноль.

\endlang

*/
double fmll_weight_init_null();

/*!

\en

	\brief TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети значением 0.5.

	\return ноль.

\endlang

*/
double fmll_weight_init_0_5();

/*!

\en

	\brief TODO.

	TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 1).

	Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

	\return случайное число из диапазона [0, 1).

\endlang

*/
double fmll_weight_init_random_0_1();

/*!

\en

	\brief TODO.

	TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.1).

	Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

	\return случайное число из диапазона [0, 0.1).

\endlang

*/
double fmll_weight_init_random_0_01();

/*!

\en

	\brief TODO.

	TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.01).

	Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

	\return случайное число из диапазона [0, 0.01).

\endlang

*/
double fmll_weight_init_random_0_001();

/*!

\en

	\brief TODO.

	TODO.

	\return TODO.

\ru

	\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [-0.1, 0.1).

	Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

	\return случайное число из диапазона [-0.1, 0.1).

\endlang

*/
double fmll_weight_init_random_m0_1_0_1();

#ifdef __cplusplus
}
#endif

#endif

