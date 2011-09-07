
/*!

\file

\brief Управление памятью.

*/

#ifndef MEMORY_H
#define MEMORY_H

#include "all.h"
#include "exception.h"

/*!

\brief Создание N-х мерного массива

\param type_size - размер (в байтах) элемента массива;
\param dim - количество размерностей массива;
\param ... - размер массива по каждой из размерностей.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void * fmll_alloc(unsigned type_size, unsigned dim, ...);

/*!

\brief Освобождение памяти, занятой массивом

\param mem - указатель на массив.

*/
void fmll_free(void * mem);

#endif

