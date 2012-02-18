
/*!

\file

\en

	\brief TODO

\ru

	\brief Обработка исключительных ситуаций

\endlang

*/

#ifndef FMLL_EXCEPTION_H
#define FMLL_EXCEPTION_H

#ifdef __cplusplus

	// C++

/*!

\def fmll_throw

\en

	\brief TODO.

	\param condition - TODO.

\ru

	\brief Условное возбуждение исключения.

	\param condition - условие, по истинности которого будет возбуждено исключение.

\endlang

*/
#ifdef FMLL_DEBUG

	#define fmll_throw(condition) \
if((condition))\
{\
	fprintf(stderr, "[Исключение] Файл %s, строка %d\n", __FILE__, __LINE__);\
	throw 1;
};

#else

	#define fmll_throw(condition) \
if((condition)) \
	throw 1;

#endif

/*!

\en

	\brief TODO.

	\param pointer - TODO.

\ru

	\brief Возбуждение исключения в случае, если указатель равен NULL.

	\param pointer - проверяемый указатель.

\endlang

*/
#define fmll_throw_null(pointer) fmll_throw((pointer) == NULL)

#else

	// C

/*!

\en

	\brief TODO.

\ru

	\brief Начало блока перехвата исключений.

\endlang

*/
#define fmll_try __label__ catch_label, finally_label;

/*!

\en

	\brief TODO.

\ru

	\brief Начало блока обработки исключений.

\endlang

*/
#define fmll_catch goto finally_label; catch_label:

/*!

\en

	\brief TODO.
	
\ru

	\brief Начало гарантированного завершения блока кода.

\endlang

*/
#define fmll_finally finally_label:

/*!

\en

	\brief TODO.

\ru

	\brief Переход на finally - блок.

\endlang

\sa fmll_finally

*/
#define fmll_to_finally goto finally_label;

/*!

\def fmll_throw

\en

	\brief TODO.

	\param condition - TODO.

\ru

	\brief Условное возбуждение исключения.

	\param condition - условие, по истинности которого будет возбуждено исключение.

\endlang

*/
#ifdef FMLL_DEBUG

	#define fmll_throw(condition) \
if((condition))\
{\
	fprintf(stderr, "[Исключение] Файл %s, строка %d\n", __FILE__, __LINE__);\
	goto catch_label;\
};

#else

	#define fmll_throw(condition) \
if((condition)) \
	goto catch_label;

#endif

/*!

\en

	\brief TODO.

	\param pointer - TODO.

\ru

	\brief Возбуждение исключения в случае, если указатель равен NULL.

	\param pointer - проверяемый указатель.

\endlang

*/
#define fmll_throw_null(pointer) fmll_throw((pointer) == NULL)

#endif

#endif

