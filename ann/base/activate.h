
/*!

\file

\en \brief TRANSLATE \ru \brief Функции активации нейронов \endlang

*/

#ifndef FMLL_ACTIVATE_H
#define FMLL_ACTIVATE_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "math/various/constant.h"

#endif

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент A синуса

Значение по умолчанию: 1.

\endlang

\sa fmll_sin, fmll_d_sin

*/
extern double fmll_sin_a;

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент B синуса

Значение по умолчанию: 1.

\endlang

\sa fmll_sin, fmll_d_sin

*/
extern double fmll_sin_b;

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент A сигмоиды

Значение по умолчанию: 1.

\endlang

\sa fmll_sigmoid, fmll_d_sigmoid

*/
extern double fmll_sigmoid_a;

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент A гиперболического тангенса

Значение по умолчанию: 1.

\endlang

\sa fmll_tanh, fmll_d_tanh

*/
extern double fmll_tanh_a;

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент B гиперболического тангенса

Значение по умолчанию: 1.

\endlang

\sa fmll_tanh, fmll_d_tanh

*/
extern double fmll_tanh_b;

/*!

\en \brief TRANSLATE

Default: 1.

\ru \brief Коэффициент A гауссиана

Значение по умолчанию: 1.

\endlang

\sa fmll_gaussian, fmll_d_gaussian

*/
extern double fmll_gaussian_a;

/*!

\en \brief TRANSLATE

Default: 0.

\ru \brief Коэффициент B гауссиана

Значение по умолчанию: 0.

\endlang

\sa fmll_gaussian, fmll_d_gaussian

*/
extern double fmll_gaussian_b;

/*!

\en \brief TRANSLATE

Default: \f$ \sqrt{2} \f$.

\ru \brief Коэффициент C гауссиана

Значение по умолчанию: \f$ \sqrt{2} \f$.

\endlang

\sa fmll_gaussian, fmll_d_gaussian

*/
extern double fmll_gaussian_c;

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en \brief TRANSLATE

\ru \brief Синус

\endlang

\f[
f(x) ~ = ~ A ~ \sin(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_sin(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Производная синуса

\endlang

\f[
f'(x) ~ = ~ A ~ B ~ \cos(Bx)
\f]

\sa fmll_sin_a, fmll_sin_b

*/
double fmll_d_sin(const double x);

/*!

\en \brief TRANSLATE (TRANSLATE)

\ru \brief Сигмоида (логистическая функция)

\endlang

\f[
f(x) ~ = ~ \frac{1}{1 + e^{-Ax}}
\f]

\sa fmll_sigmoid_a

*/
double fmll_sigmoid(const double x);

/*!

\en \brief TRANSLATE (TRANSLATE)

\ru \brief Производная сигмоиды (производная логистической функции)

\endlang

\f[
f'(x) ~ = ~ A ~ f(x) ~ (1 ~ - ~ f(x))
\f]

\sa fmll_sigmoid_a

*/
double fmll_d_sigmoid(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Гиперболический тангенс (симметричная сигмоида)

\endlang

\f[
f(x) ~ = ~ A ~ \tanh(Bx)
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_tanh(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Производная гиперболического тангенса

\endlang

\f[
f'(x) ~ = ~ \frac{B}{A} ~ (A ~ - ~ f(x)) ~ (A ~ + ~ f(x))
\f]

\sa fmll_tanh_a, fmll_tanh_b

*/
double fmll_d_tanh(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Прямая

\endlang

\f$y ~ = ~ x\f$

*/
double fmll_line(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Коэффициент прямой \f$y ~ = ~ x\f$ (всегда единица)

\endlang

*/
double fmll_d_line(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Гауссиан

\endlang

\f[
f(x) ~ = ~ A ~ exp \left (- \frac{(x - B) ^ 2}{2 C ^ 2} \right )
\f]

\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

*/
double fmll_gaussian(const double x);

/*!

\en \brief TRANSLATE

\ru \brief Производная гауссиана

\endlang

\f[
f'(x) ~ = ~ - \frac{x - B}{C ^ 2} f(x)
\f]

\sa fmll_gaussian_a, fmll_gaussian_b, fmll_gaussian_c

*/
double fmll_d_gaussian(const double x);

#ifdef __cplusplus
}
#endif

#endif

