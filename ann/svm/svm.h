
/*!

\file

\en

\brief Support Vector Machine (SVM)

TRANSLATE

\ru

\brief Машина опорных векторов (SVM; Support Vector Machine)

Алгоритм работы с машиной опорных векторов:

-# создать SVM с помощью функции fmll_svm_init();
-# обучить SVM с помощью одной из *_svm_teach_* функций;
-# прогнать SVM над целевыми векторами с помощью функции fmll_svm_run();
-# удалить SVM с помощью функции fmll_svm_destroy().

\endlang

*/

#ifndef FMLL_SVM_H
#define FMLL_SVM_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "ann/svm/kernel.h"

#endif

/* ############################################################################ */

/*! \en \brief TRANSLATE \ru \brief Описатель машины опорных векторов \endlang */
typedef struct t_fmll_svm
{

	/*! \en TRANSLATE \ru Размерность векторного пространства \endlang */
	unsigned dim;

	/*! \en TRANSLATE \ru Количество опорных векторов \endlang */
	unsigned num;

/*!

\en

\brief TRANSLATE

TRANSLATE.

\ru

\brief Коэффициенты при опорных векторах

Коэффициент при опорном векторе равен произведению номера класса опорного вектора на значение соответствующего множителя Лагранжа.

\endlang

*/
	double * w;

	/*! \en TRANSLATE \ru Опорные векторы \endlang */
	double ** s;

	/*! \en TRANSLATE \ru Порог \endlang */
	double b;

	/*! \en TRANSLATE \ru Указатель на функцию, вычисляющую ядро скалярного произведения \endlang */
	double (* K)(const double *, const double *, unsigned);

} fmll_svm;

/* ############################################################################  */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param dim - TRANSLATE;
\param K - TRANSLATE.

\return TRANSLATE;
\return NULL - TRANSLATE.

\ru

\brief Создание SVM

\param dim - размерность векторного пространства;
\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

\return указатель на описатель SVM в случае его успешного создания;
\return NULL - в случае неудачи.

\endlang

*/
fmll_svm * fmll_svm_init(const unsigned dim, double (* K)(const double *, const double *, const unsigned));

/*!

\en

\brief TRANSLATE

\param svm - TRANSLATE.

\ru

\brief Удаление SVM

\param svm - указатель на описатель SVM.

\endlang

*/
void fmll_svm_destroy(fmll_svm * svm);

/*!

\en

\brief TRANSLATE

\param svm - TRANSLATE;
\param fname_prefix - TRANSLATE (TRANSLATE).

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Сохранение в XML-файл описателя SVM

\param svm - указатель на описатель SVM;
\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return 0 - в случае успешного сохранения описателя SVM;
\return <> 0 - в случае некорректного завершения операции сохранения описателя SVM.

\endlang

*/
int fmll_svm_save(const fmll_svm * svm, const char * fname_prefix);

#ifdef FMLL_BUILD

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса сохранения в XML-файл описателя SVM */
int fmll_svm_save_main(const fmll_svm * svm, mxml_node_t * content_node);

/*! \endcond */

#endif

/*!

\en

\brief TRANSLATE

\param fname_prefix - TRANSLATE (TRANSLATE);
\param K - TRANSLATE.

\return TRANSLATE;
\return NULL - TRANSLATE.

\ru

\brief Загрузка из XML-файла описателя SVM

\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

\return указатель на описатель SVM в случае его успешной загрузки;
\return NULL - в случае некорректного завершения операции загрузки описателя SVM.

\endlang

*/
fmll_svm * fmll_svm_load(const char * fname_prefix, double (* K)(const double *, const double *, const unsigned));

#ifdef FMLL_BUILD

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса загрузки описателя SVM из XML-файла */
fmll_svm * fmll_svm_load_main(mxml_node_t * content_node, double (* K)(const double *, const double *, const unsigned));

/*! \endcond */

#endif

/*!

\en

\brief TRANSLATE

\param svm - TRANSLATE;
\param vec - TRANSLATE.

\return TRANSLATE.

\ru

\brief Прогон SVM над вектором

\param svm - указатель на описатель SVM;
\param vec - указатель на вектор.

\return вещественное число из диапазона [-1; 1]. Точное равенство выхода функции одной из границ диапазона суть свидетельство успешной классификации вектора машиной опорных векторов (отнесение к классу -1 или 1), нахождение выхода функции в диапазоне (-1; 1) свидетельствует о том, что вектор, возможно, классифицирован неверно.

\endlang

*/
double fmll_svm_run(fmll_svm * svm, const double * vec);

/*!

\en

\brief TRANSLATE

\param svm - TRANSLATE;
\param vec - TRANSLATE;
\param d - TRANSLATE;
\param vec_num - TRANSLATE;
\param st_func - TRANSLATE (TRANSLATE);
\param st_param - TRANSLATE (TRANSLATE).

TRANSLATE:

-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE.

\return TRANSLATE.

\ru

\brief Тестирование SVM

\param svm - указатель на описатель SVM;
\param vec - тестовое множество векторов;
\param d - множество эталонных откликов (-1 или +1);
\param vec_num - количество векторов в тестовом множестве векторов;
\param st_func - указатель на функцию, вызываемую после прогона SVM над каждым вектором из тестового множества (может принимать значение NULL);
\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

Функция (* st_func) обладает следующими параметрами:

-# указатель на описатель SVM;
-# указатель на обрабатываемый вектор;
-# эталонный отклик;
-# выход SVM;
-# количество векторов в тестовом множестве векторов;
-# булева переменная, установленная в true, если обрабатываемый	вектор классифицирован правильно, и установленная в false - в противном случае;
-# значение параметра st_param.

\return количество векторов из тестового множества векторов, классифицированных правильно.

\endlang

*/
unsigned fmll_svm_test(fmll_svm * svm, const double ** vec, const char * d, const unsigned vec_num,
		void (* st_func)(const fmll_svm *, const double *, const char, const double, const unsigned, const bool, void *), void * st_param);

/*!

\en

\brief TRANSLATE

\param svm - TRANSLATE;
\param vec - TRANSLATE;
\param d - TRANSLATE;
\param vec_num - TRANSLATE;
\param C - TRANSLATE;
\param tau - TRANSLATE;
\param selector - TRANSLATE;
\param max_iter - TRANSLATE;
\param epsilon - TRANSLATE.

TRANSLATE:

-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE;
-# TRANSLATE.

TRANSLATE:

- 0 - TRANSLATE;
- <> 0 - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Обучение SVM по алгоритму Sequential Minimal Optimization (SMO)

\param svm - указатель на описатель SVM;
\param vec - множество обучающих векторов;
\param d - номера классов обучающих векторов (-1 или +1);
\param vec_num - мощность множества обучающих векторов;
\param C - ширина полосы поиска коэффициентов \f$\lambda\f$;
\param tau - малое положительное число, в некоторых эвристиках используемое при выборе второго из множителей Лагранжа - кандидатов на оптимизацию;
\param selector - указатель на функцию, выбирающую пару множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма;
\param max_iter - максимальное количество итераций алгоритма обучения;
\param epsilon - малое положительное число, при достижении которого разностью частных производных функции Лагранжа, умноженных на соответствующие эталонные отклики, взятые с обратным знаком, очередной пары множителей Лагранжа - кандидатов на оптимизацию - алгоритм будет остановлен.

Функция (* selector) обладает следующими параметрами:

-# указатель на описатель SVM;
-# множество обучающих векторов;
-# номера классов обучающих векторов (-1 или +1);
-# мощность множества обучающих векторов;
-# указатель на переменную, в которой будет возвращен индекс первого множителя Лагранжа из пары множителей - кандидатов на оптимизацию;
-# указатель на переменную, в которой будет возвращен индекс второго множителя Лагранжа из пары множителей - кандидатов на оптимизацию;
-# ширина полосы поиска коэффициентов \f$\lambda\f$;
-# малое положительное число, в некоторых эвристиках используемое при выборе второго из множителей Лагранжа - кандидатов на оптимизацию;
-# малое положительное число, при достижении которого разностью частных производных функции Лагранжа, умноженных на соответствующие эталонные отклики, взятые с обратным знаком, очередной пары множителей Лагранжа - кандидатов на оптимизацию - алгоритм будет остановлен;
-# массив текущих значений множителей Лагранжа;
-# текущее значение градиента;
-# матрица значений ядра скалярного произведения, умноженного на значения эталонных откликов, для каждой пары векторов из множества обучающих векторов.

Функция (* selector) возвращает:

- 0 - в случае, если очередная пара множителей Лагранжа - кандидатов на оптимизацию - найдена;
- <> 0 - в случае необходимости останова процесса обучения.

\return 0 - в случае успеха;
\return <> 0 - в случае неудачи.

\endlang

\sa fmll_svm_teach_smo_selector_keerthi, fmll_svm_teach_smo_selector_fan_chen_lin

*/
int fmll_svm_teach_smo(fmll_svm * svm, const double ** vec, const char * d, const unsigned vec_num, const double C, const double tau,
		int (* selector)(const fmll_svm *, const double **, const char *, const unsigned, int *, int *, const double, const double, const double, const double *, const double *, const double **),
		const unsigned max_iter, const double epsilon);

/*!

\en

\brief TRANSLATE

TRANSLATE.

\ru

\brief Выбор пары множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма SMO, по эвристике, предложенной S.S. Keerthi, S.K. Shevade, C. Bhattacharyya, K.R.K. Murthy

Данная эвристика использует информацию только о первой производной функции Лагранжа.

\endlang

\sa fmll_svm_teach_smo

*/
int fmll_svm_teach_smo_selector_keerthi(const fmll_svm * svm, const double ** vec, const char * d, const unsigned vec_num, int * ri, int * rj,
		const double C, const double tau, const double epsilon, const double * lambda, const double * grad, const double ** Q);

/*!

\en

\brief TRANSLATE

TRANSLATE.

\ru

\brief Выбор пары множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма SMO, по эвристике, предложенной Rong-En Fan'ом, Pai-Hsuen Chen'ом, Chih-Jen Lin'ем

Данная эвристика использует информацию о первой и второй производных функции Лагранжа.

\endlang

\sa fmll_svm_teach_smo

*/
int fmll_svm_teach_smo_selector_fan_chen_lin(const fmll_svm * svm, const double ** vec, const char * d, const unsigned vec_num, int * ri, int * rj,
		const double C, const double tau, const double epsilon, const double * lambda, const double * grad, const double ** Q);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

