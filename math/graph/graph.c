
#include "math/graph/graph.h"

int fmll_math_graph_warshall(char ** A, char  ** W, unsigned num)
{
	unsigned v, u, t;

	for(v = 0; v < num; v++)
		memcpy(W[v], A[v], sizeof(char) * num); /* Оптимизирующий компилятор должен преобразовать третий параметр к (num) */

	for(t = 0; t < num; t++)
		for(v = 0; v < num; v++)
			for(u = 0; u < num; u++)
				if(! W[v][u])
					W[v][u] = W[v][t] && W[t][u];

	return 0;
}

