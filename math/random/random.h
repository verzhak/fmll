
/*!

\file

\en

	\brief (Pseudo) random number generation

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

	\brief (Pseudo) random number generator

\ru

	\brief Датчики (псевдо) случайных чисел

\endlang

*/
typedef enum t_fmll_random_algorithm
{

	/*! \en Mersenne twister (MT19937 algorithm) \ru Вихрь Мерсенна (алгоритм MT19937) \endlang */
	FMLL_RANDOM_ALGORITHM_MT19937,

	/*! \en Linear congruential algorithm \ru Линейный конгруэнтный метод \endlang */
	FMLL_RANDOM_ALGORITHM_LCG

} fmll_random_algorithm;

/*!

\en

	\brief (Pseudo) random number distribution

\ru

	\brief Распределения (псевдо) случайных чисел

\endlang

*/
typedef enum t_fmll_random_distribution
{

/*!
	
\en

	\brief Uniform distribution

	Parameters:

	- param[0] - lower border (default = 0);
	- param[1] - upper border (default = 1).

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

	\brief Normal (Gaussian) distribution

	Parameters:

	- param[0] - mean (\f$ \mu \f$; default \f$ \mu = 0 \f$);
	- param[1] - standard deviation (\f$ \sigma \f$; default \f$ \sigma = 1 \f$).

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

	\brief Descriptor for (pseudo) random number generator

\ru

	\brief Описатель датчика (псевдо) случайных чисел

\endlang

*/
typedef struct t_fmll_random
{

	/*! \en (Pseudo) random number generator id \ru Идентификатор датчика (псевдо) случайных чисел \endlang */
	fmll_random_algorithm algo;

	/*! \en (Pseudo) random number distribution id \ru Идентификатор распределения (псевдо) случайных чисел \endlang */
	fmll_random_distribution dist;

	/*! \cond HIDDEN_SYMBOLS */

	double * param, max;
	void * state;

	/*! \endcond */

} fmll_random;

/*!

\en

	\brief (Pseudo) random number generator initialization

	\param algo - (Pseudo) random number generator id;
	\param dist - (Pseudo) random number distribution id;
	\param param - distribution parameters (may be NULL - in this case, distribution parameters are set in default values);
	\param seed - generator seed.

	\return pointer to generator descriptor on success;
	\return NULL, if initialization failed.

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

	\brief (Pseudo) random number generator initialization with default seed

	\param algo - (Pseudo) random number generator id;
	\param dist - (Pseudo) random number distribution id;
	\param param - distribution parameters (may be NULL - in this case, distribution parameters are set in default values).

	\return pointer to generator descriptor on success;
	\return NULL, if initialization failed.

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

	\brief Generator descriptor destroy

	\param rnd - pointer to generator descriptor.

\ru

	\brief Удаление датчика (псевдо) случайных чисел

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

\endlang

*/
void fmll_random_destroy(fmll_random * rnd);

/*!

\en

	\brief Generate double value

	\param rnd - pointer to generator descriptor.

	\return resulting value.

\ru

	\brief Генерация (псевдо) случайного вещественного числа двойной точности

	\param rnd - указатель на описатель датчика (псевдо) случайных чисел.

	\return сгенерированное число.

\endlang

*/
double fmll_random_generate(fmll_random * rnd);

#ifdef __cplusplus
}
#endif

#endif

