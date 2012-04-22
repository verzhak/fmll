
/*!

\file

\en

	\brief TODO

	TODO.

\ru

	\brief Нейронная сеть, состоящая из нескольких машин опорных векторов (SVM; Support Vector Machine)

	Последовательность действий по использованию нейронной сети, состоящей из нескольких машин опорных векторов:
	.
	-# Создать нейронную сеть с помощью функции fmll_svm_net_init();
	-# Обучить нейронную сеть с помощью одной из *_svm_net_teach_* функций;
	-# Прогнать нейронную сеть над целевыми векторами с помощью функции fmll_svm_net_run();
	-# Удалить нейронную сеть с помощью функции fmll_svm_net_destroy().

\endlang

*/

#ifndef FMLL_SVM_NET_H
#define FMLL_SVM_NET_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "ann/svm/svm.h"

#endif

/* ############################################################################ */

/*!

\en

	\brief TODO

\ru

	\brief Описатель нейронной сети, состоящей из нескольких машин опорных векторов

\endlang

*/
typedef struct t_fmll_svm_net
{

	/*! \en TODO \ru Количество машин опорных векторов в сети \endlang */
	unsigned num;

	/*! \en TODO \ru Массив указателей на описатели машин опорных векторов \endlang */
	fmll_svm ** svm;

} fmll_svm_net;

/* ############################################################################  */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param num - TODO;
	\param dim - TODO;
	\param K - TODO.

	\return TODO;
	\return NULL - TODO.

\ru

	\brief Создание нейронной сети, состоящей из нескольких машин опорных векторов

	\param num - количество машин опорных векторов;
	\param dim - размерность векторного пространства;
	\param K - массив указателей на функции, вычисляющие ядра скалярных произведений.

	\return указатель на описатель нейронной сети, состоящей из нескольких машин опорных векторов, в случае его успешного создания;
	\return NULL - в случае неудачи.

\endlang

*/
fmll_svm_net * fmll_svm_net_init(unsigned num, unsigned dim, double (** K)(const double *, const double *, unsigned));

/*!

\en

	\brief TODO

	\param svm_net - TODO.

\ru

	\brief Удаление нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети, состоящей из нескольких машин опорных векторов.

\endlang

*/
void fmll_svm_net_destroy(fmll_svm_net * svm_net);

/*!

\en

	\brief TODO

	\param svm_net - TODO;
	\param fname_prefix - TODO (TODO).

	\return 0 - TODO;
	\return <> 0 - TODO.

\ru

	\brief Сохранение в XML-файл описателя нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя нейронной сети;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя нейронной сети.

\endlang

*/
int fmll_svm_net_save(fmll_svm_net * svm_net, const char * fname_prefix);

/*!

\en

	\brief TODO

	\param fname_prefix - TODO;
	\param K - TODO.

	\return TODO;
	\return NULL - TODO.

\ru

	\brief Загрузка из XML-файла описателя нейронной сети, состоящей из нескольких машин опорных векторов

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param K - массив указателей на функции, вычисляющие ядра скалярных произведений.

	\return указатель на описатель нейронной сети в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя нейронной сети.

\endlang

*/
fmll_svm_net * fmll_svm_net_load(const char * fname_prefix, double (** K)(const double *, const double *, unsigned));

/*!

\en

	\brief TODO

	\param svm_net - TODO;
	\param vec - TODO;
	\param y - TODO.

	TODO.

	\return TODO;
	\return -1 - TODO;
	\return -2 - TODO;
	\return < -2 - TODO.

\ru

	\brief Прогон нейронной сети, состоящей из нескольких машин опорных векторов, над вектором

	\param svm_net - указатель на описатель нейронной сети;
	\param vec - указатель на вектор;
	\param y - указатель на массив, в который будут записаны результаты прогона каждой из SVM, составляющих нейронную сеть, над целевым вектором.

	Параметр y может принимать значение NULL.

	\return номер машины опорных векторов, давшей результат, равный 1, если таковая машина опорных векторов одна;
	\return -1 - если ни одна из машин опорных векторов не дала результат, равный 1;
	\return -2 - если несколько машин опорных векторов дали результат, равный 1;
	\return < -2 - в случае невозможности корректно завершить прогон машины опорных векторов над целевым вектором.

\endlang

*/
int fmll_svm_net_run(fmll_svm_net * svm_net, const double * vec, double * y);

/*!

\en

	\brief TODO

	\param svm_net - TODO;
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
	-# TODO;
	-# TODO.

	\return TODO.

\ru

	\brief Тестирование нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона нейронной сети над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

	Функция (* st_func) обладает следующими параметрами:
	.
	-# указатель на описатель нейронной сети;
	-# указатель на обрабатываемый вектор;
	-# номер класса, к которому относится вектор;
	-# выход нейронной сети;
	-# указатель на вектор выходов машин опорных векторов, составляющих нейронную сеть;
	-# количество векторов в тестовом множестве векторов;
	-# булева переменная, установленная в true, если обрабатываемый	вектор классифицирован правильно, и установленная в false - в противном случае;
	-# значение параметра st_param.

	\return количество векторов из тестового множества векторов, классифицированных правильно.

\endlang

*/
unsigned fmll_svm_net_test(fmll_svm_net * svm_net, double ** vec, unsigned * d, unsigned vec_num,
		void (* st_func)(fmll_svm_net *, double *, unsigned, int, const double *, unsigned, bool, void *), void * st_param);

/*!

\en

	\brief TODO

	\param svm_net - TODO;
	\param vec - TODO;
	\param d - TODO (TODO);
	\param vec_num - TODO;
	\param C - TODO;
	\param tau - TODO;
	\param selector - TODO;
	\param max_iter - TODO;
	\param epsilon - TODO.

	\return 0 - TODO;
	\return <> 0 - TODO.

	\sa fmll_svm_teach_smo.

\ru

	\brief Обучение нейронной сети, состоящей из нескольких машин опорных векторов, по алгоритму Sequential Minimal Optimization (SMO)

	\param svm_net - указатель на описатель нейронной сети;
	\param vec - множество обучающих векторов;
	\param d - номера классов обучающих векторов ([0, num - 1], где num - количество машин опорных векторов в сети);
	\param vec_num - мощность множества обучающих векторов;
	\param C - массив ширины полос поиска коэффициентов \f$\lambda\f$;
	\param tau - массив малых положительных чисел, в некоторых эвристиках используемых при выборе второго из множителей Лагранжа - кандидатов на оптимизацию;
	\param selector - массив указателей на функции, выбирающих пару множителей Лагранжа - кандидатов на оптимизацию на очередной итерации алгоритма;
	\param max_iter - массив максимальных количеств итераций алгоритма обучения;
	\param epsilon - массив малых положительных чисел, при достижении которых разностью частных производных функции Лагранжа, умноженных на соответствующие эталонные отклики, взятые с обратным знаком, очередной пары множителей Лагранжа - кандидатов на оптимизацию - процесс обучения данной SVM будет остановлен.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

	\sa fmll_svm_teach_smo.

\endlang

*/
int fmll_svm_net_teach_smo(fmll_svm_net * svm_net, double ** vec, unsigned * d, unsigned vec_num, double * C, double * tau,
		int (** selector)(fmll_svm *, double **, char *, unsigned, int *, int *, double, double, double, double *, double *, double **),
		unsigned * max_iter, double * epsilon);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

