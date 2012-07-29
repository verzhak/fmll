
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Расстояния между векторами

\endlang

*/

#ifndef FMLL_DISTANCE_H
#define FMLL_DISTANCE_H

#ifdef FMLL_BUILD

	#include "all.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	\param vec_1 - TRANSLATE;
	\param vec_2 - TRANSLATE;
	\param dim - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Квадрат Эвклидова расстояния

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_square_euclid(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TRANSLATE

	\param vec_1 - TRANSLATE;
	\param vec_2 - TRANSLATE;
	\param dim - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Эвклидово расстояние

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_euclid(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TRANSLATE

	\param vec_1 - TRANSLATE;
	\param vec_2 - TRANSLATE;
	\param dim - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Скалярное произведение векторов, нормированное по их размерности

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_scalar(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TRANSLATE (TRANSLATE), TRANSLATE

	\param vec_1 - TRANSLATE;
	\param vec_2 - TRANSLATE;
	\param dim - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Манхэттенское расстояние (расстояние городских кварталов; мера относительно нормы \f$L1\f$), нормированное по размерности векторов

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_manhattan(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TRANSLATE (TRANSLATE), TRANSLATE

	\param vec_1 - TRANSLATE;
	\param vec_2 - TRANSLATE;
	\param dim - TRANSLATE.

	\return TRANSLATE.

\ru

	\brief Расстояние Чебышева (мера относительно нормы \f$L_{\infty}\f$), нормированное по размерности векторов

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_chebyshev(const double * vec_1, const double * vec_2, unsigned dim);

#ifdef __cplusplus
}
#endif

#endif

