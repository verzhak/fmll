
#include "ann/cnn/cnn.h"

fmll_cnn * fmll_cnn_init(const unsigned rows, const unsigned cols, double (* fun)(const double), double (* A)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned),
		double (* B)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned),
		double (* I)(const double **, const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned))
{
	unsigned v, u;
	fmll_cnn * cnn = NULL;

	fmll_try;

		fmll_throw_if(! rows || ! cols);
		fmll_throw_null(cnn = fmll_alloc(sizeof(fmll_cnn), 1, 1));

		cnn->rows = rows;
		cnn->cols = cols;
		cnn->Y = NULL;
		cnn->net = NULL;
		cnn->fun = fun;
		cnn->A = A;
		cnn->B = B;
		cnn->I = I;

		fmll_throw_null(cnn->Y = (double **) fmll_alloc(sizeof(double), 2, rows, cols));
		fmll_throw_null(cnn->net = (double **) fmll_alloc(sizeof(double), 2, rows, cols));

	fmll_catch;

		fmll_cnn_destroy(cnn);
		cnn = NULL;

	fmll_finally;

	return cnn;
}

void fmll_cnn_destroy(fmll_cnn * cnn)
{
	if(cnn != NULL)
	{
		fmll_free(cnn->Y);
		fmll_free(cnn->net);
		fmll_free(cnn);
	}
}

const double ** fmll_cnn_run(fmll_cnn * cnn, const double ** M, const unsigned max_iter)
{
	const unsigned rows = cnn->rows, cols = cnn->cols;
	const unsigned rows_per_thread = rows / omp_get_max_threads();
	unsigned iter, v, u, from_v, to_v, t_ind;
	double ** Y = cnn->Y, ** net = cnn->net;
	double (* fun)(const double) = cnn->fun;
	double (* A)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned) = cnn->A;
	double (* B)(const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned) = cnn->B;
	double (* I)(const double **, const double **, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const unsigned) = cnn->I;

	fmll_try;

		fmll_throw_if(! max_iter);

		fmll_throw_if(fmll_math_matrix_init_fill(Y, 0, rows, cols));
		fmll_throw_if(fmll_math_matrix_init_fill(net, 0, rows, cols));

		for(iter = 0; iter < max_iter; iter++)
		{
			#pragma omp parallel private(t_ind, v, u, from_v, to_v) default(shared)
			{
				t_ind = omp_get_thread_num();
				from_v = t_ind * rows_per_thread;
				to_v = (t_ind + 1) * rows_per_thread;

				if(to_v > rows)
					to_v = rows;

				for(v = from_v; v < to_v; v++)
					for(u = 0; u < cols; u++)
						net[v][u] = - net[v][u] + (* A)((const double **) Y, v, u, rows, cols, iter, max_iter) + (* B)(M, v, u, rows, cols, iter, max_iter) + (* I)(M, (const double **) Y, v, u, rows, cols, iter, max_iter);

				#pragma omp barrier

				for(v = from_v; v < to_v; v++)
					for(u = 0; u < cols; u++)
						Y[v][u] = (* fun)(net[v][u]);
			}

			fmll_print("Iteration = %u from %u (%.5f %%)\n", iter + 1, max_iter, (100.0 * (iter + 1)) / max_iter);
		}

	fmll_catch;

		Y = NULL;

	fmll_finally;

	return (const double **) Y;
}

