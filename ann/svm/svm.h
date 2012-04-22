
/*!

\file

\en

	\brief Support Vector Machine (SVM)

	TODO

\ru

	\brief Машина опорных векторов (SVM; Support Vector Machine)

	Последовательность действий по использованию SVM:
	.
	-# Создать SVM с помощью функции fmll_svm_init();
	-# Обучить SVM с помощью одной из *_svm_teach_* функций;
	-# Прогнать SVM над целевыми векторами с помощью функции fmll_svm_run();
	-# Удалить SVM с помощью функции fmll_svm_destroy().

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

/*!

\en

	\brief TODO

\ru

	\brief Описатель машины опорных векторов

\endlang

*/
typedef struct t_fmll_svm
{

	/*! \en TODO \ru Размерность векторного пространства \endlang */
	unsigned dim;

	/*! \en TODO \ru Количество опорных векторов \endlang */
	unsigned num;

	/*!
	
	\en

		\brief TODO

		TODO.

	\ru

		\brief Коэффициенты при опорных векторах
	
		Коэффициент при опорном векторе равен произведению номера класса опорного вектора на значение соответствующего множителя Лагранжа.

	\endlang

	*/
	double * w;

	/*! \en TODO \ru Опорные векторы \endlang */
	double ** s;

	/*! \en TODO \ru Порог \endlang */
	double b;

	/*! \en TODO \ru Указатель на функцию, вычисляющую ядро скалярного произведения \endlang */
	double (* K)(const double *, const double *, unsigned);

} fmll_svm;

/* ############################################################################  */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param dim - TODO;
	\param K - TODO.

	\return TODO;
	\return NULL - TODO.

\ru

	\brief Создание SVM

	\param dim - размерность векторного пространства;
	\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

	\return указатель на описатель SVM в случае его успешного создания;
	\return NULL - в случае неудачи.

\endlang

*/
fmll_svm * fmll_svm_init(unsigned dim, double (* K)(const double *, const double *, unsigned));

/*!

\en

	\brief TODO

	\param svm - TODO.

\ru

	\brief Удаление SVM

	\param svm - указатель на описатель SVM.

\endlang

*/
void fmll_svm_destroy(fmll_svm * svm);

/*!

\en

	\brief TODO

	\param svm - TODO;
	\param fname_prefix - TODO (TODO).

	\return 0 - TODO;
	\return <> 0 - TODO.

\ru

	\brief Сохранение в XML-файл описателя SVM

	\param svm - указатель на описатель SVM;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя SVM;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя SVM.

\endlang

*/
int fmll_svm_save(fmll_svm * svm, const char * fname_prefix);

#ifdef FMLL_BUILD

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса сохранения в XML-файл описателя SVM */
int fmll_svm_save_main(fmll_svm * svm, mxml_node_t * content_node);

/*! \endcond */

#endif

/*!

\en

	\brief TODO

	\param fname_prefix - TODO (TODO);
	\param K - TODO.

	\return TODO;
	\return NULL - TODO.

\ru

	\brief Загрузка из XML-файла описателя SVM

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param K - указатель на функцию, вычисляющую ядра скалярных произведений.

	\return указатель на описатель SVM в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя SVM.

\endlang

*/
fmll_svm * fmll_svm_load(const char * fname_prefix, double (* K)(const double *, const double *, unsigned));

#ifdef FMLL_BUILD

/*! \cond HIDDEN_SYMBOLS */

/* Ядро процесса загрузки описателя SVM из XML-файла */
fmll_svm * fmll_svm_load_main(mxml_node_t * content_node, double (* K)(const double *, const double *, unsigned));

/*! \endcond */

#endif

/*!

\en

	\brief TODO

	\param svm - TODO;
	\param vec - TODO.

	\return TODO.

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

	\brief TODO

	\param svm - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param vec_num - TODO;
	\param st_func - TODO (TODO);
	\param st_param - TODO (TODO).

	TODO:
	.
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO.

	\return TODO.

\ru

	\brief Тестирование SVM

	\param svm - указатель на описатель SVM;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов (-1 или +1);
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона SVM над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

	Функция (* st_func) обладает следующими параметрами:
	.
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
unsigned fmll_svm_test(fmll_svm * svm, double ** vec, char * d, unsigned vec_num,
		void (* st_func)(fmll_svm *, double *, char, double, unsigned, bool, void *), void * st_param);

/*!

\en

	\brief TODO

	\param svm - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param vec_num - TODO;
	\param C - TODO;
	\param tau - TODO;
	\param selector - TODO;
	\param max_iter - TODO;
	\param epsilon - TODO.

	TODO:
	.
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO;
	-# TODO.

	TODO:
	.
	- 0 - TODO;
	- <> 0 - TODO.

	\return 0 - TODO;
	\return <> 0 - TODO.

	\sa fmll_svm_teach_smo_selector_keerthi, fmll_svm_teach_smo_selector_fan_chen_lin.

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
	.
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
	.
	- 0 - в случае, если очередная пара множителей Лагранжа - кандидатов на оптимизацию - найдена;
	- <> 0 - в случае необходимости останова процесса обучения.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

	\sa fmll_svm_teach_smo_selector_keerthi, fmll_svm_teach_smo_selector_fan_chen_lin.

\endlang

*/
int fmll_svm_teach_smo(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, double C, double tau,
		int (* selector)(fmll_svm *, double **, char *, unsigned, int *, int *, double, double, double, double *, double *, double **),
		unsigned max_iter, double epsilon);

/*!

\en

	\brief TODO

	TODO.

	\sa fmll_svm_teach_smo.

\ru

	\brief Выбор пары множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма SMO, по эвристике, предложенной S.S. Keerthi, S.K. Shevade, C. Bhattacharyya, K.R.K. Murthy

	Данная эвристика использует информацию только о первой производной функции Лагранжа.

	\sa fmll_svm_teach_smo.

\endlang

*/
int fmll_svm_teach_smo_selector_keerthi(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, int * ri, int * rj,
		double C, double tau, double epsilon, double * lambda, double * grad, double ** Q);

/*!

\en

	\brief TODO

	TODO.

	\sa fmll_svm_teach_smo.

\ru

	\brief Выбор пары множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма SMO, по эвристике, предложенной Rong-En Fan'ом, Pai-Hsuen Chen'ом, Chih-Jen Lin'ем

	Данная эвристика использует информацию о первой и второй производных функции Лагранжа.

	\sa fmll_svm_teach_smo.

\endlang

*/
int fmll_svm_teach_smo_selector_fan_chen_lin(fmll_svm * svm, double ** vec, char * d, unsigned vec_num, int * ri, int * rj,
		double C, double tau, double epsilon, double * lambda, double * grad, double ** Q);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

