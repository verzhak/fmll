
/*!

\file

\brief Ядра скалярных произведений векторов.

*/

#ifndef FMLL_KERNEL_H
#define FMLL_KERNEL_H

#ifdef FMLL_BUILD

#include "all.h"

#endif

// ############################################################################ 

/*!

\brief Однородный полином первой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = v_1^T v_2
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_homogeneous_1(const double * v1, const double * v2, unsigned dim);

/*!

\brief Однородный полином второй степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^2
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_homogeneous_2(const double * v1, const double * v2, unsigned dim);

/*!

\brief Однородный полином третьей степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^3
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_homogeneous_3(const double * v1, const double * v2, unsigned dim);

/*!

\brief Однородный полином четвертой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^4
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_homogeneous_4(const double * v1, const double * v2, unsigned dim);

// ############################################################################ 

/*!

\brief Неоднородный полином первой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = v_1^T v_2 + 1
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_inhomogeneous_1(const double * v1, const double * v2, unsigned dim);

/*!

\brief Неоднородный полином второй степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^2
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_inhomogeneous_2(const double * v1, const double * v2, unsigned dim);

/*!

\brief Неоднородный полином третьей степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^3
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_inhomogeneous_3(const double * v1, const double * v2, unsigned dim);

/*!

\brief Неоднородный полином четвертой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^4
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_polynomial_inhomogeneous_4(const double * v1, const double * v2, unsigned dim);

// ############################################################################ 

/*!

\brief Коэффициент \f$\sigma\f$ радиальной функции.

Значение по умолчанию: 1.

\sa fmll_kernel_radial.

*/
extern double fmll_kernel_radial_sigma;

/*!

\brief Радиальная функция.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = exp \left ( - \frac{1}{2\sigma^2} | v_1 - v_2 |^2 \right )
\f]

\return результат вычисления ядра скалярного произведения.

\sa fmll_kernel_radial_sigma.

*/
double fmll_kernel_radial(const double * v1, const double * v2, unsigned dim);

// ############################################################################ 

/*!

\brief Коэффициент \f$A\f$ гиперболического тангенса.

Значение по умолчанию: 1.

\sa fmll_kernel_tanh.

*/
extern double fmll_kernel_tanh_a;

/*!

\brief Коэффициент \f$B\f$ гиперболического тангенса.

Значение по умолчанию: 0.

\sa fmll_kernel_tanh.

*/
extern double fmll_kernel_tanh_b;

/*!

\brief Гиперболический тангенс.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = tanh \left ( A v_1^T v_2 + B \right )
\f]

\return результат вычисления ядра скалярного произведения.

\sa fmll_kernel_tanh_a, fmll_kernel_tanh_b.

*/
double fmll_kernel_tanh(const double * v1, const double * v2, unsigned dim);

// ############################################################################ 

/*!

\brief Пересечение.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v_1, v_2) = \sum_{i = 1}^{dim} \min(v_{1i}, v_{2i})
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_intersection(const double * v1, const double * v2, unsigned dim);

// ############################################################################ 

#endif

