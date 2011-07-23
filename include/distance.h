
/*!

\file

\brief Вычисление по различным алгоритмам расстояний между векторами.

*/

#ifndef DISTANCE_H
#define DISTANCE_H

#include "all.h"

/*!

\brief Квадрат Эвклидова расстояния.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_square_euclid(const double *v1, const double *v2, uint8_t dim);

/*!

\brief Эвклидово расстояние.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_euclid(const double *v1, const double *v2, uint8_t dim);

/*!

\brief Скалярное произведение векторов, нормированное по их размерности.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_scalar(const double *v1, const double *v2, uint8_t dim);

/*!

\brief Манхэттенское расстояние (расстояние городских кварталов; мера относительно нормы \f$L1\f$), нормированное по размерности векторов.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_manhattan(const double *v1, const double *v2, uint8_t dim);

/*!

\brief Расстояние Чебышева (мера относительно нормы \f$L_{\infty}\f$), нормированное по размерности векторов.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторов.

\return расстояние между векторами.

*/
double fmll_distance_chebyshev(const double *v1, const double *v2, uint8_t dim);

#endif

