
/*!

\file

\en

	\brief TODO

\ru

	\brief Интерфейс к функционалу вывода информационных сообщений и сообщений об ошибках

	По умолчанию информационные сообщения выводятся в стандартный поток вывода (с помощью функции vprintf()), сообщения об ошибках выводятся в стандартный поток ошибок (с помощью функции vfprintf(), вызываемой через обертку к ней - fmll_stderr_print()).

\endlang

*/

#ifndef FMLL_PRINT_H
#define FMLL_PRINT_H

#ifdef FMLL_BUILD

	#include "all.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*! \en TODO \ru Указатель на функцию вывода информационных сообщений \endlang */
extern int (* fmll_print_fun)(const char * format, va_list param);

/*! \en TODO \ru Указатель на функцию вывода сообщений об ошибках \endlang */
extern int (* fmll_print_error_fun)(const char * format, va_list param);

/*!

\en

	\brief TODO

	TODO

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Вывод сообщения об ошибке в стандартный поток ошибок

	Функция fmll_stderr_print() принимает те же параметры, что и функция printf() из стандартной библиотеки.

	\return

		- 0 - в случае успешного вывода сообщения;
		- <> 0 - в случае, если сообщение вывести не удалось.

\endlang

*/
int fmll_stderr_print(const char * format, va_list param);
	
/*!

\en

	\brief TODO

	TODO

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Вывод информационного сообщения

	Функция fmll_print() принимает те же параметры, что и функция printf() из стандартной библиотеки.

	\return

		- 0 - в случае успешного вывода сообщения;
		- <> 0 - в случае, если сообщение вывести на целевое устройство (целевой поток вывода) не удалось.

\endlang

*/
int fmll_print(const char * format, ...);

/*!

\en

	\brief TODO

	TODO

	\return

		- 0 - TODO;
		- <> 0 - TODO.

\ru

	\brief Вывод сообщения об ошибке

	Функция fmll_print_error() принимает те же параметры, что и функция printf() из стандартной библиотеки.

	\return

		- 0 - в случае успешного вывода сообщения;
		- <> 0 - в случае, если сообщение вывести на целевое устройство (целевой поток вывода) не удалось.

\endlang

*/
int fmll_print_error(const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif

