
/*!

\file

\en

	\brief TODO.

\ru

	\brief Действия над матрицами.

\endlang

*/

#ifndef MATRIX_H
#define MATRIX_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/exception.h"
	#include "lib/memory.h"

#endif

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param value - TODO;
	\param rows - TODO;
	\param cols - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Инициализация диагональной матрицы.

	\param M - обрабатываемая матрица;
	\param value - значение, в которое устанавливаются все элементы матрицы, расположенные на ее главной диагонали, все прочие элементы матрицы устанавливаются в 0;
	\param rows - количество строк в матрице;
	\param cols - количество столбцов в матрице.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_init_main_diag(double ** M, double value, unsigned rows, unsigned cols);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param R - TODO;
	\param rows - TODO;
	\param cols - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Транспонирование матрицы.

	\param M - транспонируемая матрица;
	\param R - матрица, в которой будет сохранен результат транспонирования;
	\param rows - количество строк в матрице M и количество столбцов в матрице R;
	\param cols - количество столбцов в матрице M и количество строк в матрице R.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_transpose(double ** M, double ** R, unsigned rows, unsigned cols);

/*!

\en

	\brief TODO.

	\param M1 - TODO;
	\param M2 - TODO;
	\param R - TODO;
	\param rows - TODO;
	\param cols - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Суммирование матриц.

	\param M1 - матрица - первое слагаемое;
	\param M2 - матрица - второе слагаемое;
	\param R - матрица, в которой будет сохранена сумма матриц;
	\param rows - количество строк в каждой из матриц;
	\param cols - количество столбцов в каждой из матриц.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_sum(double ** M1, double ** M2, double ** R, unsigned rows, unsigned cols);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param value - TODO;
	\param R - TODO;
	\param rows - TODO;
	\param cols - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Умножение матрицы на число.

	\param M - матрица;
	\param value - число;
	\param R - матрица, в которой будет сохранено произведение матрицы M на число value;
	\param rows - количество строк в каждой из матриц;
	\param cols - количество столбцов в каждой из матриц.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_mult_scalar(double ** M, double value, double ** R, unsigned rows, unsigned cols);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param V - TODO;
	\param R - TODO;
	\param rows - TODO;
	\param cols - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Умножение матрицы на вектор.

	\param M - матрица;
	\param V - вектор;
	\param R - вектор, в котором будет сохранено произведение матрицы M на вектор V;
	\param rows - количество строк в матрице M;
	\param cols - количество столбцов в матрице M и количество компонент в векторах V и R.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_mult_vector(double ** M, double * V, double * R, unsigned rows, unsigned cols);

/*!

\en

	\brief TODO.

	\param M1 - TODO;
	\param M2 - TODO;
	\param R - TODO;
	\param rows_M1 - TODO;
	\param cols_M1 - TODO;
	\param cols_M2 - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Умножение матриц.

	\param M1 - первый множитель;
	\param M2 - второй множитель;
	\param R - матрица, в которой будет сохранено произведение матриц M1 и M2;
	\param rows_M1 - количество строк в матрицах M1 и R;
	\param cols_M1 - количество столбцов в матрице M1 и количество строк в матрице M2;
	\param cols_M2 - количество столбцов в матрицах M2 и R.

	Таким образом, матрица M1 имеет размер rows_M1 строк на cols_M1 столбцов, матрица M2 имеет размер cols_M1 строк на cols_M2 столбцов и матрица R имеет размер rows_M1 строк на cols_M2 столбцов.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_mult(double ** M1, double ** M2, double ** R, unsigned rows_M1, unsigned cols_M1, unsigned cols_M2);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param L - TODO;
	\param U - TODO;
	\param rows - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Вычисление LU-преобразования матрицы.

	\param M - исходная матрица;
	\param L - матрица, в которой будет сохранена L-матрица преобразования;
	\param U - матрица, в которой будет сохранена U-матрица преобразования;
	\param rows - количество строк в матрицах M, L, U.

	Матрицы M, L, U суть есть квадратные матрицы с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_lu(double ** M, double ** L, double ** U, unsigned rows);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param L - TODO;
	\param U - TODO;
	\param P - TODO;
	\param rows - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Вычисление LUP-преобразования матрицы.

	\param M - исходная матрица;
	\param L - матрица, в которой будет сохранена L-матрица преобразования;
	\param U - матрица, в которой будет сохранена U-матрица преобразования;
	\param P - матрица, в которой будет сохранена P-матрица преобразования;
	\param rows - количество строк в матрицах M, L, U, P.

	Матрицы M, L, U, P суть есть квадратные матрицы с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_lup(double ** M, double ** L, double ** U, double ** P, unsigned rows);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param MI - TODO;
	\param rows - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Расчет обратной матрицы.

	\param M - исходная матрица;
	\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
	\param rows - количество строк в матрицах M и MI.

	Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_inv(double ** M, double ** MI, unsigned rows);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param MI - TODO;
	\param rows - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Расчет обратной матрицы для верхнетреугольной матрицы.

	\param M - исходная матрица;
	\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
	\param rows - количество строк в матрицах M и MI.

	Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_inv_high_tr(double ** M, double ** MI, unsigned rows);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param MI - TODO;
	\param rows - TODO.

	TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Расчет обратной матрицы для нижнетреугольной матрицы.

	\param M - исходная матрица;
	\param MI - матрица, в которой будет сохранена обратная матрица к матрице M;
	\param rows - количество строк в матрицах M и MI.

	Матрицы M и MI суть есть квадратные матрицы с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_inv_low_tr(double ** M, double ** MI, unsigned rows);

/*!

\en

	\brief TODO.

	\param M - TODO;
	\param eigen - TODO;
	\param rows - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Расчет собственных чисел матрицы.

	\param M - матрица;
	\param eigen - вектор, в котором будут сохранены собственные числа матрицы в порядке их убывания;
	\param rows - количество строк в матрице M, количество компонент в векторе eigen.

	Матрица M суть есть квадратная матрица с размером стороны равным rows.

	\return

		- 0 - в случае успешного выполнения операции;
		- <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_matrix_eigen(double ** M, double * eigen, unsigned rows);

#endif

