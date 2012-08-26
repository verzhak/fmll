
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief Нейронная карта, самоорганизующаяся по принципу конкуренции

	Нейронная карта, самоорганизующаяся по принципу конкуренции, называемая также нейронной картой Кохонена по имени Тойво Кохонена, впервые описавшего один из возможных алгоритмов самоорганизации нейронной карты.

	Последовательность действий по использованию нейронной карты:

	-# создать карту с помощью функции fmll_som_init();
	-# иницилизировать веса синапсов нейронов карты с помощью одной из *_som_weight_init_* функций;
	-# обучить (самоорганизация) карту с помощью одной из *_som_so_* функций;
	-# прогнать карту над целевыми векторами с помощью функции fmll_som_run();
	-# удалить карту с помощью функции fmll_som_destroy().

\endlang

*/

#ifndef FMLL_SOM_H
#define FMLL_SOM_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/random/random.h"

#endif

/* ############################################################################ */

/*!

\en

	\brief TRANSLATE

\ru

	\brief Описатель нейронной карты

\endlang

*/
typedef struct t_fmll_som
{

	/*! \en TRANSLATE \ru Веса синапсов нейронов \endlang */
	double ** w;

	/*! \en TRANSLATE \ru Координаты нейронов \endlang */
	double ** coord;

	/*! \en TRANSLATE \ru Количество нейронов \endlang */
	unsigned num;

	/*! \en TRANSLATE \ru Размерность нейронной карты \endlang */
	unsigned map_dim;
	
	/*! \en TRANSLATE \ru Размерность классифицируемого векторного пространства \endlang */
	unsigned dim;

	/*! \en TRANSLATE \ru Указатель на функцию, расчитывающую расстояние между нейронами на карте нейронов \endlang */
	double (* distance_w)(const double *, const double *, unsigned);

	/*! \en TRANSLATE \ru Указатель на функцию, расчитывающую расстояние между векторами \endlang */
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

	\brief TRANSLATE

	\param som - TRANSLATE;
	\param gamma_mult - TRANSLATE;
	\param gamma_add - TRANSLATE;
	\param index_winner - TRANSLATE;
	\param index - TRANSLATE.

	\return 1 - TRANSLATE;
	\return 0 - TRANSLATE.

	\sa fmll_som_neighbor_radial().

\ru

	\brief Расчет по принципу WTA (Winner Take All - победитель получает все) коэффициента соседства очередного нейрона с нейроном - победителем
	
	\param som - указатель на описатель нейронной карты;
	\param gamma_mult - мультипликативный базовый коэффициент соседства;
	\param gamma_add - аддитивный базовый коэффициент соседства;
	\param index_winner - индекс нейрона - победителя;
	\param index - индекс нейрона, для которого вычисляется коэффициент соседства.

	\return 1 - для нейрона - победителя;
	\return 0 - для прочих нейронов.

	\sa fmll_som_neighbor_radial().

\endlang

*/
double fmll_som_neighbor_wta(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	\gamma_{mult} ~ exp \left( - \frac{distance^2_w(c_{winner}, c_{current})}{\gamma_{add}} \right )
	\f]
	
	TRANSLATE:

	- \f$\gamma_{mult}\f$ - TRANSLATE;
	- \f$\gamma_{add}\f$ - TRANSLATE;
	- \f$distance_w()\f$ - TRANSLATE;
	- \f$c_{winner}\f$ - TRANSLATE;
	- \f$c_{current}\f$ - TRANSLATE.

	\param som - TRANSLATE;
	\param gamma_mult - TRANSLATE;
	\param gamma_add - TRANSLATE;
	\param index_winner - TRANSLATE;
	\param index - TRANSLATE.

	\return TRANSLATE.

	\sa fmll_som_neighbor_wta().

\ru

	\brief Вычисление по радиальной функции коэффициента соседства очередного нейрона с нейроном - победителем

	Радиальная функция:

	\f[
	\gamma_{mult} ~ exp \left( - \frac{distance^2_w(c_{winner}, c_{current})}{\gamma_{add}} \right )
	\f]
	
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

	\sa fmll_som_neighbor_wta().

\endlang

*/
double fmll_som_neighbor_radial(fmll_som * som, double gamma_mult, double gamma_add, unsigned index_winner, unsigned index);

/* ############################################################################ */

/*!

\en

	\brief TRANSLATE

	\param N - TRANSLATE;
	\param map_dim - TRANSLATE;
	\param dim - TRANSLATE;
	\param distance_w - TRANSLATE;
	\param distance - TRANSLATE.

	TRANSLATE:

	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Создание нейронной карты

	\param N - массив размеров нейронной карты по каждой из ее размерностей;
	\param map_dim - размерность нейронной карты;
	\param dim - размерность классифицируемого векторного пространства;
	\param distance_w - указатель на функцию, вычисляющую расстояния между нейронами на нейронной карте;
	\param distance - указатель на функцию, вычисляющую расстояния между векторами в классифицируемом векторном пространстве.

	Функции distance_w() и distance() обладают следующими параметрами:

	-# указатель на первый вектор;
	-# указатель на второй вектор;
	-# размерность векторов.

	\return указатель на описатель нейронной карты в случае ее успешного создания;
	\return NULL - в случае неудачи.

\endlang

*/
fmll_som * fmll_som_init(const unsigned * N, unsigned map_dim, unsigned dim,
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned));

/*!

\en

	\brief TRANSLATE

	\param som - TRANSLATE.

\ru

	\brief Удаление нейронной карты

	\param som - указатель на описатель нейронной карты.

\endlang

*/
void fmll_som_destroy(fmll_som * som);

/*!

\en

	\brief TRANSLATE

	\param som - TRANSLATE;
	\param fname_prefix - TRANSLATE (TRANSLATE).

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

\ru

	\brief Сохранение в XML-файл описателя нейронной карты

	\param som - указатель на описатель нейронной карты;
	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

	\return 0 - в случае успешного сохранения описателя нейронной карты;
	\return <> 0 - в случае некорректного завершения операции сохранения описателя нейронной карты.

\endlang

*/
int fmll_som_save(fmll_som * som, const char * fname_prefix);

/*!

\en

	\brief TRANSLATE

	\param fname_prefix - TRANSLATE (TRANSLATE);
	\param distance_w - TRANSLATE;
	\param distance - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Загрузка из XML-файла описателя нейронной карты

	\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml);
	\param distance_w - указатель на функцию, вычисляющую расстояния между нейронами на нейронной карте;
	\param distance - указатель на функцию, вычисляющую расстояния между векторами в классифицируемом векторном пространстве.

	\return указатель на описатель нейронной карты в случае его успешной загрузки;
	\return NULL - в случае некорректного завершения операции загрузки описателя нейронной карты.

\endlang

*/
fmll_som * fmll_som_load(const char * fname_prefix,
		double (* distance_w)(const double *, const double *, unsigned), double (* distance)(const double *, const double *, unsigned));

/*!

\en

	\brief TRANSLATE

	\param som - TRANSLATE;
	\param vec - TRANSLATE.

	\return TRANSLATE.

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

	\brief TRANSLATE

	\param som - TRANSLATE;
	\param vec - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param beta_0 - TRANSLATE, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TRANSLATE;
	\param gamma_mult - TRANSLATE;
	\param gamma_add - TRANSLATE;
	\param neighbor - TRANSLATE.

	TRANSLATE:

	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE.

	TRANSLATE: \f$neighbor() ~ \to ~ 1\f$ TRANSLATE \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, TRANSLATE:

	- \f$distance_w()\f$ - TRANSLATE;
	- \f$c_{winner}\f$ - TRANSLATE;
	- \f$c_{current}\f$ - TRANSLATE.

	TRANSLATE.

	TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

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

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_som_so_kohonen(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned));

/*!

\en

	\brief TRANSLATE

	\param som - TRANSLATE;
	\param vec - TRANSLATE;
	\param vec_num - TRANSLATE;
	\param beta_0 - TRANSLATE, \f$\beta_0 ~ \in ~ [0, 1]\f$;
	\param next_beta - TRANSLATE;
	\param gamma_mult - TRANSLATE;
	\param gamma_add - TRANSLATE;
	\param neighbor - TRANSLATE;
	\param max_win - TRANSLATE;
	\param penalty - TRANSLATE.

	TRANSLATE:

	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE;
	-# TRANSLATE.

	TRANSLATE: \f$neighbor() ~ \to ~ 1\f$ TRANSLATE \f$distance_w(c_{winner}, c_{current}) ~ \to ~ 0\f$, TRANSLATE:

	- \f$distance_w()\f$ - TRANSLATE;
	- \f$c_{winner}\f$ - TRANSLATE;
	- \f$c_{current}\f$ - TRANSLATE.

	TRANSLATE.

	TRANSLATE.

	\return 0 - TRANSLATE;
	\return <> 0 - TRANSLATE.

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

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_som_so_kohonen_penalty(fmll_som * som, double ** vec, unsigned vec_num, double beta_0, double (* next_beta)(double),
		double gamma_mult, double gamma_add, double (* neighbor)(fmll_som *, double, double, unsigned, unsigned), unsigned max_win, unsigned penalty);

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

