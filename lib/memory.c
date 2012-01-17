
#include "lib/memory.h"

void * fmll_alloc(unsigned type_size, unsigned dim, ...)
{
	fmll_try;

		void * ret = NULL;
		va_list val;
		unsigned u, v, num, size, step, dim_1 = dim - 1, dim_2 = dim - 2, dim_size[dim];
		void * pof, * dof, * t_dof;

		va_start(val, dim);

		for(u = 0, size = 0, num = 1; u < dim; u++)
		{
			num *= (dim_size[u] = va_arg(val, unsigned));
			size += (u == dim_1 ? type_size : sizeof(void *)) * num;
		}

		va_end(val);

		fmll_throw_null((ret = malloc(size)));

		for(u = 0, num = 1, dof = ret; u < dim_1; u++)
		{
			num *= dim_size[u];
			pof = dof;
			t_dof = (dof += num * sizeof(void *));
			step = (u == dim_2 ? type_size : sizeof(void *)) * dim_size[u + 1];

			for(v = 0; v < num; v++, pof += sizeof(void *), t_dof += step)
				memcpy(pof, & t_dof, sizeof(void *));
		}

	fmll_catch;

		fmll_free(ret);
		ret = NULL;

	fmll_finally;

	return ret;
}

void fmll_free(void * mem)
{
	if(mem != NULL)
		free(mem);
}

