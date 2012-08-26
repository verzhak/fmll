
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Функции инициализации весов синапсов нейронов

\endlang

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/random/random.h"
	#include "ann/ff/ff.h"
	#include "ann/mlp/mlp.h"
	#include "ann/som/som.h"
	#include "ann/pca/pca.h"

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

	\param som - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов синапсов нейронов нейронной карты (псевдо) случайными значениями

	\param som - указатель на описатель нейронной карты;
	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_som_weight_init_random(fmll_som * som, fmll_random * rnd);

/*!

\en

	\brief TRANSLATE

	\param pca - TRANSLATE;
	\param rnd - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Инициализация весов нейронов PCA-сети (псевдо) случайными значениями

	\param pca - указатель на описатель сети;
	\param rnd - описатель датчика (псевдо) случайных чисел.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_pca_weight_init_random(fmll_pca * pca, fmll_random * rnd);

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

