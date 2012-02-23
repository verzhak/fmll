
/*!

\file

\en

	\brief TODO

\ru

	\brief Функции активации нейронов

\endlang

*/

#ifndef FMLL_ACTIVATE_H
#define FMLL_ACTIVATE_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/constant.h"

#endif

/*!

\en

	\brief TODO

	TODO.

\ru

	\brief Коэффициент A синуса

	Значение по умолчанию: 1.

\endlang

\sa fmll_sin, fmll_d_sin.

*/
extern double fmll_sin_a;

/*!

\en

	\brief TODO

	TODO.

\ru

	\brief Коэффициент B синуса

	Значение по умолчанию: 1.

\endlang

\sa fmll_sin, fmll_d_sin.

*/
extern double fmll_sin_b;

/*!

\en

	\brief TODO

	TODO.

\ru

	\brief Коэффициент A сигмоиды

	Значение по умолчанию: 1.

\endlang

\sa fmll_sigmoid, fmll_d_sigmoid.

*/
extern double fmll_sigmoid_a;

/*!

\en

	\brief TODO

	TODO.

\ru

	\brief Коэффициент A гиперболического тангенса

	Значение по умолчанию: 1.

\endlang

\sa fmll_tanh, fmll_d_tanh.

*/
extern double fmll_tanh_a;

/*!

\en

	\brief TODO

	TODO.

\ru

	\brief Коэффициент B гиперболического тангенса

	Значение по умолчанию: 1.

\endlang

\sa fmll_tanh, fmll_d_tanh.

*/
extern double fmll_tanh_b;

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO

	TODO:

\ru

	\brief Синус

	Вычисляется по формуле:

\endlang

\f[
	f(x) ~ = ~ A ~ \sin(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_sin(double x);

/*!

\en

	\brief TODO

	TODO:

\ru

	\brief Производная синуса

	Вычисляется по формуле:

\endlang

\f[
	f'(x) ~ = ~ A ~ B ~ \cos(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_d_sin(double x);

/*!

\en

	\brief TODO (TODO)

	TODO:

\ru

	\brief Сигмоида (логистическая функция)

	Вычисляется по формуле:

\endlang

\f[
	f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
\f]

\sa fmll_sigmoid_a

*/
double fmll_sigmoid(double x);

/*!

\en

	\brief TODO (TODO)

	TODO:

\ru

	\brief Производная сигмоиды (производная логистической функции)

	Вычисляется по формуле:

\endlang

\f[
	f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
\f]

\sa fmll_sigmoid_a

*/
double fmll_d_sigmoid(double x);

/*!

\en

	\brief TODO

	TODO:

\ru

	\brief Гиперболический тангенс

	Вычисляется по формуле:

\endlang

\f[
	f(x) ~ = ~ A ~ \tanh(Bx)
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_tanh(double x);

/*!

\en

	\brief TODO

	TODO:

\ru

	\brief Производная тангенса

	Вычисляется по формуле:

\endlang

\f[
	f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_d_tanh(double x);

/*!

\en

	\brief TODO

\ru

	\brief Прямая \f$y ~ = ~ x\f$

\endlang

*/
double fmll_line(double x);

/*!

\en

	\brief TODO

\ru

	\brief Коэффициент прямой \f$y ~ = ~ x\f$ (всегда единица)

\endlang

*/
double fmll_d_line(double x);

#ifdef __cplusplus
}
#endif

#endif

