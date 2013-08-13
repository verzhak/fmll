
/*!

\file

\en

\brief TRANSLATE

TRANSLATE

\ru

\brief Нелинейная ячеистая нейронная сеть (NL-CNN - Non-linear cellular neural network)

Алгоритм работы с нелинейной ячеистой нейронной сетью:

-# создать NL-CNN с помощью функции fmll_cnn_init();
-# прогнать NL-CNN над целевой матрицей с помощью функции fmll_cnn_run();
-# удалить NL-CNN с помощью функции fmll_cnn_destroy().

\endlang

*/

#ifndef FMLL_CNN_H
#define FMLL_CNN_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/matrix/matrix.h"

#endif

/* ############################################################################ */

/*! \en \brief TRANSLATE \ru \brief Описатель нелинейной ячеистой нейронной сети \endlang */
typedef struct t_fmll_cnn
{

	/*! \en TRANSLATE \ru Количество строк во входной матрице \endlang */
	unsigned rows;

	/*! \en TRANSLATE \ru Количество столбцов во входной матрице \endlang */
	unsigned cols;

	/*! \en TRANSLATE \ru Матрица выходов нейронов \endlang */
	double ** Y;

	/*! \cond HIDDEN_SYMBOLS */

	double ** net;
	double (* fun)(const double);
	double (* A)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned);
	double (* B)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned);
	double (* I)(const double **, const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned);

	/*! \endcond */

} fmll_cnn;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param rows - TRANSLATE;
\param cols - TRANSLATE;
\param fun - TRANSLATE;
\param A - TRANSLATE;
\param B - TRANSLATE;
\param I - TRANSLATE.

TRANSLATE:

-# TRANSLATE (TRANSLATE) or TRANSLATE (TRANSLATE);
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE (TRANSLATE);
-# TRANSLATE (TRANSLATE);
-# TRANSLATE;
-# TRANSLATE;

\return TRANSLATE;
\return NULL - TRANSLATE.

\ru

\brief Создание нелинейной ячеистой нейронной сети

\param rows - количество строк во входной матрице сети;
\param cols - количество столбцов во входной матрице сети;
\param fun - функция активации нейронов;
\param A - функция расчета индуцированного поля соседства нейрона;
\param B - функция расчета индуцированного поля входных данных нейрона;
\param I - функция расчета порогового значения нейрона.

Функции A, B и I обладают следующими параметрами:

- указатель на матрицу выходов нейронов на предыдущей итерации алгоритма (функция A) или указатель на исходную матрицу (функции B и I);
- указатель на матрицу выходов нейронов на предыдущей итерации алгоритма (только функция I);
- номер строки нейрона в двухмерной сетке нейронов;
- номер столбца нейрона в двухмерной сетке нейронов;
- количество строк в исходной матрице (количество строк в двухмерной сетке нейронов);
- количество столбцов в исходной матрице (количество столбцов в двухмерной сетке нейронов);
- номер текущего прогона NL-CNN;
- количество прогонов NL-CNN над исходной матрицей.

\return указатель на описатель NL-CNN в случае ее успешного создания;
\return NULL - в случае неудачи.

\endlang

*/
fmll_cnn * fmll_cnn_init(const unsigned rows, const unsigned cols, double (* fun)(const double), double (* A)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned),
		double (* B)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned),
		double (* I)(const double **, const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned));

/*!

\en

\brief TRANSLATE

\param cnn - TRANSLATE.

\ru

\brief Удаление нелинейной ячеистой нейронной сети

\param cnn - указатель на описатель NL-CNN.

\endlang

*/
void fmll_cnn_destroy(fmll_cnn * cnn);

/*!

\en

\brief TRANSLATE

\param cnn - TRANSLATE;
\param M - TRANSLATE;
\param max_iter - TRANSLATE.

\return TRANSLATE.

\ru

\brief Прогон нелинейной ячеистой нейронной сети над некоторой матрицей

\param cnn - указатель на описатель NL-CNN;
\param M - исходная матрица;
\param max_iter - количество прогонов NL-CNN над исходной матрицей.

\return указатель на результирующую матрицу.

\endlang

*/
const double ** fmll_cnn_run(fmll_cnn * cnn, const double ** M, const unsigned max_iter);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

