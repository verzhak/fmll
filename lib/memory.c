
#include "memory.h"

void *** fmll_alloc_3D(unsigned height, unsigned width, unsigned dim, unsigned elem_size)
{
	fmll_try;

		unsigned u, v, height_width = height * width, dim_elem_size = dim * elem_size;
		void *** ret = malloc(height * sizeof(void **) + height_width * (sizeof(void *) + dim_elem_size));
	
		fmll_throw_null(ret);

		void *** ppp = ret;
		void ** pp = (void **) (ppp + height);
		void * p = (void *) (pp + height_width);

		for(u = 0; u < height; u++, ppp++)
		{
			(* ppp) = pp;

			for(v = 0; v < width; v++, pp++, p += dim_elem_size)
				(* pp) = p;
		}

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

void ** fmll_alloc_2D(unsigned height, unsigned dim, unsigned elem_size)
{
	fmll_try;

		unsigned u, dim_elem_size = dim * elem_size;
		void ** ret = malloc(height * (sizeof(void *) + dim_elem_size));
	
		fmll_throw_null(ret);

		void ** pp = ret;
		void * p = (void *) (pp + height);

		for(u = 0; u < height; u++, pp++, p += dim_elem_size)
			(* pp) = p;

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

void * fmll_alloc_1D(unsigned dim, unsigned elem_size)
{
	fmll_try;

		void * ret = malloc(dim * elem_size);
	
		fmll_throw_null(ret);

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

void ** fmll_realloc_2D(void ** mem, unsigned old_height, unsigned height, unsigned dim, unsigned elem_size)
{
	fmll_try;

		void ** ret = NULL;

		fmll_throw(old_height > height);
		fmll_throw_null(ret = fmll_alloc_2D(height, dim, elem_size));

		memcpy((void *) (ret + height), (void *) (mem + old_height), old_height * dim * elem_size);
		fmll_free_ND(mem);

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

void * fmll_realloc_1D(void * mem, unsigned old_dim, unsigned dim, unsigned elem_size)
{
	fmll_try;

		void * ret = NULL;

		fmll_throw(old_dim > dim);
		fmll_throw_null(ret = fmll_alloc_1D(dim, elem_size));

		memcpy(ret, mem, old_dim * elem_size);
		fmll_free_ND(mem);

	fmll_catch;

		ret = NULL;

	fmll_finally;

	return ret;
}

void fmll_free_ND(void * mem)
{
	if(mem != NULL)
		free(mem);
}

