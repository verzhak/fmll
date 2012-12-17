
/*!

\file

\en

	\brief Vectors distances

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

	\brief Square of Euclidean distance

	\param vec_1 - first vector;
	\param vec_2 - second vector;
	\param dim - vector dimension.

	\return distance between vectors.

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

	\brief Euclidean distance

	\param vec_1 - first vector;
	\param vec_2 - second vector;
	\param dim - vector dimension.

	\return distance between vectors.

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

	\brief Scalar product of vectors, normalize by his dimension

	\param vec_1 - first vector;
	\param vec_2 - second vector;
	\param dim - vector dimension.

	\return distance between vectors.

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

	\brief Manhattan distance (city block distance; \f$L1\f$ distance) between vectors, normalize by his dimension

	\param vec_1 - first vector;
	\param vec_2 - second vector;
	\param dim - vector dimension.

	\return distance between vectors.

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

	\brief Chebyshev distance (\f$L_{\infty}\f$ distance) between vectors, normalize by his dimension

	\param vec_1 - first vector;
	\param vec_2 - second vector;
	\param dim - vector dimension.

	\return distance between vectors.

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

