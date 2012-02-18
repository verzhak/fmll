
/*!

\file

\en

	\brief TODO

\ru

	\brief Управление памятью

\endlang

*/

#ifndef FMLL_MEMORY_H
#define FMLL_MEMORY_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/exception.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

	\brief TODO.

	\param type_size - TODO;
	\param dim - TODO;
	\param ... - TODO.

	\return

		- TODO;
		- NULL - TODO.

\ru

	\brief Создание N-х мерного массива.

	\param type_size - размер (в байтах) элемента массива;
	\param dim - количество размерностей массива;
	\param ... - размер массива по каждой из размерностей.

	\return

		- указатель на массив в случае успешного выделения памяти;
		- NULL - в случае неудачи.

\endlang

*/
void * fmll_alloc(unsigned type_size, unsigned dim, ...);

/*!

\en

	\brief TODO.

	\param mem - TODO.

\ru

	\brief Освобождение памяти, занятой массивом.

	\param mem - указатель на массив.

\endlang

*/
void fmll_free(void * mem);

#ifdef __cplusplus
}
#endif

#endif

