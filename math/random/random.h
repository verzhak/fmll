
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
	#include "math/various/constant.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

\ru

	\brief Датчики (псевдо) случайных чисел

\endlang

*/
typedef enum t_fmll_random_algorithm
{

	/*! \en TODO \ru Вихрь Мерсенна (алгоритм MT19937) \endlang */
	FMLL_RANDOM_ALGORITHM_MT19937,

	/*! \en TODO \ru Линейный конгруэнтный метод \endlang */
	FMLL_RANDOM_ALGORITHM_LCG

} fmll_random_algorithm;

/*!

\en

	\brief TODO

\ru

	\brief Распределения (псевдо) случайных чисел

\endlang

*/
typedef enum t_fmll_random_distribution
{

/*!
	
\en

	\brief TODO

	TODO

\ru

	\brief Равномерное распределение
	
	Параметры:
	
	- param[0] - нижняя граница (по умолчанию = 0);
	- param[1] - верхняя граница (по умолчанию = 1).

\endlang

*/
	FMLL_RANDOM_DISTRIBUTION_UNIFORM,

/*!
	
\en

	\brief TODO

	TODO

\ru

	\brief Нормальное распределение (распределение Гаусса)
	
	Параметры:
	
	- param[0] - математическое ожидание (\f$ \mu \f$; по умолчанию \f$ \mu = 0 \f$);
	- param[1] - среднеквадратичное отклонение (\f$ \sigma \f$; по умолчанию \f$ \sigma = 1 \f$).

	По умолчанию моделируется стандартное нормальное распределение.

\endlang

*/
	FMLL_RANDOM_DISTRIBUTION_NORMAL

} fmll_random_distribution;

/*!

\en

	\brief TODO

\ru

	\brief Описатель датчика (псевдо) случайных чисел

\endlang

*/
typedef struct t_fmll_random
{

	/*! \en TODO \ru Идентификатор датчика (псевдо) случайных чисел \endlang */
	fmll_random_algorithm algo;

	/*! \en TODO \ru Идентификатор распределения (псевдо) случайных чисел \endlang */
	fmll_random_distribution dist;

	/*! \cond HIDDEN_SYMBOLS */

	double * param, max;
	void * state;

	/*! \endcond */

} fmll_random;

/*!

\en

	\brief TODO

	\param algo - TODO;
	\param dist - TODO;
	\param param - TODO;
	\param seed - TODO.

	\return TODO;
	\return NULL - TODO.

	\sa fmll_random_distribution

\ru

	\brief Инициализация датчика (псевдо) случайных чисел

	\param algo - идентификатор датчика (псевдо) случайных чисел;
	\param dist - идентификатор распределения (псевдо) случайных чисел;
	\param param - параметры распределения (может принимать значение NULL - в этом случае, параметры распределения будут установлены в значение по умолчанию);
	\param seed - "зерно" (seed) датчика.

	\return указатель на описатель датчика (псевдо) случайных чисел в случае его успешной инициализации;
	\return NULL, если датчик (псевдо) случайных чисел инициализировать не удалось.

	\sa fmll_random_distribution

\endlang

*/
fmll_random * fmll_random_init(fmll_random_algorithm algo, fmll_random_distribution dist, double * param, unsigned long seed);

/*!

\en

	\brief TODO

	\param algo - TODO;
	\param dist - TODO;
	\param from - TODO;
	\param to - TODO.

	\return TODO;
	\return NULL - TODO.

	\sa fmll_random_distribution

\ru

	\brief Инициализация датчика (псевдо) случайных чисел "зерном" по умолчанию

	\param algo - идентификатор датчика (псевдо) случайных чисел;
	\param dist - идентификатор распределения (псевдо) случайных чисел;
	\param param - параметры распределения (может принимать значение NULL - в этом случае, параметры распределения будут установлены в значение по умолчанию).

	\return указатель на описатель датчика (псевдо) случайных чисел в случае его успешной инициализации;
	\return NULL, если датчик (псевдо) случайных чисел инициализировать не удалось.

	\sa fmll_random_distribution

\endlang

*/
fmll_random * fmll_random_init_default_seed(fmll_random_algorithm algo, fmll_random_distribution dist, double * param);

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

	\brief Генерация (псевдо) случайного вещественного числа двойной точности

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

	В случае, если в качестве параметра rnd передано значение NULL, функция вернет 0.

	\return сгенерированное число.

\endlang

*/
double fmll_random_generate(fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

