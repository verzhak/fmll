
/*!

\file

\brief Ядра скалярных произведений векторов.

*/

#ifndef KERNEL_H
#define KERNEL_H

#include "all.h"

/*!

\brief Полином первой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v1, v2) = v_1^T v_2 + 1
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_scalar_1(const double * v1, const double * v2, unsigned dim);

/*!

\brief Полином второй степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v1, v2) = \left ( v_1^T v_2 + 1 \right )^2
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_scalar_2(const double * v1, const double * v2, unsigned dim);

/*!

\brief Полином третьей степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v1, v2) = \left ( v_1^T v_2 + 1 \right )^3
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_scalar_3(const double * v1, const double * v2, unsigned dim);

/*!

\brief Полином четвертой степени.

\param v1 - первый вектор;
\param v2 - второй вектор;
\param dim - размерность векторного пространства.

Ядро скалярного произведения вычисляется по формуле:

\f[
	K(v1, v2) = \left ( v_1^T v_2 + 1 \right )^4
\f]

\return результат вычисления ядра скалярного произведения.

*/
double fmll_kernel_scalar_4(const double * v1, const double * v2, unsigned dim);

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
	K(v1, v2) = exp \left ( - \frac{1}{2\sigma^2} | v_1 - v_2 |^2 \right )
\f]

\return результат вычисления ядра скалярного произведения.

\sa fmll_kernel_radial_sigma.

*/
double fmll_kernel_radial(const double * v1, const double * v2, unsigned dim);

/*!

\brief Коэффициент \f$A\f$ гиперболического тангенса.

Значение по умолчанию: 1.

\sa fmll_kernel_tanh.

*/
extern double fmll_kernel_tanh_a;

/*!

\brief Коэффициент \f$B\f$ гиперболического тангенса.

Значение по умолчанию: 1.

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
	K(v1, v2) = tanh \left ( A v_1^T v_2 + B \right )
\f]

\return результат вычисления ядра скалярного произведения.

\sa fmll_kernel_tanh_a, fmll_kernel_tanh_b.

*/
double fmll_kernel_tanh(const double * v1, const double * v2, unsigned dim);

#endif

