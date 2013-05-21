
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief Нейронная сеть, состоящая из нескольких машин опорных векторов (SVM; Support Vector Machine)

	Последовательность действий по использованию нейронной сети, состоящей из нескольких машин опорных векторов:

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

	\brief TRANSLATE

\ru

	\brief Описатель нейронной сети, состоящей из нескольких машин опорных векторов

\endlang

*/
typedef struct t_fmll_svm_net
{

	/*! \en TRANSLATE \ru Количество машин опорных векторов в сети \endlang */
	unsigned num;

	/*! \en TRANSLATE \ru Массив указателей на описатели машин опорных векторов \endlang */
	fmll_svm ** svm;

} fmll_svm_net;

/* ############################################################################  */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param num - TRANSLATE;
	\param dim - TRANSLATE;
	\param K - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Создание нейронной сети, состоящей из нескольких машин опорных векторов

	\param num - количество машин опорных векторов;
	\param dim - размерность векторного пространства;
	\param K - массив указателей на функции, вычисляющие ядра скалярных произведений.

	\return указатель на описатель нейронной сети, состоящей из нескольких машин опорных векторов, в случае его успешного создания;
	\return NULL - в случае неудачи.

\endlang

*/
fmll_svm_net * fmll_svm_net_init(const unsigned num, const unsigned dim, double (** K)(const double *, const double *, const unsigned));

/*!

\en

	\brief TRANSLATE

	\param svm_net - TRANSLATE.

\ru

	\brief Удаление нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети, состоящей из нескольких машин опорных векторов.

\endlang

*/
void fmll_svm_net_destroy(fmll_svm_net * svm_net);

/*!

\en

	\brief TRANSLATE

	\param svm_net - TRANSLATE;
	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Сохранение в XML-файл описателя нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя нейронной сети;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя нейронной сети.

\endlang

*/
int fmll_svm_net_save(const fmll_svm_net * svm_net, const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param fname_prefix - TRANSLATE;
	\param K - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Загрузка из XML-файла описателя нейронной сети, состоящей из нескольких машин опорных векторов

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param K - массив указателей на функции, вычисляющие ядра скалярных произведений.

	\return указатель на описатель нейронной сети в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя нейронной сети.

\endlang

*/
fmll_svm_net * fmll_svm_net_load(const char * fname_prefix, double (** K)(const double *, const double *, const unsigned));

/*!

\en

	\brief TRANSLATE

	\param svm_net - TRANSLATE;
	\param vec - TRANSLATE;
	\param y - TRANSLATE.

	TRANSLATE.

	\return TRANSLATE;
	\return -1 - TRANSLATE;
	\return -2 - TRANSLATE;
	\return < -2 - TRANSLATE.

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

	\brief TRANSLATE

	\param svm_net - TRANSLATE;
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
	-# TRANSLATE;
	-# TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Тестирование нейронной сети, состоящей из нескольких машин опорных векторов

	\param svm_net - указатель на описатель нейронной сети;
	\param vec - тестовое множество векторов;
	\param d - множество эталонных откликов;
	\param vec_num - количество векторов в тестовом множестве векторов;
	\param st_func - указатель на функцию, вызываемую после прогона нейронной сети над каждым вектором из тестового множества (может принимать значение NULL);
	\param st_param - один из параметров функции (* st_func)() (если параметр st_func установлен в значение NULL, параметр st_param не используется).

	Функция (* st_func) обладает следующими параметрами:

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
unsigned fmll_svm_net_test(fmll_svm_net * svm_net, const double ** vec, const unsigned * d, const unsigned vec_num,
		void (* st_func)(const fmll_svm_net *, const double *, const unsigned, const int, const double *, const unsigned, const bool, void *), void * st_param);

/*!

\en

	\brief TRANSLATE

	\param svm_net - TRANSLATE;
	\param vec - TRANSLATE;
	\param d - TRANSLATE (TRANSLATE);
	\param vec_num - TRANSLATE;
	\param C - TRANSLATE;
	\param tau - TRANSLATE;
	\param selector - TRANSLATE;
	\param max_iter - TRANSLATE;
	\param epsilon - TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

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
int fmll_svm_net_teach_smo(fmll_svm_net * svm_net, const double ** vec, const unsigned * d, const unsigned vec_num, const double * C, const double * tau,
		int (** selector)(const fmll_svm *, const double **, const char *, const unsigned, int *, int *, const double, const double, const double, const double *, const double *, const double **),
		const unsigned * max_iter, const double * epsilon);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

