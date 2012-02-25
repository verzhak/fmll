
/*!

\file

\en

	\brief TODO

	TODO

\ru

	\brief Нейронная карта, самоорганизующаяся по принципу конкуренции

	Нейронная карта, самоорганизующаяся по принципу конкуренции, называемая также нейронной картой Кохонена по имени Тойво Кохонена, впервые описавшего один из возможных алгоритмов самоорганизации нейронной карты.

	Последовательность действий по использованию нейронной карты:

		-# Создать карту с помощью функции fmll_som_init();
		-# Обучить (самоорганизация) карту с помощью одной из *_som_so_* функций;
		-# Прогнать карту над целевыми векторами с помощью функции fmll_som_run();
		-# Удалить карту с помощью функции fmll_som_destroy().

\endlang

*/

#ifndef FMLL_SOM_H
#define FMLL_SOM_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "ann/base/weight_init.h"

#endif

/* ############################################################################ */

/*!

\en

	\brief TODO

\ru

	\brief Описатель нейронной карты

\endlang

*/
typedef struct t_fmll_som
{

	/*! \en TODO \ru Веса синапсов нейронов \endlang */
	double ** w;

	/*! \en TODO \ru Координаты нейронов \endlang */
	double ** coord;

	/*! \en TODO \ru Количество нейронов \endlang */
	unsigned num;

	/*! \en TODO \ru Размерность нейронной карты \endlang */
	unsigned map_dim;
	
	/*! \en TODO \ru Размерность классифицируемого векторного пространства \endlang */
	unsigned dim;

	/*! \en TODO \ru Указатель на функцию, расчитывающую расстояние между нейронами на карте нейронов \endlang */
	double (* distance_w)(const double *, const double *, unsigned);

	/*! \en TODO \ru Указатель на функцию, расчитывающую расстояние между векторами \endlang */
	double (* distance)(const double *, const double *, unsigned);

	/*! \cond HIDDEN_SYMBOLS */

	unsigned * N;

	/*! \endcond */

} fmll_som;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param som - TODO;
	\param gamma_mult - TODO;
	\param gamma_add - TODO;
	\param index_winner - TODO;
	\param index - TODO.

	\return

		- 1 - TODO;
		- 0 - TODO.

\ru

	\brief Вычисление по принципу WTA (Winner Take All - победитель получает все) коэффициента соседства очередного нейрона с нейроном - победителем

	\param som - указатель на описатель нейронной карты;
	\param gamma_mult - мультипликативный базовый коэффициент соседства;
	\param gamma_add - аддитивный базовый коэффициент соседства;
	\param index_winner - индекс нейрона - победителя;
	\param index - индекс нейрона, для которого вычисляется коэффициент соседства.

	\return

		- 1 - для нейрона - победителя;
		- 0 - для прочих нейронов.

\endlang

\sa fmll_som_neighbor_radial().

*/
double fmll_som_neighbor_wta(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index);

/*!

\en

	\brief TODO

	TODO:

\ru

	\brief Вычисление по радиальной функции коэффициента соседства очередного нейрона с нейроном - победителем

	Радиальная функция:

\endlang

\f[
	\gamma_{mult} ~ exp \left( - \frac{distance^2_w(c_{winner}, c_{current})}{\gamma_{add}} \right )
\f]

\en

	TODO:

	- \f$\gamma_{mult}\f$ - TODO;
	- \f$\gamma_{add}\f$ - TODO;
	- \f$distance_w()\f$ - TODO;
	- \f$c_{winner}\f$ - TODO;
	- \f$c_{current}\f$ - TODO.

	\param som - TODO;
	\param gamma_mult - TODO;
	\param gamma_add - TODO;
	\param index_winner - TODO;
	\param index - TODO.

	\return TODO.

\ru

	где:

		- \f$\gamma_{mult}\f$ - мультипликативный базовый коэффициент соседства;
		- \f$\gamma_{add}\f$ - аддитивный базовый коэффициент соседства;
		- \f$distance_w()\f$ - функция расстояния между нейронами на карте нейронов;
		- \f$c_{winner}\f$ - координаты нейрона - победителя;
		- \f$c_{current}\f$ - координаты очередного нейрона.

	\param som - указатель на описатель нейронной карты;
	\param gamma_mult - мультипликативный базовый коэффициент соседства;
	\param gamma_add - аддитивный базовый коэффициент соседства;
	\param index_winner - индекс нейрона - победителя;
	\param index - индекс нейрона, для которого вычисляется коэффициент соседства.

	\return коэффициент соседства для данного нейрона.

\endlang

\sa fmll_som_neighbor_wta().

*/
double fmll_som_neighbor_radial(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index);

/* ############################################################################ */

/*!

\en

	\brief TODO

	\param N - TODO;
	\param map_dim - TODO;
	\param dim - TODO;
	\param weight_init - TODO;
	\param rnd - TODO (TODO);
	\param distance_w - TODO;
	\param distance - TODO.

	TODO:

		-# TODO;
		-# TODO;
		-# TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Создание нейронной карты

	\param N - массив размеров нейронной карты по каждой из ее размерностей;
	\param map_dim - размерность нейронной карты;
	\param dim - размерность классифицируемого векторного пространства;
	\param weight_init - указатель на функцию, инициализирующую веса синапсов нейронов;
	\param rnd - указатель на описатель датчика (псевдо) случайных чисел (настоящий указатель передается как параметр в функцию (* weight_init));
	\param distance_w - указатель на функцию, вычисляющую расстояния между нейронами на нейронной карте;
	\param distance - указатель на функцию, вычисляющую расстояния между векторами в классифицируемом векторном пространстве.

	Функции distance_w() и distance() обладают следующими параметрами:

		-# указатель на первый вектор;
		-# указатель на второй вектор;
		-# размерность векторов.

	\return

		- указатель на описатель нейронной карты в случае ее успешного создания;
		- NULL - в случае неудачи.

\endlang

*/
fmll_som * fmll_som_init(const unsigned * N, unsigned map_dim, unsigned dim, double (* weight_init)(fmll_random *), fmll_random * rnd,
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned));

/*!

\en

	\brief TODO

	\param som - TODO.

\ru

	\brief Удаление нейронной карты

	\param som - указатель на описатель нейронной карты.

\endlang

*/
void fmll_som_destroy(fmll_som * som);

/*!

\en

	\brief TODO

	\param som - TODO;
	\param fname_prefix - TODO (TODO).

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Сохранение в XML-файл описателя нейронной карты

	\param som - указатель на описатель нейронной карты;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return

		- 0 - в случае успешного сохранения описателя нейронной карты;
		- <> 0 - в случае некорректного завершения операции сохранения описателя нейронной карты.

\endlang

*/
int fmll_som_save(fmll_som * som, const char * fname_prefix);

/*!

\en

	\brief TODO

	\param fname_prefix - TODO (TODO);
	\param distance_w - TODO;
	\param distance - TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Загрузка из XML-файла описателя нейронной карты

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param distance_w - указатель на функцию, вычисляющую расстояния между нейронами на нейронной карте;
	\param distance - указатель на функцию, вычисляющую расстояния между векторами в классифицируемом векторном пространстве.

	\return

		- указатель на описатель нейронной карты в случае его успешной загрузки;
		- NULL - в случае некорректного завершения операции загрузки описателя нейронной карты.

\endlang

*/
fmll_som * fmll_som_load(const char * fname_prefix,
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned));

/*!

\en

	\brief TODO

	\param som - TODO;
	\param vec - TODO.

	\return TODO.

\ru

	\brief Прогон нейронной карты над некоторым вектором

	\param som - указатель на описатель нейронной карты;
	\param vec - некоторый вектор.

	\return индекс нейрона - победителя.

\endlang

*/
unsigned fmll_som_run(fmll_som * som, const double * vec);

/*!

\en

	\brief TODO

	\param som - TODO;
	\param vec - TODO;
	\param vec_num - TODO;
	\param beta_0 - TODO, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TODO;
	\param gamma_mult - TODO;
	\param gamma_add - TODO;
	\param neighbor - TODO.

	TODO:

		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO.

	TODO: \f$neighbor() ~ \to ~ 1\f$ TODO \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, TODO:

		- \f$distance_w()\f$ - TODO;
		- \f$c_{winner}\f$ - TODO;
		- \f$c_{current}\f$ - TODO.

	TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Самоорганизация нейронной карты по алгоритму Кохонена

	\param som - указатель на описатель карты;
	\param vec - массив обучающих векторов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение коэффициента скорости обучения в начале каждой итерации обучения по значению коэффициента на предыдущей итерации;
	\param gamma_mult - мультипликативный базовый коэффициент соседства;
	\param gamma_add - аддитивный базовый коэффициент соседства;
	\param neighbor - указатель на функцию, рассчитывающую коэффициент соседства нейронов.

	Функция neighbor() обладает следующими параметрами:

		-# указатель на описатель нейронной карты;
		-# мультипликативный базовый коэффициент соседства;
		-# аддитивный базовый коэффициент соседства;
		-# индекс нейрона - победителя;
		-# индекс нейрона, для которого выполняется расчет коэффициента соседства.

	Функция neighbor() должна возвращать вещественное число из диапазона [0, 1], причем ее значение должно быть нормированным - то есть: \f$neighbor() ~ \to ~ 1\f$ при \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, где:

		- \f$distance_w()\f$ - функция расстояния между нейронами на карте нейронов;
		- \f$c_{winner}\f$ - координаты нейрона - победителя;
		- \f$c_{current}\f$ - координаты очередного нейрона.

	Аддитивный базовый коэффициент соседства должен быть не меньше 0, если в качестве функции, рассчитывающей коэффициент соседства нейронов, выбрана радиальная функция fmll_som_neighbor_radial().

	Мультипликативный базовый коэффициент соседства должен лежать в диапазоне (0, 1], если в качестве функции, рассчитывающей коэффициент соседства нейронов, выбрана радиальная функция fmll_som_neighbor_radial().

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_som_so_kohonen(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned));

/*!

\en

	\brief TODO

	\param som - TODO;
	\param vec - TODO;
	\param vec_num - TODO;
	\param beta_0 - TODO, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TODO;
	\param gamma_mult - TODO;
	\param gamma_add - TODO;
	\param neighbor - TODO;
	\param max_win - TODO;
	\param penalty - TODO.

	TODO:

		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO;
		-# TODO.

	TODO: \f$neighbor() ~ \to ~ 1\f$ TODO \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, TODO:

		- \f$distance_w()\f$ - TODO;
		- \f$c_{winner}\f$ - TODO;
		- \f$c_{current}\f$ - TODO.

	TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Самоорганизация нейронной карты по алгоритму Кохонена со штрафом переобучающихся нейронов

	\param som - указатель на описатель карты;
	\param vec - массив обучающих векторов;
	\param vec_num - количество векторов в массиве обучающих векторов;
	\param beta_0 - начальное значение коэффициента скорости обучения, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - указатель на функцию, пересчитывающую значение коэффициента скорости обучения в начале каждой итерации обучения по значению коэффициента на предыдущей итерации;
	\param gamma_mult - мультипликативный базовый коэффициент соседства;
	\param gamma_add - аддитивный базовый коэффициент соседства;
	\param neighbor - указатель на функцию, рассчитывающую коэффициент соседства нейронов;
	\param max_win - максимальное количество "побед" нейрона, после которого на него накладывается штраф;
	\param penalty - штраф, накладываемый на нейрон - количество векторов из классифицируемого множества векторов, при прогоне над которыми нейрон не будет приниматься во внимание.

	Функция neighbor() обладает следующими параметрами:

		-# указатель на описатель нейронной карты;
		-# мультипликативный базовый коэффициент соседства;
		-# аддитивный базовый коэффициент соседства;
		-# индекс нейрона - победителя;
		-# индекс нейрона, для которого выполняется расчет коэффициента соседства.

	Функция neighbor() должна возвращать вещественное число из диапазона [0, 1], причем ее значение должно быть нормированным - то есть: \f$neighbor() ~ \to ~ 1\f$ при \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, где:

		- \f$distance_w()\f$ - функция расстояния между нейронами на карте нейронов;
		- \f$c_{winner}\f$ - координаты нейрона - победителя;
		- \f$c_{current}\f$ - координаты очередного нейрона.

	Аддитивный базовый коэффициент соседства должен быть не меньше 0, если в качестве функции, рассчитывающей коэффициент соседства нейронов, выбрана радиальная функция fmll_som_neighbor_radial().

	Мультипликативный базовый коэффициент соседства должен лежать в диапазоне (0, 1], если в качестве функции, рассчитывающей коэффициент соседства нейронов, выбрана радиальная функция fmll_som_neighbor_radial().

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_som_so_kohonen_penalty(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned), unsigned max_win, unsigned penalty);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

