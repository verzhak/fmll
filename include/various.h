
/*!

\file

\brief Дополнительный функционал.

*/

#ifndef FMLL_VARIOUS_H
#define FMLL_VARIOUS_H

#ifdef FMLL_BUILD

#include "all.h"
#include "memory.h"
#include "exception.h"

#endif

/*!

\brief Нормирование векторов.

\param vec - множество векторов;
\param vec_num - размер множества векторов;
\param dim - размерность векторного пространства.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int fmll_normalization(double ** vec, unsigned vec_num, unsigned dim);

/*!

\brief Центрирование векторов (вычитание из каждой компоненты вектора ее эмпирического математического ожидания).

\param vec - множество векторов;
\param vec_num - размер множества векторов;
\param dim - размерность векторного пространства.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int fmll_centering(double ** vec, unsigned vec_num, unsigned dim);

#endif

