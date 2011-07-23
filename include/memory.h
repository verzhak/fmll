
/*!

\file

\brief Управление памятью.

*/

#ifndef MEMORY_H
#define MEMORY_H

#include "all.h"

/*!

\brief Создание 3-х мерного массива

\param height - первая размерность массива;
\param width - вторая размерность массива;
\param dim - третья размерность массива;
\param elem_size - размер (в байтах) элемента массива.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void *** fmll_alloc_3D(unsigned height, unsigned width, unsigned dim, unsigned elem_size);

/*!

\brief Создание 2-х мерного массива

\param height - первая размерность массива;
\param dim - вторая размерность массива;
\param elem_size - размер (в байтах) элемента массива.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void ** fmll_alloc_2D(unsigned height, unsigned dim, unsigned elem_size);

/*!

\brief Создание 1-о мерного массива

\param dim - размерность массива;
\param elem_size - размер (в байтах) элемента массива.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void * fmll_alloc_1D(unsigned dim, unsigned elem_size);

/*!

\brief Изменение первой размерности 2-х мерного массива

\param mem - указатель на массив;
\param old_height - текущее значение первой размерности массива;
\param height - новое значение первой размерности массива;
\param dim - вторая размерность массива;
\param elem_size - размер (в байтах) элемента массива.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void ** fmll_realloc_2D(void ** mem, unsigned old_height, unsigned height, unsigned dim, unsigned elem_size);

/*!

\brief Изменение размера 1-о мерного массива

\param mem - указатель на массив;
\param old_dim - текущий размер массива;
\param dim - новый размер массива;
\param elem_size - размер (в байтах) элемента массива.

\return

	- указатель на массив в случае успеха;
	- NULL в случае неудачи.

*/
void * fmll_realloc_1D(void * mem, unsigned old_dim, unsigned dim, unsigned elem_size);

/*!

\brief Уничтожение массива

\param mem - указатель на массив.

*/
void fmll_free_ND(void * mem);

#endif

