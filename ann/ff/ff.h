
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief Сеть прямого распространения (feed forward ann)

	Последовательность действий по использованию сети прямого распространения:

	-# Создать сеть с помощью функции fmll_ff_init();
	-# Иницилизировать веса синапсов нейронов сети с помощью одной из *_ff_weight_init_* функций;
	-# Обучить сеть с помощью одной из *_ff_teach_* функций;
	-# Оценить качество обучения сети с помощью функции fmll_ff_test();
	-# Прогнать сеть над целевыми векторами с помощью функции fmll_ff_run();
	-# Удалить сеть с помощью функции fmll_ff_destroy().

	\sa activate.h, weight_init.h

\endlang

*/

#ifndef FMLL_FF_H
#define FMLL_FF_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/graph/graph.h"
	#include "math/random/random.h"

#endif

/* ############################################################################ */

/*! \cond HIDDEN_SYMBOLS */

typedef struct t_fmll_ff_neuro fmll_ff_neuro;

/*! \endcond */

/*!

\en

	\brief TRANSLATE

\ru

	\brief Описатель сети прямого распространения

\endlang

*/
typedef struct t_fmll_ff
{

	/*! \en TRANSLATE \ru Массив весов синапсов нейронов \endlang */
	double ** w;

	/*! \en TRANSLATE \ru Массив пороговых значений функций активации нейронов \endlang */
	double * b;

	/*! \en TRANSLATE \ru Матрица смежности нейронов сети \endlang */
	unsigned char ** connect;

	/*! \en TRANSLATE \ru Количество нейронов в сети \endlang */
	unsigned num;

	/*! \en TRANSLATE \ru Количество нейронов во входном слое сети \endlang */
	unsigned in_dim;

	/*! \en TRANSLATE \ru Количество нейронов в выходном слое сети \endlang */
	unsigned out_dim;

	/*! \cond HIDDEN_SYMBOLS */

	double (** fun)(double);
	double (** d_fun)(double);

	unsigned * in, * out;
	double * y;
	fmll_ff_neuro * neuro, ** in_neuro, ** out_neuro;
	
	/*! \endcond */

} fmll_ff;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param num - TRANSLATE;
	\param connect - TRANSLATE;
	\param fun - TRANSLATE;
	\param d_fun - TRANSLATE.

	\return TRANSLATE;
	\return TRANSLATE.

\ru

	\brief Создание сети прямого распространения

	\param num - количество нейронов;
	\param connect - матрица смежности нейронов сети (элемент [v][u] данной матрицы устанавливается в 1, если существует синапс из нейрона v в нейрон u, и в 0 в противном случае); матрица connect не должна содержать циклов; может принимать значение NULL - в этом случае матрица смежности будет инициализирована нулями.
	\param fun - массив функций активации нейронов (своя функция активации для каждого нейрона);
	\param d_fun - массив производных функций активации нейронов.

	Веса синапсов нейронов устанавливаются в ноль.

	\return указатель на описатель сети в случае ее успешного создания;
	\return NULL в случае неудачи.

\endlang

*/
fmll_ff * fmll_ff_init(unsigned num, unsigned char ** connect, double (** fun)(double), double (** d_fun)(double));

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE.

\ru

	\brief Удаление сети прямого распространения

	\param ff - указатель на описатель сети.

\endlang

*/
void fmll_ff_destroy(fmll_ff * ff);

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Проверка корректности матрицы смежности сети прямого распространения, поиск входных и выходных нейронов сети

	\param ff - указатель на описатель сети.

	Вызов данной функции должен осуществлятся после изменения матрицы смежности сети непосредственно в ее (сети) описателе.

	\return 0 - в случае успешного завершения операций проверки корректности матрицы смежности и поиска входных и выходных нейронов сети;
	\return <> 0 - в случае неудачного завершения операций проверки корректности матрицы смежности и поиска входных и выходных нейронов сети.

\endlang

*/
int fmll_ff_init_2(fmll_ff * ff);

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE;
	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Сохранение в XML-файл описателя сети прямого распространения

	\param ff - указатель на описатель сети;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя сети;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя сети.

\endlang

*/
int fmll_ff_save(fmll_ff * ff, const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param fname_prefix - TRANSLATE (TRANSLATE);
	\param fun - TRANSLATE (TRANSLATE);
	\param d_fun - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Загрузка из XML-файла описателя сети прямого распространения

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param fun - массив функций активации нейронов (своя функция активации для каждого из нейронов);
	\param d_fun - массив производных функций активации нейронов.

	\return указатель на описатель сети в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя сети.

\endlang

*/
fmll_ff * fmll_ff_load(const char * fname_prefix, double (** fun)(double), double (** d_fun)(double));

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE;
	\param vec - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Прогон сети прямого распространения над некоторым вектором

	\param ff - указатель на описатель сети;
	\param vec - некоторый вектор (размерность вектора должна совпадать с количеством входных нейронов сети).

	\return указатель на массив выходных значений сети (размерность вектора совпадает с количествов выходных нейронов сети).

\endlang

*/
const double * fmll_ff_run(fmll_ff * ff, const double * vec);

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE;
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

	\brief Тестирование сети прямого распространения

	\param ff - указатель на описатель сети;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов;
	\param deviation - допустимые отклонения для каждого из откликов;
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона сети над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

	Функция (* st_func) обладает следующими параметры:

	-# указатель на описатель сети;
	-# указатель на обрабатываемый вектор;
	-# указатель на вектор эталонных откликов, соответствующий обрабатываемому вектору;
	-# указатель на вектор выходов сети;
	-# количество векторов в тестовом множестве векторов;
	-# булева переменная, установленная в true, если вектор выходов сети равен вектору эталонных откликов с точностью до указанных допустимых отклонений, и установленная в false - в противном случае;
	-# значение параметра st_param.

	\return количество векторов из тестового множества векторов, вектор выходов сети для которых равен соответствующим векторам эталонных откликов с точностью до указанных допустимых отклонений.

\endlang

*/
unsigned fmll_ff_test(fmll_ff * ff, double ** vec, double ** d, double * deviation, unsigned vec_num,
		void (* st_func)(fmll_ff *, double *, double *, const double *, unsigned, bool, void *), void * st_param);

/*!

\en

	\brief TRANSLATE

	\param ff - TRANSLATE;
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

	\brief Обучение сети прямого распространения путем пакетного градиентного спуска с использованием алгоритма обратного распространения ошибки

	\param ff - указатель на описатель сети;
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
int fmll_ff_teach_gradient_batch(fmll_ff * ff, double ** vec, double ** d, unsigned vec_num,
		double beta_0, double (* next_beta)(double), double coef_moment, unsigned max_iter, double E_thres, double d_E_thres);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

