
/*!

\file

\en

	\brief TODO.

	TODO

\ru

	\brief Однослойные и многослойные перцептроны.

	Последовательность действий по использованию перцептрона:

		-# Создать перцептрон с помощью функции fmll_perceptron_init();
		-# Обучить перцептрон с помощью одной из *_perceptron_teach_* функций;
		-# Оценить качество обучения перцептрона с помощью функции fmll_perceptron_test();
		-# Прогнать перцептрон над целевыми векторами с помощью функции fmll_perceptron_run();
		-# Удалить перцептрон с помощью функции fmll_perceptron_destroy().

\endlang

*/

#ifndef FMLL_PERCEPTRON_H
#define FMLL_PERCEPTRON_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/matrix.h"
	#include "nn/weight_init.h"

#endif

// ############################################################################

/*!

\en

	\brief TODO.

\ru

	\brief Описатель перцептрона.

\endlang

*/
typedef struct t_fmll_perceptron
{

	/*! \en TODO. \ru Массив нейронов. \endlang */
	double ** w;

	/*! \en TODO. \ru Массив входных и выходных векторов значений слоев перцептрона (в том числе и вектор выходных значений). \endlang */
	double *** y;

	/*! \en TODO. \ru Размерность входного вектора. \endlang */
	unsigned dim;

	/*! \en TODO. \ru Количество скрытых слоев. \endlang */
	unsigned layers_num;

	/*! \en TODO. \ru Количество нейронов в каждом из скрытых слоев. \endlang */
	unsigned * N;

	/*! \en TODO. \ru Суммарное количество нейронов во всех скрытых слоях. \endlang */
	unsigned num;

	/*! \en TODO. \ru Массив функций активаций (своя функция активации для каждого из скрытых слоев). \endlang */
	double (** fun)(double);

	/*! \en TODO. \ru Массив производных функций активаций. \endlang */
	double (** d_fun)(double);

	/*! \cond HIDDEN_SYMBOLS */

	unsigned max_N, num_weight;
	double ** net;

	/*! \endcond */

} fmll_perceptron;

// ############################################################################

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO.

	\param dim - TODO;
	\param layers_num - TODO;
	\param N - TODO;
	\param weight_init - TODO;
	\param fun - TODO;
	\param d_fun - TODO.

	\return

		- TODO;
		- TODO.

\ru

	\brief Создание перцептрона.

	\param dim - размер входного вектора;
	\param layers_num - количество скрытых слоев;
	\param N - количество нейронов в каждом из скрытых слоев;
	\param weight_init - указатель на функцию, инициализирующую веса синапсов нейронов;
	\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
	\param d_fun - массив производных функций активации нейронов.

	\return

		- указатель на описатель перцептрона в случае его успешного создания;
		- NULL в случае неудачи.

\endlang

*/
fmll_perceptron * fmll_perceptron_init(unsigned dim, unsigned layers_num, const unsigned * N,
		double (* weight_init)(), double (** fun)(double), double (** d_fun)(double));

/*!

\en

	\brief TODO.

	\param perc - TODO.

\ru

	\brief Удаление перцептрона.

	\param perc - указатель на описатель перцептрона.

\endlang

*/
void fmll_perceptron_destroy(fmll_perceptron * perc);

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param fname_prefix - TODO (TODO).

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Сохранение в XML-файл описателя перцептрона.

	\param perc - указатель на описатель перцептрона;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return

		- 0 - в случае успешного сохранения описателя перцептрона;
		- <> 0 - в случае некорректного завершения операции сохранения описателя перцептрона.

\endlang

*/
int fmll_perceptron_save(fmll_perceptron * perc, const char * fname_prefix);

/*!

\en

	\brief TODO.

	\param fname_prefix - TODO (TODO);
	\param fun - TODO (TODO);
	\param d_fun - TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Загрузка из XML-файла описателя перцептрона.

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
	\param d_fun - массив производных функций активации нейронов.

	\return

		- указатель на описатель перцептрона в случае его успешной загрузки;
		- NULL - в случае некорректного завершения операции загрузки описателя перцептрона.

\endlang

*/
fmll_perceptron * fmll_perceptron_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double));

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param vec - TODO.

	\return TODO.

\ru

	\brief Прогон перцептрона над некоторым вектором.

	\param perc - указатель на описатель перцептрона;
	\param vec - некоторый вектор.

	\return указатель на массив выходных значений перцептрона.

\endlang

*/
const double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec);

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param deviation - TODO;
	\param vec_num - TODO;
	\param st_func - TODO (TODO);
	\param st_param - TODO (TODO).

	TODO

	TODO:

		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO.

	\return TODO.

\ru

	\brief Тестирование перцептрона.

	\param perc - указатель на описатель перцептрона;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов;
	\param deviation - допустимые отклонения для каждого из откликов;
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона перцептрона над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

	Массив deviation имеет размерность N x 2, где N - количество выходов перцептрона (количество нейронов в последнем слое перцептрона). Нулевой элемент второй размерности означает максимальное допустимое отклонение влево от соответствующего эталонного отклика, первый элемент - соответственно, вправо. Контроль за отношением нулевого элемента второй размерности к первому элементу сознательно не реализован.

	Функция (* st_func) обладает следующими параметры:

		-# указатель на описатель перцептрона;
		-# указатель на обрабатываемый вектор;
		-# указатель на вектор эталонных откликов, соответствующий обрабатываемому вектору;
		-# указатель на вектор выходов перцептрона;
		-# количество векторов в тестовом множестве векторов;
		-# булева переменная, установленная в true, если вектор выходов перцептрона равен вектору эталонных откликов с точностью до указанных допустимых отклонений, и установленная в false - в противном случае;
		-# значение параметра st_param.

	\return количество векторов из тестового множества векторов, вектор выходов перцептрона для которых равен соответствующим векторам эталонных откликов с точностью до указанных допустимых отклонений.

\endlang

*/
unsigned fmll_perceptron_test(fmll_perceptron * perc, double ** vec, double ** d, double ** deviation, unsigned vec_num,
		void (* st_func)(fmll_perceptron *, double *, double *, const double *, unsigned, bool, void *), void * st_param);

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param vec_num - TODO;
	\param beta_0 - TODO;
	\param next_beta - TODO;
	\param coef_moment - TODO;
	\param max_iter - TODO;
	\param E_thres - TODO;
	\param d_E_thres - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Обучение перцептрона путем пакетного градиентного спуска с использованием алгоритма обратного распространения ошибки.

	\param perc - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение коэффициента скорости обучения в начале каждой итерации обучения по значению коэффициента на предыдущей итерации;
	\param coef_moment - коэффициент момента;
	\param max_iter - максимальное количество итераций процесса обучения;
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_teach_gradient_batch(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, unsigned max_iter, double E_thres, double d_E_thres);

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param vec_num - TODO;
	\param eta_0 - TODO;
	\param eta_coef - TODO (TODO);
	\param max_iter - TODO;
	\param E_thres - TODO;
	\param d_E_thres - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Обучение перцептрона по алгоритму Левенберга - Марквардта (один из квазиньютоновских методов).

	\param perc - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param eta_0 - множитель для начального значения коэффициента Марквардта;
	\param eta_coef - делитель коэффициента Марквардта (должен быть больше единицы);
	\param max_iter - максимальное количество итераций алгоритма обучения;
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_teach_Levenberg_Marquardt(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		double eta_0, double eta_coef, unsigned max_iter, double E_thres, double d_E_thres);

/*!

\en

	\brief TODO.

	\param perc - TODO;
	\param vec - TODO;
	\param d - TODO;
	\param vec_num - TODO;
	\param max_iter - TODO;
	\param coef_E - TODO;
	\param E_thres - TODO;
	\param d_E_thres - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Обучение перцептрона по алгоритму сопряженных градиентов.

	\param perc - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param max_iter - максимальное количество итераций процесса обучения;
	\param coef_E - множитель, определяющий максимальный размер области поиска оптимального значения параметра \f$\eta\f$, при достижении которого поиск очередного значения параметра \f$\eta\f$ будет завершен, как \f$\eta ~ E\f$, где \f$E\f$ - текущее значение функции ошибки; coef_E может принимать значения из диапазона (0, 1);
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_perceptron_teach_conjugate_gradient(fmll_perceptron * perc, double ** vec, double ** d, unsigned vec_num,
		unsigned max_iter, double coef_E, double E_thres, double d_E_thres);

#ifdef __cplusplus
}
#endif

// ############################################################################

#endif

