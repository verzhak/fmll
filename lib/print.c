
#include "lib/print.h"

int (* fmll_print_fun)(const char * format, va_list param) = & vprintf;
int (* fmll_print_error_fun)(const char * format, va_list param) = & fmll_stderr_print;

int fmll_stderr_print(const char * format, va_list param)
{
	return vfprintf(stderr, format, param);
}

int fmll_print(const char * format, ...)
{
	int ret = 0;

	if(fmll_print_fun != NULL)
	{
		va_list args;
		va_start(args, format);

		ret = (* fmll_print_fun)(format, args);

		va_end (args);
	}

	return ret;
}

int fmll_print_error(const char * format, ...)
{
	int ret = 0;

	if(fmll_print_error_fun != NULL)
	{
		va_list args;
		va_start(args, format);

		ret = (* fmll_print_error_fun)(format, args);

		va_end (args);
	}

	return ret;
}

