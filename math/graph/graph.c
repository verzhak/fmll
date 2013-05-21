
#include "math/graph/graph.h"

int fmll_math_graph_warshall(const unsigned char ** A, unsigned char  ** W, const unsigned num)
{
	unsigned v, u, t;

	for(v = 0; v < num; v++)
		memcpy(W[v], A[v], num);

	for(t = 0; t < num; t++)
		for(v = 0; v < num; v++)
			for(u = 0; u < num; u++)
				if(! W[v][u])
					W[v][u] = W[v][t] && W[t][u];

	return 0;
}

