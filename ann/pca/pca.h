
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief Нейронная сеть, используемая для анализа главных компонент (PCA - Principal Component Analysis), обучаемая по принципу самоорганизации корреляционного типа

	Последовательность действий по использованию PCA-сети:

	-# центрировать (вычесть среднее) множество реализаций случайного вектора с помощью функции fmll_centering();
	-# создать PCA-сеть с помощью функции fmll_pca_init();
	-# иницилизировать веса нейронов PCA-сети с помощью одной из *_pca_weight_init_* функций;
	-# обучить (самоорганизация) PCA-сеть с помощью функции fmll_pca_so();
	-# прогнать PCA-сеть над целевыми векторами с помощью функции fmll_pca_run();
	-# удалить PCA-сеть с помощью функции fmll_pca_destroy().

	\sa weight_init.h

\endlang

*/

#ifndef FMLL_PCA_H
#define FMLL_PCA_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/random/random.h"

#endif

/* ############################################################################ */

/*!

\en

	\brief TRANSLATE

\ru

	\brief Описатель PCA-сети

\endlang

*/
typedef struct t_fmll_pca
{

	/*! \en TRANSLATE \ru Веса синапсов нейронов \endlang */
	double ** w;

	/*! \en TRANSLATE \ru Вектор выходных значений \endlang */
	double * y;

	/*! \en TRANSLATE \ru Размерность исходного векторного пространства \endlang*/
	unsigned dim;

	/*!
	
	\en

		TRANSLATE

		TRANSLATE: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\ru

		Количество нейронов (размерность целевого векторного пространства)

		Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\endlang

	*/
	unsigned num;

} fmll_pca;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param dim - TRANSLATE;
	\param num - TRANSLATE.

	TRANSLATE: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Создание PCA-сети

	\param dim - размерность исходного векторного пространства;
	\param num - количество нейронов в PCA-сети (размерность целевого векторного пространства).

	Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

	\return указатель на описатель PCA-сети в случае ее успешного создания;
	\return NULL - в случае неудачи.

\endlang

*/
fmll_pca * fmll_pca_init(const unsigned dim, const unsigned num);

/*!

\en

	\brief TRANSLATE

	\param pca - TRANSLATE.

\ru

	\brief Удаление PCA-сети

	\param pca - указатель на описатель PCA-сети.

\endlang

*/
void fmll_pca_destroy(fmll_pca * pca);

/*!

\en

	\brief TRANSLATE

	\param pca - TRANSLATE;
	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Сохранение в XML-файл описателя PCA-сети

	\param pca - указатель на описатель PCA-сети;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя PCA-сети;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя PCA-сети.

\endlang

*/
int fmll_pca_save(const fmll_pca * pca, const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Загрузка из XML-файла описателя PCA-сети

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return указатель на описатель PCA-сети в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя PCA-сети.

\endlang

*/
fmll_pca * fmll_pca_load(const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param pca - TRANSLATE;
	\param vec - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Прогон PCA-сети над некоторым вектором

	\param pca - указатель на описатель PCA-сети;
	\param vec - некоторый вектор.

	\return указатель на результирующий вектор.

\endlang

*/
const double * fmll_pca_run(fmll_pca * pca, const double * vec);

/*!

\en

	\brief TRANSLATE

	\param pca - TRANSLATE;
	\param vec - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param beta_0 - TRANSLATE, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TRANSLATE;
	\param max_d - TRANSLATE;
	\param eigen - TRANSLATE.

	<b>TRANSLATE.</b>

	TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Обучение (самоорганизация) PCA-сети

	\param pca - указатель на описатель PCA-сети;
	\param vec - массив реализаций случайного вектора;
	\param vec_num - количество реализаций случайного вектора;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение скорости обучения в начале каждой итерации обучения по значению скорости обучения на предыдущей итерации;
	\param max_d - максимальное изменение весов нейронов PCA-сети на очередного итерации алгоритма самоорганизации, при котором процесс самоорганизации будет остановлен;
	\param eigen - указатель на массив размерости dim, в котором будут сохранены собственные числа матрицы ковариации случайного вектора, отсортированные в порядке убывания.

	<b>Реализации случайного вектора (массив vec) должны быть предварительно отцентрированы (из каждой реализации должно быть вычтено среднее по всем реализациям), что может быть выполнено с помощью функции fmll_centering().</b>

	Параметр eigen может принимать значение NULL - в этом случае расчет собственных чисел матрицы ковариации выполнен не будет. Размер массива eigen должен быть равен количеству нейронов в обучаемой PCA-сети.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_pca_so(fmll_pca * pca, const double ** vec, const unsigned vec_num, const double beta_0, double (* next_beta)(const double), const double max_d, double * eigen);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

