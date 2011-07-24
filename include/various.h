
/*!

\file

\brief Дополнительный функционал.

*/

#ifndef VARIOUS_H
#define VARIOUS_H

#include "all.h"

/*!

\brief Нормировка векторов.

\param vec - множество векторов;
\param vec_num - размер множества векторов;
\param dim - размерность векторного пространства.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int8_t fmll_normalization(double ** vec, uint32_t vec_num, uint8_t dim);

#endif

