
/*! \file \en \brief TRANSLATE \ru \brief Матрицы \endlang */

#ifndef MATRIX_H
#define MATRIX_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/exception.h"
	#include "lib/memory.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param value - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Инициализация матрицы

\param M - обрабатываемая матрица;
\param value - значение, в которое устанавливаются все элементы матрицы;
\param rows - количество строк в матрице;
\param cols - количество столбцов в матрице.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_init_fill(double ** M, const double value, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param value - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Инициализация диагональной матрицы

\param M - обрабатываемая матрица;
\param value - значение, в которое устанавливаются все элементы матрицы, расположенные на ее главной диагонали, все прочие элементы матрицы устанавливаются в 0;
\param rows - количество строк в матрице;
\param cols - количество столбцов в матрице.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_init_main_diag(double ** M, const double value, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param F - TRANSLATE;
\param T - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Копирование матрицы

\param F - исходная матрица;
\param T - результирующая матрица;
\param rows - количество строк в матрицах F и T;
\param cols - количество столбцов в матрицах F и T.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_copy(const double ** F, double ** T, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param R - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Транспонирование матрицы

\param M - транспонируемая матрица;
\param R - матрица, в которой будет сохранен результат транспонирования;
\param rows - количество строк в матрице M и количество столбцов в матрице R;
\param cols - количество столбцов в матрице M и количество строк в матрице R.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_transpose(const double ** M, double ** R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return >= 0 - TRANSLATE;
\return < 0 - TRANSLATE.

\ru

\brief Эвклидова норма матрицы

\param M - матрица;
\param rows - количество строк в матрице M;
\param cols - количество столбцов в матрице M.

\return >= 0 - норма матрицы;
\return < 0 - в случае неудачного завершения расчета нормы.

\endlang

*/
double fmll_math_matrix_euclid_norm(const double ** M, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param alpha_1 - TRANSLATE;
\param M1 - TRANSLATE;
\param alpha_2 - TRANSLATE;
\param M2 - TRANSLATE;
\param R - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\f[
sum = \alpha_1 M_1 + \alpha_2 M_2
\f]

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Суммирование матриц с домножением обоих слагаемых на вещественное число

\param alpha_1 - множитель для первого слагаемого;
\param M1 - матрица - первое слагаемое;
\param alpha_2 - множитель для второго слагаемого;
\param M2 - матрица - второе слагаемое;
\param R - матрица, в которой будет сохранена сумма матриц;
\param rows - количество строк в каждой из матриц;
\param cols - количество столбцов в каждой из матриц.

\f[
sum = \alpha_1 M_1 + \alpha_2 M_2
\f]

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_sum(const double alpha_1, const double ** M1, const double alpha_2, const double ** M2, double ** R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param V - TRANSLATE;
\param V_T - TRANSLATE;
\param M - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Умножение транспонированного вектора на вектор

\param V - вектор;
\param V_T - транспонируемый вектор;
\param M - матрица, в которую будет сохранено произведение вектора V на транспонированный вектор V_T;
\param rows - количество строк в матрице M и количество компонент в векторе V;
\param cols - количество столбцов в матрице M и количество компонент в векторе V_T.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_vector_transpose_mult_vector(const double * V, const double * V_T, double ** R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param value - TRANSLATE;
\param R - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Умножение матрицы на число

\param M - матрица;
\param value - число;
\param R - матрица, в которой будет сохранено произведение матрицы M на число value;
\param rows - количество строк в каждой из матриц;
\param cols - количество столбцов в каждой из матриц.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_mult_scalar(const double ** M, const double value, double ** R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param V - TRANSLATE;
\param R - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Умножение матрицы на вектор

\param M - матрица;
\param V - вектор;
\param R - вектор, в котором будет сохранено произведение матрицы M на вектор V;
\param rows - количество строк в матрице M;
\param cols - количество столбцов в матрице M и количество компонент в векторах V и R.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_mult_vector(const double ** M, const double * V, double * R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M1 - TRANSLATE;
\param M2 - TRANSLATE;
\param R - TRANSLATE;
\param rows_M1 - TRANSLATE;
\param cols_M1 - TRANSLATE;
\param cols_M2 - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Умножение матриц

\param M1 - первый множитель;
\param M2 - второй множитель;
\param R - матрица, в которой будет сохранено произведение матриц M1 и M2;
\param rows_M1 - количество строк в матрицах M1 и R;
\param cols_M1 - количество столбцов в матрице M1 и количество строк в матрице M2;
\param cols_M2 - количество столбцов в матрицах M2 и R.

Матрицы M1 и M2 не должны пересекаться с матрицой R.

Таким образом, матрица M1 имеет размер rows_M1 строк на cols_M1 столбцов, матрица M2 имеет размер cols_M1 строк на cols_M2 столбцов и матрица R имеет размер rows_M1 строк на cols_M2 столбцов.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_mult(const double ** M1, const double ** M2, double ** R, const unsigned rows_M1, const unsigned cols_M1, const unsigned cols_M2);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param L - TRANSLATE;
\param U - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief LU - разложение матрицы

\param M - исходная матрица;
\param L - матрица, в которой будет сохранена L-матрица преобразования;
\param U - матрица, в которой будет сохранена U-матрица преобразования;
\param rows - количество строк в матрицах M, L, U.

Матрицы M, L, U суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_lu(const double ** M, double ** L, double ** U, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param L - TRANSLATE;
\param U - TRANSLATE;
\param P - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief LUP - разложение матрицы

\param M - исходная матрица;
\param L - матрица, в которой будет сохранена L-матрица преобразования;
\param U - матрица, в которой будет сохранена U-матрица преобразования;
\param P - матрица, в которой будет сохранена P-матрица преобразования;
\param rows - количество строк в матрицах M, L, U, P.

Матрицы M, L, U, P суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_lup(const double ** M, double ** L, double ** U, double ** P, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param MI - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Расчет обратной матрицы

\param M - исходная матрица;
\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
\param rows - количество строк в матрицах M и MI.

Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_inv(const double ** M, double ** MI, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param MI - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Расчет обратной матрицы для верхнетреугольной матрицы

\param M - исходная матрица;
\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
\param rows - количество строк в матрицах M и MI.

Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_inv_high_tr(const double ** M, double ** MI, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param MI - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Расчет обратной матрицы для нижнетреугольной матрицы

\param M - исходная матрица;
\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
\param rows - количество строк в матрицах M и MI.

Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_inv_low_tr(const double ** M, double ** MI, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param H - TRANSLATE;
\param rows - TRANSLATE.

TRANSLATE

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Хессенбергова форма матрицы

\param M - исходная квадратная матрица;
\param H - матрица, в которую будет сохранена Хессенбергова форма исходной матрицы;
\param rows - количество строк и столбцов в матрицах M и H.

Матрицы M и H суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_hessenberg(const double ** M, double ** H, const unsigned rows);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param S - TRANSLATE;
\param rows - TRANSLATE;
\param precision - TRANSLATE.

TRANSLATE

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Форма Шура

\param M - исходная квадратная матрица;
\param S - матрица, в которую будет сохранена форма Шура исходной матрицы;
\param rows - количество строк и столбцов в матрицах M и S;
\param precision - точность вычисления формы (алгоритм будет остановлен, как только наибольшее по модулю произведение соседних поддиагональных элементов станет меньше по модулю указанной точности; \f$precision \ge 0\f$).

Матрицы M и S суть есть квадратные матрицы с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_shur(const double ** M, double ** S, const unsigned rows, const double precision);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param Q - TRANSLATE;
\param R - TRANSLATE;
\param rows - TRANSLATE;
\param cols - TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief QR - разложение матрицы

\param M - исходная матрица;
\param Q - квадратная матрица, в которую будет сохранена Q - матрица разложения (ортогональная матрица разложения);
\param R - матрица, в которую будет сохранена R - матрица разложения (верхнетреугольная матрица разложения);
\param rows - количество строк в матрицах M, Q и R, количество столбцов в матрице Q;
\param cols - количество столбцов в матрицах M и R.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_QR(const double ** M, double ** Q, double ** R, const unsigned rows, const unsigned cols);

/*!

\en

\brief TRANSLATE

\param M - TRANSLATE;
\param eigen_real - TRANSLATE;
\param eigen_complex - TRANSLATE;
\param rows - TRANSLATE;
\param precision_shur - TRANSLATE.

TRANSLATE.

\return 0 - TRANSLATE;
\return <> 0 - TRANSLATE.

\ru

\brief Собственные числа матрицы

\param M - исходная матрица;
\param eigen_real - вектор, в котором будут сохранены действительные части собственных чисел матрицы в порядке убывания модулей собственных чисел;
\param eigen_complex - вектор, в котором будут сохранены мнимые части собственных чисел матрицы в порядке убывания модулей собственных чисел (значение данного параметра может быть равно NULL);
\param rows - количество строк в матрице M, количество компонент в векторах eigen_real и eigen_complex;
\param precision_shur - точность расчета матрицы Шура в процессе вычисления собственных чисел (\f$precision\_shur \ge 0\f$).

Матрица M суть есть квадратная матрица с размером стороны равным rows.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения операции.

\endlang

*/
int fmll_math_matrix_eigen(const double ** M, double * eigen_real, double * eigen_complex, const unsigned rows, const double precision_shur);

#ifdef __cplusplus
}
#endif

#endif

