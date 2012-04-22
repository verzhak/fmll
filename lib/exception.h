
/*!

\file

\en

	\brief TODO

\ru

	\brief Обработка исключительных ситуаций

	\remark Только для C

\endlang

*/

#ifndef FMLL_EXCEPTION_H
#define FMLL_EXCEPTION_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/print.h"

#endif

#ifdef __cplusplus

	// C++

	#define fmll_throw \
	{\
		fmll_print_error("[Exception] File %s, line %d\n", __FILE__, __LINE__);\
		throw 1;\
	};

#else

	/* C */

	/*!

	\en

		\brief TODO

		TODO

	\ru

		\brief Начало блока перехвата исключений

	\endlang

	*/
	#define fmll_try ;

	/*!

	\en

		\brief TODO

	\ru

		\brief Начало блока обработки исключений

	\endlang

	*/
	#define fmll_catch \
		goto finally_label; catch_label:

	/*!

	\en

		\brief TODO
		
	\ru

		\brief Начало блока гарантированного выполнения по выходу из функции

	\endlang

	*/
	#define fmll_finally \
		finally_label:

	/*!

	\en

		\brief TODO

	\ru

		\brief Переход на finally - блок

	\endlang

	*/
	#define fmll_to_finally \
		goto finally_label;

	/*!

	\en

		\brief TODO

	\ru

		\brief Возбуждение исключения

	\endlang

	*/
	#define fmll_throw \
	{\
		fmll_print_error("[Exception] File %s, line %d\n", __FILE__, __LINE__);\
		goto catch_label;\
	};

#endif

/*!

\en

	\brief TODO

	\param condition - TODO.

\ru

	\brief Условное возбуждение исключения

	\param condition - условие, по истинности которого будет возбуждено исключение.

\endlang

*/
#define fmll_throw_if(condition) \
{\
	if((condition))\
		fmll_throw;\
}

/*!

\en

	\brief TODO

	\param pointer - TODO.

\ru

	\brief Возбуждение исключения в случае, если указатель равен NULL

	\param pointer - проверяемый указатель.

\endlang

*/
#define fmll_throw_null(pointer) \
	fmll_throw_if((pointer) == NULL)

#endif

