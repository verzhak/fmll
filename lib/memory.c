
#include "lib/memory.h"

void * fmll_alloc(const unsigned type_size, const unsigned dim, ...)
{
	void * mem = NULL;
	va_list val;
	const unsigned dim_1 = dim - 1, dim_2 = dim - 2;
	unsigned u, v, num, size, step, * dim_size;
	char * pof, * dof, * t_dof; /* Стандартом гарантируется, что sizeof(char) == 1 */

	fmll_try;

		fmll_throw_null(dim_size = fmll_alloc_a(dim * sizeof(unsigned)));

		va_start(val, dim);

		for(u = 0, size = 0, num = 1; u < dim; u++)
		{
			num *= (dim_size[u] = va_arg(val, unsigned));
			size += (u == dim_1 ? type_size : sizeof(void *)) * num;
		}

		va_end(val);

		#ifdef FMLL_OS_WINDOWS

			mem = malloc(size); /* TODO */

		#else

			mem = malloc(size);

		#endif

		fmll_throw_null(mem);

		for(u = 0, num = 1, dof = mem; u < dim_1; u++)
		{
			num *= dim_size[u];
			pof = dof;
			t_dof = (dof += num * sizeof(void *));
			step = (u == dim_2 ? type_size : sizeof(void *)) * dim_size[u + 1];

			for(v = 0; v < num; v++, pof += sizeof(void *), t_dof += step)
				memcpy(pof, & t_dof, sizeof(void *));
		}

	fmll_catch;

		fmll_free(mem);
		mem = NULL;

	fmll_finally;

	return mem;
}

void fmll_free(void * mem)
{
	if(mem != NULL)
	{
		#ifdef FMLL_OS_WINDOWS

			free(mem); /* TODO */

		#else

			free(mem);

		#endif
	}
}

