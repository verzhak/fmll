
/*!

\file

\en

\brief Graphs

TRANSLATE

\ru

\brief Графы

В любой матрице, описывающей граф:

- номер строки суть есть индекс вершины графа - исхода соответствующего ребра;
- номер столбца суть есть индекс вершины графа - захода соответствующего ребра.

\endlang

*/

#ifndef GRAPH_H
#define GRAPH_H

#ifdef FMLL_BUILD

	#include "all.h"
	#include "lib/exception.h"
	#include "lib/memory.h"

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!

\en

\brief TRANSLATE

\param A - TRANSLATE;
\param W - TRANSLATE;
\param num - TRANSLATE.

\return 0 - success;
\return <> 0 - fail.

\ru

\brief Расчет транзитивного замыкания (матрицы достижимости) графа с помощью алгоритма Уоршалла

\param A - матрица смежности графа (0 - ребра нет, != 0 - ребро наличествует);
\param W - результирующая матрица достижимости;
\param num - количество вершин в графе.

\return 0 - в случае успешного выполнения операции;
\return <> 0 - в случае неудачного завершения выполнения операции.

\endlang

*/
int fmll_math_graph_warshall(const unsigned char ** A, unsigned char  ** W, const unsigned num);

#ifdef __cplusplus
}
#endif

#endif


