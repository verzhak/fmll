
/*!

\file

\en
	
	\brief TODO

\ru

	\brief Ядра скалярных произведений векторов

\endlang

*/

#ifndef FMLL_KERNEL_H
#define FMLL_KERNEL_H

#ifdef FMLL_BUILD

	#include "all.h"

#endif

/* ----------------------------------------------------------------------------  */

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Однородный полином первой степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = v_1^T v_2
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_homogeneous_1(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Однородный полином второй степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^2
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_homogeneous_2(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Однородный полином третьей степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^3
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_homogeneous_3(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Однородный полином четвертой степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 \right )^4
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_homogeneous_4(const double * v1, const double * v2, unsigned dim);

/* ----------------------------------------------------------------------------  */

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Неоднородный полином первой степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = v_1^T v_2 + 1
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_inhomogeneous_1(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Неоднородный полином второй степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^2
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_inhomogeneous_2(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Неоднородный полином третьей степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^3
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_inhomogeneous_3(const double * v1, const double * v2, unsigned dim);

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Неоднородный полином четвертой степени

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \left ( v_1^T v_2 + 1 \right )^4
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_polynomial_inhomogeneous_4(const double * v1, const double * v2, unsigned dim);

/* ----------------------------------------------------------------------------  */

/*!

\en

	\brief TODO

	TODO: 1.

\ru

	\brief Коэффициент \f$\sigma\f$ радиальной функции

	Значение по умолчанию: 1.

\endlang

\sa fmll_kernel_radial.

*/
extern double fmll_kernel_radial_sigma;

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Радиальная функция

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = exp \left ( - \frac{1}{2\sigma^2} | v_1 - v_2 |^2 \right )
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

\sa fmll_kernel_radial_sigma.

*/
double fmll_kernel_radial(const double * v1, const double * v2, unsigned dim);

/* ----------------------------------------------------------------------------  */

/*!

\en

	\brief TODO

	TODO: 1.

\ru

	\brief Коэффициент \f$A\f$ гиперболического тангенса

	Значение по умолчанию: 1.

\endlang

\sa fmll_kernel_tanh.

*/
extern double fmll_kernel_tanh_a;

/*!

\en

	\brief TODO

	TODO: 0.

\ru

	\brief Коэффициент \f$B\f$ гиперболического тангенса

	Значение по умолчанию: 0.

\endlang

\sa fmll_kernel_tanh.

*/
extern double fmll_kernel_tanh_b;

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Гиперболический тангенс

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = tanh \left ( A v_1^T v_2 + B \right )
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

\sa fmll_kernel_tanh_a, fmll_kernel_tanh_b.

*/
double fmll_kernel_tanh(const double * v1, const double * v2, unsigned dim);

/* ----------------------------------------------------------------------------  */

/*!

\en

	\brief TODO

	\param v1 - TODO;
	\param v2 - TODO;
	\param dim - TODO.

	TODO:

\ru

	\brief Пересечение

	\param v1 - первый вектор;
	\param v2 - второй вектор;
	\param dim - размерность векторного пространства.

	Ядро скалярного произведения вычисляется по формуле:

\endlang

\f[
	K(v_1, v_2) = \sum_{i = 1}^{dim} \min(v_{1i}, v_{2i})
\f]

\en

	\return TODO.

\ru

	\return результат вычисления ядра скалярного произведения.

\endlang

*/
double fmll_kernel_intersection(const double * v1, const double * v2, unsigned dim);

#ifdef __cplusplus
}
#endif

/* ----------------------------------------------------------------------------  */

#endif

