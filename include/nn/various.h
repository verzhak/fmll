
/*!

\file

\en

	\brief TODO.

\ru

	\brief Дополнительный функционал.

\endlang

*/

#ifndef FMLL_VARIOUS_H
#define FMLL_VARIOUS_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/exception.h"
	#include "lib/memory.h"

#endif

/*!

\en

	\brief TODO.

	\param vec - TODO;
	\param vec_num - TODO;
	\param dim - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Нормирование векторов.

	\param vec - множество векторов;
	\param vec_num - размер множества векторов;
	\param dim - размерность векторного пространства.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_normalization(double ** vec, unsigned vec_num, unsigned dim);

/*!

\en

	\brief TODO (TODO).

	\param vec - TODO;
	\param vec_num - TODO;
	\param dim - TODO.

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Центрирование векторов (вычитание из каждой компоненты вектора ее эмпирического математического ожидания).

	\param vec - множество векторов;
	\param vec_num - размер множества векторов;
	\param dim - размерность векторного пространства.

	\return

		- 0 - в случае успеха;
		- <> 0 - в случае неудачи.

\endlang

*/
int fmll_centering(double ** vec, unsigned vec_num, unsigned dim);

#endif

