
/*!

\file

\brief Обработка исключительных ситуаций

*/

#ifndef FMLL_EXCEPTION_H
#define FMLL_EXCEPTION_H

/*! \brief Начало блока перехвата исключений. */
#define fmll_try __label__ catch_label, finally_label;

/*! \brief Начало блока обработки исключений. */
#define fmll_catch goto finally_label; catch_label:

/*! \brief Начало гарантированного завершения блока кода. */
#define fmll_finally finally_label:

/*!

\brief Переход на finally-блок.

\sa fmll_finally

*/
#define fmll_to_finally goto finally_label;

/*!

\def fmll_throw

\brief Условное возбуждение исключения.

\param condition - условие, по истинности которого будет возбуждено исключение.

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

\brief Возбуждение исключения в случае, если указатель равен NULL.

\param pointer - проверяемый указатель.

*/
#define fmll_throw_null(pointer) fmll_throw((pointer) == NULL)

#endif

