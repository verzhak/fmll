
/*!

\file

\en

	\brief TODO

\ru

	\brief Функции инициализации весов синапсов нейронов перцетрона

\endlang

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/random/random.h"
	#include "ann/perceptron/perceptron.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param perc - TODO;
	\param rnd - TODO.

	\return TODO.

\ru

	\brief Установ весов синапсов нейронов перцептрона в 0

	\param perc - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел (не используется).

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_weight_init_0(fmll_perceptron * perc, fmll_random * rnd);

/*!

\en

	\brief TODO

	\param perc - TODO;
	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона (псевдо) случайными значениями

	\param perc - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_weight_init_random(fmll_perceptron * perc, fmll_random * rnd);

/*!

\en

	\brief TODO

	\param perc - TODO;
	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона по LeCun'у

	\param perc - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.
	
	Распределение (псевдо) случайных чисел, генерируемых датчиком, должно быть равномерным.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_weight_init_lecun(fmll_perceptron * perc, fmll_random * rnd);

/*!

\en

	\brief TODO

	\param perc - TODO;
	\param rnd - TODO.

	\return TODO.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона по алгоритму, предложенному Nguyen D. и Widrow B.

	\param perc - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.
	
	Распределение (псевдо) случайных чисел, генерируемых датчиком, должно быть равномерным.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_weight_init_nguyen_widrow(fmll_perceptron * perc, fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

