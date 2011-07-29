
/*!

\file

\brief Однослойные и многослойные перцептроны.

Последовательность действий по использованию перцептрона:

	-# Создать перцептрон с помощью функции fmll_perceptron_init();
	-# Обучить перцептрон с помощью одной из *_teach_* функций;
	-# Прогнать перцептрон над целевыми векторами с помощью функции fmll_perceptron_run();
	-# Удалить перцептрон с помощью функции fmll_perceptron_destroy().

*/

#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "xml.h"
#include "weight_init.h"

// ############################################################################

/*!

\brief Описатель перцептрона.

*/
typedef struct t_fmll_perceptron
{

	/*! Массив нейронов. */
	double ** w;

	/*! Вектор выходных значений. */
	double *** y;

	/*! Размерность входного вектора. */
	uint8_t dim;

	/*! Количество скрытых слоев. */
	uint8_t layers_num;

	/*! Количество нейронов в каждом из скрытых слоев. */
	uint16_t * N;

	/*! Суммарное количество нейронов во всех скрытых слоях. */
	uint32_t num;

	/*! Массив функций активаций (своя функция активации для каждого из скрытых слоев). */
	double (** fun)(double);

	/*! Массив производных функций активаций. */
	double (** d_fun)(double);

	/*! \cond HIDDEN_SYMBOLS */

	uint16_t max_N;
	double ** net;

	/*! \endcond */

} fmll_perceptron;

// ############################################################################

/*!

\brief Создание перцептрона.

\param dim - размер входного вектора;
\param layers_num - количество скрытых слоев;
\param N - количество нейронов в каждом из скрытых слоев;
\param weight_init - указатель на функцию, инициализирующую веса синапсов нейронов;
\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
\param d_fun - массив производных функций активации нейронов.

\return

	- указатель на описатель перцептрона в случае успешного создания перцептрона;
	- NULL в случае неудачи.

*/
fmll_perceptron * fmll_perceptron_init(uint8_t dim, uint8_t layers_num, const uint16_t * N,
		double (* weight_init)(), double (** fun)(double), double (** d_fun)(double));

/*!

\brief Удаление перцептрона.

\param perc - указатель на описатель перцептрона.

*/
void fmll_perceptron_destroy(fmll_perceptron * perc);

/*!

\brief Сохранение в XML-файл описателя перцептрона.

\param perc - указатель на описатель перцептрона;
\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return

	- 0 - в случае успешного сохранения описателя перцептрона;
	- <> 0 - в случае некорректного завершения операции сохранения описателя перцептрона.

*/
int8_t fmll_perceptron_save(fmll_perceptron * perc, const char * fname_prefix);

/*!

\brief Загрузка из XML-файла описателя перцептрона.

\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
\param fun - массив функций активации нейронов (своя функция активации для каждого из скрытых слоев);
\param d_fun - массив производных функций активации нейронов.

\return

	- указатель на описатель перцептрона - в случае успешной загрузки описателя перцептрона;
	- NULL - в случае некорректного завершения операции загрузки описателя перцептрона.

*/
fmll_perceptron * fmll_perceptron_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double));

/*!

\brief Прогон перцептрона над некоторым вектором.

\param perc - указатель на описатель перцептрона;
\param vec - некоторый вектор.

\return указатель на массив выходных значений перцептрона.

*/
double * fmll_perceptron_run(fmll_perceptron * perc, const double * vec);

/*!

\brief Обучение перцептрона путем пакетного градиентного спуска с использованием алгоритма обратного распространения ошибки.

\param perc - дескриптор перцептрона;
\param vec - массив обучающих векторов;
\param d - множество эталонных откликов;
\param vec_num - количество векторов в массиве обучающих векторов;
\param beta_0 - начальное значение скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
\param next_beta - указатель на функцию, пересчитывающую значение скорости обучения в начале каждой итерации обучения по значению скорости обучения на предыдущей итерации;
\param coef_moment - коэффициент момента;
\param max_iter - максимальное количество итераций процесса обучения;
\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int8_t fmll_perceptron_teach_gradient_batch(fmll_perceptron * perc, double ** vec, double ** d, uint32_t vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, uint32_t max_iter, double E_thres, double d_E_thres);

/*!

\brief Обучение перцептрона по алгоритму Левенберга - Марквардта (один из квазиньютоновских методов).

\param perc - дескриптор перцептрона;
\param vec - массив обучающих векторов;
\param d - множество эталонных откликов;
\param vec_num - количество векторов в массиве обучающих векторов;
\param eta_mult - множитель для начального значения коэффициента Марквардта;
\param eta_coef - делитель коэффициента Марквардта (должен быть больше единицы);
\param max_iter - максимальное количество итераций процесса обучения;
\param E_thres - максимальное значение ошибки, при котором обучение будет остановлено;
\param d_E_thres - минимальное значение модуля производной функционала ошибки, при котором обучение будет остановлено.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int8_t fmll_perceptron_teach_lm(fmll_perceptron * perc, double ** vec, double ** d, uint32_t vec_num,
		double eta_0, double eta_coef, uint32_t max_iter, double E_thres, double d_E_thres);

// ############################################################################

#endif

