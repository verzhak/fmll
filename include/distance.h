
/*!

\file

\brief Вычисление по различным алгоритмам расстояний между векторами.

*/

#ifndef DISTANCE_H
#define DISTANCE_H

#include "all.h"

/*!

\brief Квадрат Эвклидова расстояния.

\param vec_1 - первый вектор;
\param vec_2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_square_euclid(const double * vec_1, const double * vec_2, uint8_t dim);

/*!

\brief Эвклидово расстояние.

\param vec_1 - первый вектор;
\param vec_2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_euclid(const double * vec_1, const double * vec_2, uint8_t dim);

/*!

\brief Скалярное произведение векторов, нормированное по их размерности.

\param vec_1 - первый вектор;
\param vec_2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_scalar(const double * vec_1, const double * vec_2, uint8_t dim);

/*!

\brief Манхэттенское расстояние (расстояние городских кварталов; мера относительно нормы \f$L1\f$), нормированное по размерности векторов.

\param vec_1 - первый вектор;
\param vec_2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_manhattan(const double * vec_1, const double * vec_2, uint8_t dim);

/*!

\brief Расстояние Чебышева (мера относительно нормы \f$L_{\infty}\f$), нормированное по размерности векторов.

\param vec_1 - первый вектор;
\param vec_2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_chebyshev(const double * vec_1, const double * vec_2, uint8_t dim);

#endif

