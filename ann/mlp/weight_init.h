
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Функции инициализации весов синапсов нейронов перцетрона

\endlang

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/random/random.h"
	#include "ann/mlp/mlp.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона (псевдо) случайными значениями

	\param mlp - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_weight_init_random(fmll_mlp * mlp, fmll_random * rnd);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона по алгоритму, предложенному LeCun Y.

	\param mlp - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.
	
	Распределение (псевдо) случайных чисел, генерируемых датчиком, должно быть равномерным.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_weight_init_lecun(fmll_mlp * mlp, fmll_random * rnd);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов синапсов нейронов перцептрона по алгоритму, предложенному Nguyen D. и Widrow B.

	\param mlp - указатель на описатель перцептрона;
	\param rnd - описатель датчика (псевдо) случайных чисел.
	
	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_weight_init_nguyen_widrow(fmll_mlp * mlp, fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

