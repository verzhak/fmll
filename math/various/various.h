
/*!

\file

\en

	\brief Additional functions

\ru

	\brief Дополнительный функционал

\endlang

*/

#ifndef FMLL_VARIOUS_H
#define FMLL_VARIOUS_H

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

	\brief Vector normalization

	\param vec - set of vectors;
	\param vec_num - number of vectors in set;
	\param dim - vector dimension.

	\return 0 - success;
	\return <> 0 - fail.

\ru

	\brief Нормирование векторов

	\param vec - множество векторов;
	\param vec_num - размер множества векторов;
	\param dim - размерность векторного пространства.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_normalization(double ** vec, unsigned vec_num, unsigned dim);

/*!

\en

	\brief Vector centering (subtraction from all vector components his mean values)

	\param vec - set of vectors;
	\param vec_num - number of vectors in set;
	\param dim - vector dimension.

	\return 0 - success;
	\return <> 0 - fail.

\ru

	\brief Центрирование векторов (вычитание из каждой компоненты вектора ее эмпирического математического ожидания)

	\param vec - множество векторов;
	\param vec_num - размер множества векторов;
	\param dim - размерность векторного пространства.

	\return 0 - в случае успеха;
	\return <> 0 - в случае неудачи.

\endlang

*/
int fmll_centering(double ** vec, unsigned vec_num, unsigned dim);

#ifdef __cplusplus
}
#endif

#endif

