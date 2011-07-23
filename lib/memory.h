
/* Управление памятью */

#ifndef MEMORY_H
#define MEMORY_H

#include "all.h"

/* Создание 3-х мерного массива */
void ***alloc_3D(unsigned height, unsigned width, unsigned dim, unsigned elem_size);

/* Создание 2-х мерного массива */
void **alloc_2D(unsigned height, unsigned dim, unsigned elem_size);

/* Создание 1-о мерного массива */
void *alloc_1D(unsigned dim, unsigned elem_size);

/* Довыделение памяти под 2-х мерный массив */
void **realloc_2D(void **mem, unsigned old_height, unsigned height, unsigned dim, unsigned elem_size);

/* Довыделение памяти под 1-о мерный массив */
void *realloc_1D(void *mem, unsigned old_dim, unsigned dim, unsigned elem_size);

/* Уничтожение массива */
void free_ND(void *mem);

#endif

