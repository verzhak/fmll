
/*!

\file

\en

	\brief TODO.

	TODO

\ru

	\brief Нейронная сеть, используемая для анализа главных компонент (PCA - Principal Component Analysis), обучаемая по принципу самоорганизации корреляционного типа.

	Последовательность действий по использованию PCA-сети:

		-# Центрировать (вычесть среднее) множество реализаций случайного вектора с помощью функции fmll_centering();
		-# Создать PCA-сеть с помощью функции fmll_pca_init();
		-# Обучить (самоорганизация) PCA-сеть с помощью функции fmll_pca_so();
		-# Прогнать PCA-сеть над целевыми векторами с помощью функции fmll_pca_run();
		-# Удалить PCA-сеть с помощью функции fmll_pca_destroy().

\endlang

*/

#ifndef FMLL_PCA_H
#define FMLL_PCA_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "nn/weight_init.h"

#endif

// ############################################################################ 

/*!

\en

	\brief TODO.

\ru

	\brief Описатель PCA-сети.

\endlang

*/
typedef struct t_fmll_pca
{

	/*! \en TODO. \ru Веса синапсов нейронов. \endlang */
	double ** w;

	/*! \en TODO. \ru Вектор выходных значений. \endlang */
	double * y;

	/*! \en TODO. \ru Размерность исходного векторного пространства. \endlang*/
	unsigned dim;

	/*!
	
	\en

		TODO.
		TODO: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\ru

		Количество нейронов (размерность целевого векторного пространства).
		Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\endlang

	*/
	unsigned num;

} fmll_pca;

// ############################################################################ 

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO.

	\param dim - TODO;
	\param num - TODO;
	\param weight_init - TODO.

	TODO: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Создание PCA-сети.

	\param dim - размерность исходного векторного пространства;
	\param num - количество нейронов в PCA-сети (размерность целевого векторного пространства);
	\param weight_init - указатель на функцию, инициализирующую веса синапсов нейронов.

	Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\return

		- указатель на описатель PCA-сети в случае ее успешного создания;
		- NULL - в случае неудачи.

\endlang

*/
fmll_pca * fmll_pca_init(unsigned dim, unsigned num, double (* weight_init)());

/*!

\en

	\brief TODO.

	\param pca - TODO.

\ru

	\brief Удаление PCA-сети.

	\param pca - указатель на описатель PCA-сети.

\endlang

*/
void fmll_pca_destroy(fmll_pca * pca);

/*!

\en

	\brief TODO.

	\param pca - TODO;
	\param fname_prefix - TODO (TODO).

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Сохранение в XML-файл описателя PCA-сети.

	\param pca - указатель на описатель PCA-сети;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return

		- 0 - в случае успешного сохранения описателя PCA-сети;
		- <> 0 - в случае некорректного завершения операции сохранения описателя PCA-сети.

\endlang

*/
int fmll_pca_save(fmll_pca * pca, const char * fname_prefix);

/*!

\en

	\brief TODO.

	\param fname_prefix - TODO (TODO).

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Загрузка из XML-файла описателя PCA-сети.

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return

		- указатель на описатель PCA-сети в случае его успешной загрузки;
		- NULL - в случае некорректного завершения операции загрузки описателя PCA-сети.

\endlang

*/
fmll_pca * fmll_pca_load(const char * fname_prefix);

/*!

\en

	\brief TODO.

	\param pca - TODO;
	\param vec - TODO.

	\return TODO.

\ru

	\brief Прогон PCA-сети над некоторым вектором.

	\param pca - указатель на описатель PCA-сети;
	\param vec - некоторый вектор.

	\return указатель на результирующий вектор.

\endlang

*/
const double * fmll_pca_run(fmll_pca * pca, const double * vec);

/*!

\en

	\brief TODO.

	\param pca - TODO;
	\param vec - TODO;
	\param vec_num - TODO;
	\param beta_0 - TODO, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TODO;
	\param max_d - TODO;
	\param eigen - TODO.

	<b>TODO.</b>

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Обучение (самоорганизация) PCA-сети.

	\param pca - указатель на описатель PCA-сети;
	\param vec - массив реализаций случайного вектора;
	\param vec_num - количество реализаций случайного вектора;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение скорости обучения в начале каждой итерации обучения по значению скорости обучения на предыдущей итерации;
	\param max_d - максимальное изменение весов нейронов PCA-сети на очередного итерации алгоритма самоорганизации, при котором процесс самоорганизации будет остановлен;
	\param eigen - указатель на массив размерости dim, в котором будут сохранены собственные числа матрицы ковариации случайного вектора, отсортированные в порядке убывания.

	<b>Реализации случайного вектора (массив vec) должны быть предварительно отцентрированы (из каждой реализации должно быть вычтено среднее по всем реализациям), что может быть выполнено с помощью функции fmll_centering().</b>

	Параметр eigen может принимать значение NULL - в этом случае расчет собственных чисел матрицы ковариации выполнен не будет. Размер массива eigen должен быть равен количеству нейронов в обучаемой PCA-сети.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_pca_so(fmll_pca * pca, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double), double max_d, double * eigen);

#ifdef __cplusplus
}
#endif

// ############################################################################ 

#endif

