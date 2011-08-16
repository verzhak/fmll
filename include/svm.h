
/*!

\file

\brief Машина опорных векторов (SVM; Support Vector Machine).

Последовательность действий по использованию SVM:

	-# Создать SVM с помощью функции fmll_svm_init();
	-# Обучить SVM с помощью одной из *_svm_teach_* функций;
	-# Прогнать SVM над целевыми векторами с помощью функции fmll_svm_run();
	-# Удалить SVM с помощью функции fmll_svm_destroy().

*/

#ifndef SVM_H
#define SVM_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "xml.h"
#include "kernel.h"

// ############################################################################

/*!

\brief Описатель машины опорных векторов.

*/

typedef struct t_fmll_svm
{

	/*! Размерность векторного пространства. */
	unsigned dim;

	/*! Количество опорных векторов. */
	unsigned num;

	/*!
	
	\brief Коэффициенты при опорных векторах.
	
	Коэффициент при опорном векторе равен произведению номера класса опорного вектора на значение соответствующего множителя Лагранжа.

	*/
	double * w;

	/*! Опорные векторы. */
	double ** s;

	/*! Порог. */
	double b;

	/*! Указатель на функцию, вычисляющую ядро скалярного произведения. */
	double (* K)(const double *, const double *, unsigned);

} fmll_svm;

// ############################################################################ 

/*!

\brief Создание SVM.

\param dim - размерность векторного пространства;
\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

\return

	- указатель на описатель SVM в случае его успешного создания;
	- NULL в случае неудачи.

*/
fmll_svm * fmll_svm_init(unsigned dim, double (* K)(const double *, const double *, unsigned));

/*!

\brief Удаление SVM.

\param svm - указатель на описатель SVM.

*/
void fmll_svm_destroy(fmll_svm * svm);

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса сохранения в XML-файл описателя SVM */
int fmll_svm_save_main(fmll_svm * svm, mxml_node_t * content_node);

/*! \endcond */

/*!

\brief Сохранение в XML-файл описателя SVM.

\param svm - указатель на описатель SVM;
\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return

	- 0 - в случае успешного сохранения описателя SVM;
	- <> 0 - в случае некорректного завершения операции сохранения описателя SVM.

*/
int fmll_svm_save(fmll_svm * svm, const char * fname_prefix);

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса загрузки описателя SVM из XML-файла */
fmll_svm * fmll_svm_load_main(mxml_node_t * content_node, double (* K)(const double *, const double *, unsigned));

/*! \endcond */

/*!

\brief Загрузка из XML-файла описателя SVM.

\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

\return

	- указатель на описатель SVM в случае его успешной загрузки;
	- NULL - в случае некорректного завершения операции загрузки описателя SVM.

*/
fmll_svm * fmll_svm_load(const char * fname_prefix, double (* K)(const double *, const double *, unsigned));

/*!

\brief Прогон SVM над вектором.

\param svm - указатель на описатель SVM;
\param vec - указатель на вектор.

\return вещественное число из диапазона [-1; 1]. Точное равенство выхода функции одной из границ диапазона суть свидетельство успешной классификации вектора машиной опорных векторов (отнесение к классу -1 или 1), нахождение выхода функции в диапазоне (-1; 1) свидетельствует о том, что вектор, возможно, классифицирован неверно.

*/
double fmll_svm_run(fmll_svm * svm, const double * vec);

/*!

\brief Обучение SVM TODO.

\param svm - дескриптор SVM;
\param vec - массив обучающих векторов;
\param d - номера классов обучающих векторов (-1 или +1);
\param vec_num - количество векторов в массиве обучающих векторов;
\param TODO - TODO.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
// int fmll_svm_teach_TODO(fmll_svm * svm, double ** vec, char * d, unsigned vec_num,
//		TODO);

// ############################################################################

#endif

