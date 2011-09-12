
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_statistics_double.h>

#include <fmll.h>

int main(const int argc, const char * argv[])
{
	unsigned u, v, t, vec_num = 10000, dim = 3, num = 3;
	double norm, sum, eigen[dim], ** vec = (double **) fmll_alloc(sizeof(double), 2, vec_num, dim);

	srand48(drand48());

	for(u = 0; u < vec_num; u++)
		for(v = 0; v < dim; v++)
			// vec[u][v] = (mrand48() % 65536 + drand48()) / 65536;
			vec[u][v] = drand48();
	
	fmll_centering(vec, vec_num, dim);

	fmll_pca * pca = fmll_pca_init(dim, num, & fmll_weight_init_random_0_01);
	fmll_pca_so(pca, vec, vec_num, 0.0001, & fmll_timing_next_beta_step_0, 0.0000001, eigen);

	printf("\nW = [ ");

	for(u = 0; u < num; u ++)
	{
		for(v = 0; v < dim - 1; v ++)
			printf("%.11lf, ", pca->w[u][v]);

		if(u == num - 1)
			printf("%.11lf]\n\n", pca->w[num - 1][dim - 1]);
		else
			printf("%.11lf; ", pca->w[u][dim - 1]);
	}

	gsl_vector * eval = gsl_vector_alloc(dim);
	gsl_matrix * cov = gsl_matrix_alloc(dim, dim), * tcov = gsl_matrix_alloc(dim, dim),* evec = gsl_matrix_alloc(dim, dim);

	for(u = 0; u < dim; u ++)
		for(v = 0; v < dim; v ++)
		{
			gsl_matrix_set(cov, u, v,
				gsl_stats_covariance((double *) (vec + vec_num) + u, dim, (double *) (vec + vec_num) + v, dim, vec_num));
			gsl_matrix_set(tcov, u, v, gsl_matrix_get(cov, u, v));
		}

	gsl_matrix_scale(cov, (vec_num - 1.0) / vec_num);
	gsl_matrix_scale(tcov, (vec_num - 1.0) / vec_num);

	gsl_eigen_symmv_workspace * ws = gsl_eigen_symmv_alloc(dim);
	gsl_eigen_symmv(tcov, eval, evec, ws); // Значения элементов матрицы tcov изменяются в процессе расчета ее собственных значений и собственных векторов
	gsl_eigen_symmv_free(ws);

	printf("Собственные числа (вычисленные с помощью библиотеки GSL): ");

	for(u = 0; u < dim; u ++)
		printf("%.11lf ", gsl_vector_get(eval, u));

	printf("\n");

	printf("Собственные числа (вычисленные с помощью библиотеки FMLL): ");

	for(u = 0; u < dim; u ++)
		printf("%.11lf ", eigen[u]);

	printf("\n");

	for(u = 0; u < num; u ++)
	{
		printf("\n%u:\n\n\tСобственное число = ", u);

		for(v = 0; v < dim; v ++)
		{
			for(t = 0, sum = 0; t < dim; t ++)
				sum += pca->w[u][t] * gsl_matrix_get(cov, v, t);

			printf("%lf ", sum / pca->w[u][v]);
		}

		for(v = 0, norm = 0; v < dim; v ++)
			norm += pca->w[u][v] * pca->w[u][v];

		printf("\n\tНорма = %.11lf\n\n\tСвязь с собственными векторами, вычисленными с помощью библиотеки GSL:\n\n\t\t", sqrt(norm));

		for(v = 0; v < dim; v ++)
		{
			for(t = 0; t < dim; t ++)
				printf("%lf ", pca->w[u][t] / gsl_matrix_get(evec, t, v));

			printf("\n\t\t");
		}
	}

	printf("\n\n");

	gsl_vector_free(eval);
	gsl_matrix_free(tcov);
	gsl_matrix_free(cov);
	gsl_matrix_free(evec);

	fmll_pca_save(pca, "pca");

	fmll_pca_destroy(pca);
	pca = NULL;

	pca = fmll_pca_load("pca");

	printf("\nW = [ ");

	for(u = 0; u < num; u ++)
	{
		for(v = 0; v < dim - 1; v ++)
			printf("%.11lf, ", pca->w[u][v]);

		if(u == num - 1)
			printf("%.11lf]\n\n", pca->w[num - 1][dim - 1]);
		else
			printf("%.11lf; ", pca->w[u][dim - 1]);
	}

	for(u = 0; u < num; u ++)
	{
		for(v = 0, norm = 0; v < dim; v ++)
			norm += pca->w[u][v] * pca->w[u][v];

		printf("%u: норма = %lf\n", u, sqrt(norm));
	}

	printf("\n");

	fmll_pca_destroy(pca);
	fmll_free(vec);

	return 0;
}

