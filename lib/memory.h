
/*!

\file

\en

	\brief TRANSLATE

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

	\brief TRANSLATE

	\param type_size - TRANSLATE;
	\param dim - TRANSLATE;
	\param ... - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Создание N - мерного массива

	\param type_size - размер (в байтах) элемента массива;
	\param dim - количество размерностей массива;
	\param ... - размер массива по каждой из размерностей.

	\return указатель на массив в случае успешного выделения памяти;
	\return NULL - в случае неудачи.

\endlang

*/
void * fmll_alloc(const unsigned type_size, const unsigned dim, ...);

#ifdef FMLL_OS_WINDOWS

#define fmll_alloc_a(size) \
	_alloca(size)

#else

/*!

\en

	\brief TRANSLATE

	\param size - TRANSLATE.

	\return TRANSLATE;
	\return NULL - TRANSLATE.

\ru

	\brief Выделение памяти, динамически освобождающейся по выходу из блока (функции)

	\param size - размер (в байтах) выделяемого участка памяти.

	\return указатель на выделенную область памяти в случае успешного ее выделения;
	\return NULL - в случае, если память выделить не удалось.

\endlang

*/
#define fmll_alloc_a(size) \
	alloca(size)

#endif

/*!

\en

	\brief TRANSLATE

	\param mem - TRANSLATE.

\ru

	\brief Освобождение памяти, занятой массивом

	\param mem - указатель на массив.

\endlang

*/
void fmll_free(void * mem);

#ifdef __cplusplus
}
#endif

#endif

