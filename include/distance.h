
/*!

\file

\en

	\brief TODO.

\ru

	\brief Расстояния между векторами.

\endlang

*/

#ifndef FMLL_DISTANCE_H
#define FMLL_DISTANCE_H

#ifdef FMLL_BUILD

#include "all.h"

#endif

/*!

\en

	\brief TODO.

	\param vec_1 - TODO;
	\param vec_2 - TODO;
	\param dim - TODO.

	\return TODO.

\ru

	\brief Квадрат Эвклидова расстояния.

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_square_euclid(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TODO.

	\param vec_1 - TODO;
	\param vec_2 - TODO;
	\param dim - TODO.

	\return TODO.

\ru

	\brief Эвклидово расстояние.

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_euclid(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TODO.

	\param vec_1 - TODO;
	\param vec_2 - TODO;
	\param dim - TODO.

	\return TODO.

\ru

	\brief Скалярное произведение векторов, нормированное по их размерности.

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_scalar(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TODO (TODO), TODO.

	\param vec_1 - TODO;
	\param vec_2 - TODO;
	\param dim - TODO.

	\return TODO.

\ru

	\brief Манхэттенское расстояние (расстояние городских кварталов; мера относительно нормы \f$L1\f$), нормированное по размерности векторов.

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_manhattan(const double * vec_1, const double * vec_2, unsigned dim);

/*!

\en

	\brief TODO (TODO), TODO.

	\param vec_1 - TODO;
	\param vec_2 - TODO;
	\param dim - TODO.

	\return TODO.

\ru

	\brief Расстояние Чебышева (мера относительно нормы \f$L_{\infty}\f$), нормированное по размерности векторов.

	\param vec_1 - первый вектор;
	\param vec_2 - второй вектор;
	\param dim - размерность векторов.

	\return расстояние между векторами.

\endlang

*/
double fmll_distance_chebyshev(const double * vec_1, const double * vec_2, unsigned dim);

#endif

