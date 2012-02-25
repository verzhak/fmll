
/*!

\file

\en

	\brief TODO

\ru

	\brief Генерация (псевдо) случайных чисел

\endlang

*/

#ifndef FMLL_RANDOM_H
#define FMLL_RANDOM_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/memory.h"
	#include "lib/exception.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

\ru

	\brief Перечисление реализованных алгоритмов генерации (псевдо) случайных чисел

\endlang

*/
typedef enum t_fmll_random_algorithm
{

	/*! \en TODO \ru Вихрь Мерсенна (алгоритм MT19937) \endlang */
	FMLL_RANDOM_MT19937

} fmll_random_algorithm;

/*!

\en

	\brief TODO

\ru

	\brief Описатель датчика (псевдо) случайных чисел

\endlang

*/
typedef struct t_fmll_random
{

	/*! \en TODO \ru Идентификатор алгоритма генерации (псевдо) случайных чисел \endlang */
	fmll_random_algorithm algo;
	
	/*! \cond HIDDEN_SYMBOLS */

	void * state;

	/*! \endcond */

} fmll_random;

/*!

\en

	\brief TODO

	\param algo - TODO;
	\param seed - TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Инициализация датчика (псевдо) случайных чисел

	\param algo - идентификатор алгоритма генерации (псевдо) случайных чисел;
	\param seed - "зерно" (seed) алгоритма.

	\return

		- указатель на описатель датчика (псевдо) случайных чисел в случае его успешной инициализации;
		- NULL, если датчик (псевдо) случайных чисел инициализировать не удалось.

\endlang

*/
fmll_random * fmll_random_init(fmll_random_algorithm algo, unsigned long seed);

/*!

\en

	\brief TODO

	\param algo - TODO;
	\param seed - TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Инициализация датчика (псевдо) случайных чисел "зерном" по умолчанию

	\param algo - идентификатор алгоритма генерации (псевдо) случайных чисел.

	\return

		- указатель на описатель датчика (псевдо) случайных чисел в случае его успешной инициализации;
		- NULL, если датчик (псевдо) случайных чисел инициализировать не удалось.

\endlang

*/
fmll_random * fmll_random_init_default_seed(fmll_random_algorithm algo);

/*!

\en

	\brief TODO

	\param rnd - TODO.

\ru

	\brief Удаление датчика (псевдо) случайных чисел

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

\endlang

*/
void fmll_random_destroy(fmll_random * rnd);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Генерация целого числа в диапазоне \f$[ ~ 0 ;~ 2^{32} ~ ) \f$

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

	\return сгенерированное число.

\endlang

*/
unsigned fmll_random_unsigned(fmll_random * rnd);

/*!

\en

	\brief TODO

	\param rnd - TODO.

	\return TODO.

\ru

	\brief Генерация вещественного числа двойной точности в диапазоне \f$[ ~ 0 ;~ 1 ~ ) \f$

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

	\return сгенерированное число.

\endlang

*/
double fmll_random_double_0_1(fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

