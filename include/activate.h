
/*!

\file

\brief Функции активации нейронов.

*/

#ifndef ACTIVATE_H
#define ACTIVATE_H

#include "all.h"

/*!

\brief Коэффициент A синуса.

Значение по умолчанию: 1.

\sa fmll_sin, fmll_d_sin.

*/
extern double fmll_sin_a;

/*!

\brief Коэффициент B синуса.

Значение по умолчанию: 1.

\sa fmll_sin, fmll_d_sin.

*/
extern double fmll_sin_b;

/*!

\brief Коэффициент A сигмоиды.

Значение по умолчанию: 1.

\sa fmll_sigmoid, fmll_d_sigmoid.

*/
extern double fmll_sigmoid_a;

/*!

\brief Коэффициент A гиперболического тангенса.

Значение по умолчанию: 1.

\sa fmll_tanh, fmll_d_tanh.

*/
extern double fmll_tanh_a;

/*!

\brief Коэффициент B гиперболического тангенса.

Значение по умолчанию: 1.

\sa fmll_tanh, fmll_d_tanh.

*/
extern double fmll_tanh_b;

/*!

\brief Синус.

Вычисляется по формуле:

\f[
	f(x) ~ = ~ A ~ \sin(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_sin(double x);

/*!

\brief Производная синуса.

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ A ~ B ~ \cos(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_d_sin(double x);

/*!

\brief Сигмоида (логистическая функция).

Вычисляется по формуле:

\f[
	f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
\f]

\sa fmll_sigmoid_a

*/
double fmll_sigmoid(double x);

/*!

\brief Производная сигмоиды (производная логистической функции).

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
\f]

\sa fmll_sigmoid_a

*/
double fmll_d_sigmoid(double x);

/*!

\brief Гиперболический тангенс.

Вычисляется по формуле:

\f[
	f(x) ~ = ~ A ~ \tanh(Bx)
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_tanh(double x);

/*!

\brief Производная тангенса.

Вычисляется по формуле:

\f[
	f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_d_tanh(double x);

/*!

\brief Прямая \f$y ~ = ~ x\f$.

*/
double fmll_line(double x);

/*!

\brief Коэффициент прямой \f$y ~ = ~ x\f$ (всегда единица).

*/
double fmll_d_line(double x);

#endif

