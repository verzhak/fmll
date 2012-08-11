
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Функции активации нейронов

\endlang

*/

#ifndef FMLL_ACTIVATE_H
#define FMLL_ACTIVATE_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/various/constant.h"

#endif

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_sin, fmll_d_sin.

\ru

	\brief Коэффициент A синуса

	Значение по умолчанию: 1.

	\sa fmll_sin, fmll_d_sin.

\endlang

*/
extern double fmll_sin_a;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_sin, fmll_d_sin.

\ru

	\brief Коэффициент B синуса

	Значение по умолчанию: 1.

	\sa fmll_sin, fmll_d_sin.

\endlang

*/
extern double fmll_sin_b;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_sigmoid, fmll_d_sigmoid.

\ru

	\brief Коэффициент A сигмоиды

	Значение по умолчанию: 1.

	\sa fmll_sigmoid, fmll_d_sigmoid.

\endlang

*/
extern double fmll_sigmoid_a;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_tanh, fmll_d_tanh.

\ru

	\brief Коэффициент A гиперболического тангенса

	Значение по умолчанию: 1.

	\sa fmll_tanh, fmll_d_tanh.

\endlang

*/
extern double fmll_tanh_a;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_tanh, fmll_d_tanh.

\ru

	\brief Коэффициент B гиперболического тангенса

	Значение по умолчанию: 1.

	\sa fmll_tanh, fmll_d_tanh.

\endlang

*/
extern double fmll_tanh_b;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 1.

	\sa fmll_gaussian, fmll_d_gaussian.

\ru

	\brief Коэффициент A гауссиана

	Значение по умолчанию: 1.

	\sa fmll_gaussian, fmll_d_gaussian.

\endlang

*/
extern double fmll_gaussian_a;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: 0.

	\sa fmll_gaussian, fmll_d_gaussian.

\ru

	\brief Коэффициент B гауссиана

	Значение по умолчанию: 0.

	\sa fmll_gaussian, fmll_d_gaussian.

\endlang

*/
extern double fmll_gaussian_b;

/*!

\en

	\brief TRANSLATE

	TRANSLATE: \f$ \sqrt{2} \f$.

	\sa fmll_gaussian, fmll_d_gaussian.

\ru

	\brief Коэффициент C гауссиана

	Значение по умолчанию: \f$ \sqrt{2} \f$.

	\sa fmll_gaussian, fmll_d_gaussian.

\endlang

*/
extern double fmll_gaussian_c;

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f(x) ~ = ~ A ~ \sin(Bx)
	\f]

	\sa fmll_sin_a, fmll_sin_b

\ru

	\brief Синус

	Вычисляется по формуле:

	\f[
	f(x) ~ = ~ A ~ \sin(Bx)
	\f]

	\sa fmll_sin_a, fmll_sin_b

\endlang

*/
double fmll_sin(double x);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f'(x) ~ = ~ A ~ B ~ \cos(Bx)
	\f]

	\sa fmll_sin_a, fmll_sin_b

\ru

	\brief Производная синуса

	Вычисляется по формуле:

	\f[
	f'(x) ~ = ~ A ~ B ~ \cos(Bx)
	\f]

	\sa fmll_sin_a, fmll_sin_b

\endlang

*/
double fmll_d_sin(double x);

/*!

\en

	\brief TRANSLATE (TRANSLATE)

	TRANSLATE:

	\f[
	f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
	\f]

	\sa fmll_sigmoid_a

\ru

	\brief Сигмоида (логистическая функция)

	Вычисляется по формуле:

	\f[
	f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
	\f]

	\sa fmll_sigmoid_a

\endlang

*/
double fmll_sigmoid(double x);

/*!

\en

	\brief TRANSLATE (TRANSLATE)

	TRANSLATE:

	\f[
	f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
	\f]

	\sa fmll_sigmoid_a

\ru

	\brief Производная сигмоиды (производная логистической функции)

	Вычисляется по формуле:

	\f[
	f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
	\f]

	\sa fmll_sigmoid_a

\endlang

*/
double fmll_d_sigmoid(double x);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f(x) ~ = ~ A ~ \tanh(Bx)
	\f]

	\sa fmll_tanh_a, fmll_tanh_b

\ru

	\brief Гиперболический тангенс (симметричная сигмоида)

	Вычисляется по формуле:

	\f[
	f(x) ~ = ~ A ~ \tanh(Bx)
	\f]

	\sa fmll_tanh_a, fmll_tanh_b

\endlang

*/
double fmll_tanh(double x);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
	\f]

	\sa fmll_tanh_a, fmll_tanh_b

\ru

	\brief Производная гиперболического тангенса

	Вычисляется по формуле:

	\f[
	f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
	\f]

	\sa fmll_tanh_a, fmll_tanh_b

\endlang

*/
double fmll_d_tanh(double x);

/*!

\en

	\brief TRANSLATE

\ru

	\brief Прямая \f$y ~ = ~ x\f$

\endlang

*/
double fmll_line(double x);

/*!

\en

	\brief TRANSLATE

\ru

	\brief Коэффициент прямой \f$y ~ = ~ x\f$ (всегда единица)

\endlang

*/
double fmll_d_line(double x);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f(x) ~ = ~ A ~ e ^ {- \frac{(x - B) ^ 2}{2 C ^ 2}}
	\f]

	\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

\ru

	\brief Гауссиан

	Вычисляется по формуле:

	\f[
	f(x) ~ = ~ A ~ exp \left (- \frac{(x - B) ^ 2}{2 C ^ 2} \right )
	\f]

	\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

\endlang

*/
double fmll_gaussian(double x);

/*!

\en

	\brief TRANSLATE

	TRANSLATE:

	\f[
	f'(x) ~ = ~ - \frac{x - B}{C ^ 2} f(x)
	\f]

	\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

\ru

	\brief Производная гауссиана

	Вычисляется по формуле:

	\f[
	f'(x) ~ = ~ - \frac{x - B}{C ^ 2} f(x)
	\f]

	\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

\endlang

*/
double fmll_d_gaussian(double x);

#ifdef __cplusplus
}
#endif

#endif

