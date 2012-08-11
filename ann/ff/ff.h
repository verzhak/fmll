
/*!

\file

\en

	\brief TRANSLATE

	TRANSLATE

\ru

	\brief TODO

	Последовательность действий по использованию TODO:

	-# TODO

\endlang

*/

#ifndef FMLL_FF_H
#define FMLL_FF_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "private/xml.h"
	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "math/graph/graph.h"
	#include "math/random/random.h"

#endif

/* TODO Документация */

/* ############################################################################ */

typedef struct t_fmll_ff
{

	char * is_run, ** connect;
	unsigned num, in_dim, out_dim, * in, * out;
	double * ry, * y, * net, * b, ** w;
	double (** fun)(double);
	double (** d_fun)(double);

} fmll_ff;

/* ############################################################################ */

#ifdef __cplusplus
extern "C"
{
#endif

fmll_ff * fmll_ff_init(unsigned num, double (** fun)(double), double (** d_fun)(double));
void fmll_ff_destroy(fmll_ff * ff);
int fmll_ff_set_connect(fmll_ff * ff, char ** connect);
int fmll_ff_save(fmll_ff * ff, const char * fname_prefix);
fmll_ff * fmll_ff_load(const char * fname_prefix);
const double * fmll_ff_run(fmll_ff * ff, const double * vec);

/* TODO Обратное распространение ошибки */

#ifdef __cplusplus
}
#endif

/* ############################################################################ */

#endif

