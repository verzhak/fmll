
/*!

\file

\brief Управление памятью.

*/

#ifndef FMLL_MEMORY_H
#define FMLL_MEMORY_H

#ifdef FMLL_BUILD

#include "all.h"
#include "exception.h"

#endif

/*!

\brief Создание N-х мерного массива

\param type_size - размер (в байтах) элемента массива;
\param dim - количество размерностей массива;
\param ... - размер массива по каждой из размерностей.

\return

	- указатель на массив в случае успешного выделения памяти;
	- NULL в случае неудачи.

*/
void * fmll_alloc(unsigned type_size, unsigned dim, ...);

/*!

\brief Освобождение памяти, занятой массивом

\param mem - указатель на массив.

*/
void fmll_free(void * mem);

#endif

