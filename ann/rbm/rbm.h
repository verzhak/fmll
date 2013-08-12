
/*!

\file

\en

\brief TRANSLATE

TRANSLATE

\ru

\brief Ограниченная машина Больцмана (RBM - Restricted Bolzmann machine)

Алгоритм работы с ограниченной машиной Больцмана:

-# создать ограниченную машину Больцмана с помощью функции fmll_rbm_init();
-# иницилизировать веса нейронов ограниченной машины Больцмана с помощью одной из *_rbm_weight_init_* функций;
-# обучить ограниченную машину Больцмана с помощью функции fmll_rbm_CD_k();
-# прогнать ограниченную машину Больцмана над целевыми векторами с помощью функции fmll_rbm_run();
-# удалить ограниченную машину Больцмана с помощью функции fmll_rbm_destroy().

\endlang

\sa weight_init.h

*/

#ifndef FMLL_RBM_H
#define FMLL_RBM_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/random/random.h"

#endif

/* ############################################################################ */

/*! \en \brief TRANSLATE \ru \brief Описатель ограниченной машины Больцмана \endlang */
typedef struct t_fmll_rbm
{

	/*! \en TRANSLATE \ru Веса синапсов нейронов \endlang */
	double ** w;

	/*! \en TRANSLATE \ru Вектор состояния видимых нейронов \endlang */
	double * state;

	/*! \en TRANSLATE \ru Размерность исходного векторного пространства \endlang*/
	unsigned dim;

	/*! \en TRANSLATE \ru Минимальная температура, при которой будет работать ограниченная машина Больцмана \endlang*/
	double T_min;

	/*! \cond HIDDEN_SYMBOLS */

	double * hidden_state;
	unsigned hidden_num;

	/*! \endcond */

} fmll_rbm;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param dim - TRANSLATE;
\param hidden_num - TRANSLATE;
\param T_min - TODO \f$T_{min} > 0\f$.

\return TRANSLATE;
\return NULL - TRANSLATE.

\ru

\brief Создание ограниченной машины Больцмана

\param dim - размерность исходного векторного пространства;
\param hidden_num - количество скрытых нейронов;
\param T_min - минимальная температура, при которой будет работать ограниченная машина Больцмана \f$T_{min} > 0\f$.

\return указатель на описатель ограниченной машины Больцмана в случае ее успешного создания;
\return NULL - в случае неудачи.

\endlang

*/
fmll_rbm * fmll_rbm_init(const unsigned dim, const unsigned hidden_num, const double T_min);

/*!

\en

\brief TRANSLATE

\param rbm - TRANSLATE.

\ru

\brief Удаление ограниченной машины Больцмана

\param rbm - указатель на описатель ограниченной машины Больцмана.

\endlang

*/
void fmll_rbm_destroy(fmll_rbm * rbm);

/*!

\en

\brief TRANSLATE

\param rbm - TRANSLATE;
\param fname_prefix - TRANSLATE (TRANSLATE).

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Сохранение в XML-файл описателя ограниченной машины Больцмана

\param rbm - указатель на описатель ограниченной машины Больцмана;
\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return 0 - в случае успешного сохранения описателя ограниченной машины Больцмана;
\return <> 0 - в случае некорректного завершения операции сохранения описателя ограниченной машины Больцмана.

\endlang

*/
int fmll_rbm_save(const fmll_rbm * rbm, const char * fname_prefix);

/*!

\en

\brief TRANSLATE

\param fname_prefix - TRANSLATE (TRANSLATE).

\return TRANSLATE;
\return NULL - TRANSLATE.

\ru

\brief Загрузка из XML-файла описателя ограниченной машины Больцмана

\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return указатель на описатель ограниченной машины Больцмана в случае его успешной загрузки;
\return NULL - в случае некорректного завершения операции загрузки описателя ограниченной машины Больцмана.

\endlang

*/
fmll_rbm * fmll_rbm_load(const char * fname_prefix);

/*!

\en

\brief TRANSLATE

\param rbm - TRANSLATE;
\param vec - TRANSLATE;
\param max_iter - TRANSLATE.

\return TRANSLATE.

\ru

\brief Прогон ограниченной машины Больцмана над некоторым вектором

\param rbm - указатель на описатель ограниченной машины Больцмана;
\param vec - некоторый вектор;
\param max_iter - количество итераций работы ограниченной машины Больцмана.

\return указатель на результирующий вектор.

\endlang

*/
const double * fmll_rbm_run(fmll_rbm * rbm, const double * vec, const unsigned max_iter);

/*!

\en

\brief TRANSLATE

\param rbm - TODO;
\param vec - TODO;
\param vec_num - TODO;
\param T_max - TODO, \f$T_{max} > 0\f$;
\param T_step_count - TODO \f$[ T_{min}, T_{max} ]\f$;
\param max_iter - TODO;
\param beta_0 - TODO, \f$\beta_0 ~ \in ~ [0, 1]\f$;
\param next_beta - TODO.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Обучение ограниченной машины Больцмана с помощью алгоритма имитации отжига и алгоритма CD-k

\param rbm - указатель на описатель ограниченной машины Больцмана;
\param vec - обучающий массив векторов;
\param vec_num - количество обучающих векторов;
\param T_max - начальная температура, \f$T_{min} < T_{max}\f$;
\param T_step_count - количество квантований температуры на промежутке \f$[ T_{min}, T_{max} ]\f$;
\param max_iter - количество итераций алгоритма CD-k на каждой итерации алгоритма имитации отжига;
\param beta_0 - начальное значение коэффициента скорости обучения на каждой итерации алгоритма имитации отжига, \f$\beta_0 ~ \in ~ [0, 1]\f$;
\param next_beta - указатель на функцию, пересчитывающую значение скорости обучения в начале каждой итерации алгоритма CD-k по значению скорости обучения на предыдущей итерации.

\return 0 - в случае успеха;
\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_rbm_CD_k(fmll_rbm * rbm, const double ** vec, const unsigned vec_num, const double T_max, const unsigned T_step_count, const unsigned max_iter, const double beta_0, double (* next_beta)(const double));

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

