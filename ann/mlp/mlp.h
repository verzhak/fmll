
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief Многослойный перцептрон

	Последовательность действий по использованию перцептрона:

	-# создать перцептрон с помощью функции fmll_mlp_init();
	-# иницилизировать веса синапсов нейронов перцептрона с помощью одной из *_mlp_weight_init_* функций;
	-# обучить перцептрон с помощью одной из *_mlp_teach_* функций;
	-# оценить качество обучения перцептрона с помощью функции fmll_mlp_test();
	-# прогнать перцептрон над целевыми векторами с помощью функции fmll_mlp_run();
	-# удалить перцептрон с помощью функции fmll_mlp_destroy().

	\sa activate.h, weight_init.h

\endlang

*/

#ifndef FMLL_mlp_H
#define FMLL_mlp_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/matrix/matrix.h"
	#include "math/random/random.h"

#endif

/* ############################################################################ */

/*!

\en

	\brief TRANSLATE

\ru

	\brief Описатель перцептрона

\endlang

*/
typedef struct t_fmll_mlp
{

	/*! \en TRANSLATE \ru Массив весов синапсов нейронов \endlang */
	double ** w;

	/*! \en TRANSLATE \ru Размерность входного вектора \endlang */
	unsigned dim;

	/*! \en TRANSLATE \ru Количество скрытых слоев \endlang */
	unsigned layers_num;

	/*! \en TRANSLATE \ru Количество нейронов в каждом из скрытых слоев \endlang */
	unsigned * N;

	/*! \en TRANSLATE \ru Суммарное количество нейронов во всех скрытых слоях \endlang */
	unsigned num;

	/*! \en TRANSLATE \ru Массив функций активаций (своя функция активации для каждого из скрытых слоев) \endlang */
	double (** fun)(double);

	/*! \en TRANSLATE \ru Массив производных функций активаций \endlang */
	double (** d_fun)(double);

	/*! \cond HIDDEN_SYMBOLS */

	unsigned max_N, num_weight;
	double ** net;
	double *** y; /* Массив входных и выходных векторов значений слоев перцептрона (в том числе и вектор выходных значений) */

	/*! \endcond */

} fmll_mlp;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param dim - TRANSLATE;
	\param layers_num - TRANSLATE;
	\param N - TRANSLATE;
	\param fun - TRANSLATE;
	\param d_fun - TRANSLATE.

	\return TRANSLATE;
	\return TRANSLATE.

\ru

	\brief Создание перцептрона

	\param dim - размер входного вектора;
	\param layers_num - количество скрытых слоев;
	\param N - количество нейронов в каждом из скрытых слоев;
	\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
	\param d_fun - массив производных функций активации нейронов.

	Веса синапсов нейронов перцептрона устанавливаются в ноль.

	\return указатель на описатель перцептрона в случае его успешного создания;
	\return NULL в случае неудачи.

\endlang

*/
fmll_mlp * fmll_mlp_init(const unsigned dim, const unsigned layers_num, const unsigned * N, double (** fun)(const double), double (** d_fun)(const double));

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE.

\ru

	\brief Удаление перцептрона

	\param mlp - указатель на описатель перцептрона.

\endlang

*/
void fmll_mlp_destroy(fmll_mlp * mlp);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Сохранение в XML-файл описателя перцептрона

	\param mlp - указатель на описатель перцептрона;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя перцептрона;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя перцептрона.

\endlang

*/
int fmll_mlp_save(const fmll_mlp * mlp, const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param fname_prefix - TRANSLATE (TRANSLATE);
	\param fun - TRANSLATE (TRANSLATE);
	\param d_fun - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Загрузка из XML-файла описателя перцептрона

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
	\param d_fun - массив производных функций активации нейронов.

	\return указатель на описатель перцептрона в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя перцептрона.

\endlang

*/
fmll_mlp * fmll_mlp_load(const char * fname_prefix, double (** fun)(const double), double (** d_fun)(const double));

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param vec - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Прогон перцептрона над некоторым вектором

	\param mlp - указатель на описатель перцептрона;
	\param vec - некоторый вектор.

	\return указатель на массив выходных значений перцептрона.

\endlang

*/
const double * fmll_mlp_run(fmll_mlp * mlp, const double * vec);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param vec - TRANSLATE;
	\param d - TRANSLATE;
	\param deviation - TRANSLATE;
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

	\brief Тестирование перцептрона

	\param mlp - указатель на описатель перцептрона;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов;
	\param deviation - допустимые отклонения для каждого из откликов;
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона перцептрона над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

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
unsigned fmll_mlp_test(fmll_mlp * mlp, const double ** vec, const double ** d, const double * deviation, const unsigned vec_num,
		void (* st_func)(const fmll_mlp *, const double *, const double *, const double *, const unsigned, const bool, void *), void * st_param);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param vec - TRANSLATE;
	\param d - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param beta_0 - TRANSLATE;
	\param next_beta - TRANSLATE;
	\param coef_moment - TRANSLATE;
	\param max_iter - TRANSLATE;
	\param E_thres - TRANSLATE;
	\param d_E_thres - TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Обучение перцептрона путем пакетного градиентного спуска с использованием алгоритма обратного распространения ошибки

	\param mlp - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение коэффициента скорости обучения в начале каждой итерации обучения по значению коэффициента на предыдущей итерации;
	\param coef_moment - коэффициент момента;
	\param max_iter - максимальное количество итераций процесса обучения;
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_teach_gradient_batch(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num,
		const double beta_0, double (* next_beta)(const double), const double coef_moment, const unsigned max_iter, const double E_thres, const double d_E_thres);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param vec - TRANSLATE;
	\param d - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param eta_0 - TRANSLATE;
	\param eta_coef - TRANSLATE (TRANSLATE);
	\param max_iter - TRANSLATE;
	\param E_thres - TRANSLATE;
	\param d_E_thres - TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Обучение перцептрона по алгоритму Левенберга - Марквардта (один из квазиньютоновских методов)

	\param mlp - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param eta_0 - множитель для начального значения коэффициента Марквардта;
	\param eta_coef - делитель коэффициента Марквардта (должен быть больше единицы);
	\param max_iter - максимальное количество итераций алгоритма обучения;
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_teach_Levenberg_Marquardt(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num,
		const double eta_0, const double eta_coef, const unsigned max_iter, const double E_thres, const double d_E_thres);

/*!

\en

	\brief TRANSLATE

	\param mlp - TRANSLATE;
	\param vec - TRANSLATE;
	\param d - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param max_iter - TRANSLATE;
	\param coef_E - TRANSLATE;
	\param E_thres - TRANSLATE;
	\param d_E_thres - TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Обучение перцептрона по алгоритму сопряженных градиентов

	\param mlp - указатель на описатель перцептрона;
	\param vec - массив обучающих векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param max_iter - максимальное количество итераций процесса обучения;
	\param coef_E - множитель, определяющий максимальный размер области поиска оптимального значения параметра \f$\eta\f$, при достижении которого поиск очередного значения параметра \f$\eta\f$ будет завершен, как \f$\eta ~ E\f$, где \f$E\f$ - текущее значение функции ошибки; coef_E может принимать значения из диапазона (0, 1);
	\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
	\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_mlp_teach_conjugate_gradient(fmll_mlp * mlp, const double ** vec, const double ** d, const unsigned vec_num, const unsigned max_iter,
		const double coef_E, const double E_thres, const double d_E_thres);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

