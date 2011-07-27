
/*!

\file

\brief Функции активации нейронов.

*/

#ifndef ACTIVATE_H
#define ACTIVATE_H

#include "all.h"

/*! Коэффициент A синуса. */
extern double fmll_sin_a;

/*! Коэффициент B синуса. */
extern double fmll_sin_b;

/*! Коэффициент A сигмоиды. */
extern double fmll_sigmoid_a;

/*! Коэффициент A гиперболического тангенса. */
extern double fmll_atan_a;

/*! Коэффициент B гиперболического тангенса. */
extern double fmll_atan_b;

/*!

\brief Синус.

Вычисляется по формуле:

\f[
	f(x) ~ = ~ A ~ \sin(Bx)
\f]

*/
double fmll_sin(double x);

/*!

\brief Производная синуса.

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ A ~ B ~ \cos(Bx)
\f]

*/
double fmll_d_sin(double x);

/*!

\brief Сигмоида (логистическая функция).

Вычисляется по формуле:

\f[
	f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
\f]

*/
double fmll_sigmoid(double x);

/*!

\brief Производная сигмоиды (производная логистической функции).

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
\f]

*/
double fmll_d_sigmoid(double x);

/*!

\brief Арктангенс.

Вычисляется по формуле:

\f[
	f(x) ~ = ~ A ~ \tanh(Bx)
\f]

*/
double fmll_atan(double x);

/*!

\brief Производная арктангенса.

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
\f]

*/
double fmll_d_atan(double x);

/*!

\brief Прямая \f$y ~ = ~ x\f$.

*/
double fmll_line(double x);

/*!

\brief Коэффициент прямой \f$y ~ = ~ x\f$ (всегда единица).

*/
double fmll_d_line(double x);

#endif

