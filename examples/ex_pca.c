
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_statistics_double.h>

#include <fmll/fmll.h>

int mnist(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
//	return mnist(argc, argv);

	unsigned u, v, t, vec_num = 10000, dim = 3, num = 3;
	double norm, sum, eigen[dim], ** vec = (double **) fmll_alloc_2D(vec_num, dim, sizeof(double));

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
	fmll_free_ND(vec);

	return 0;
}

int mnist(const int argc, const char * argv[])
{
	/*
	 * Использована база изображений "MNIST - database of handwritten digits" (http://yann.lecun.com/exdb/mnist/index.html), спасибо Яну ЛеКунну (Yann LeCun)!
	 */

	fmll_try;

		int ret = 0;
		uint8_t buf[400];
		unsigned v, u, dim, w_num, vec_num;
		uint32_t height[2], width[2], num[2], magic[2];
		double * eigen = NULL, ** vec = NULL;
		FILE * fl[2] = {NULL, NULL};
		fmll_pca * pca = NULL;

		if(argc != 3)
		{
		//	fprintf(stderr, "\nДемонстрация перцептрона на изображениях из базы MNIST.\n\nЗапуск программы:\n\nex_perceptron TRAIN_DIR TEST_DIR\n\nГде:\n\n\tTRAIN_DIR - путь и имя каталога, в котором сохранены изображения, составляющие обучающую выборку изображений (изображения должны быть пронумерованы от 0 до N, где N - количество строк в файле labels, расположенного в том же каталоге; файл labels - файл меток исходных изображений (одна строка - одна метка));\n\tTEST_DIR - путь и имя каталога, в котором сохранены изображения, составляющие тестовую выборку изображений (принцип организации изображений в каталоге TEST_DIR тот же, что и принцип организации изображений в каталоге TRAIN_DIR).\n\n");
		
			fmll_throw(1);
		}

#define TO_LE(x) \
        x = ((x & 0xFF000000) >> 24) + ((x & 0xFF0000) >> 8) + ((x & 0xFF00) << 8) + ((x & 0xFF) << 24);

		for(u = 0; u < 2; u++)
		{
			fmll_throw_null((fl[u] = fopen(argv[u + 1], "r")));

			fread(magic + u, 4, 1, fl[u]);
			fread(num + u, 4, 1, fl[u]);
			
			TO_LE((* (magic + u)));
			TO_LE((* (num + u)));
		}

		fmll_throw((magic[0] != 2051 || magic[1] != 2051));

		fread(height, 4, 1, fl[0]);
		fread(height + 1, 4, 1, fl[1]);
		fread(width, 4, 1, fl[0]);
		fread(width + 1, 4, 1, fl[1]);

		TO_LE(height[0]);
		TO_LE(height[1]);
		TO_LE(width[0]);
		TO_LE(width[1]);

		w_num = dim = height[0] * width[0];
		vec_num = 1000; //num[0];

		fmll_throw(height[0] != height[1] || width[0] != width[1]);
		fmll_throw_null((eigen = fmll_alloc_1D(dim, sizeof(double))));
		fmll_throw_null((vec = (double **) fmll_alloc_2D(vec_num, dim, sizeof(double))));

		for(v = 0; v < vec_num; v++)
		{
			if(v % 1000 == 999)
				printf("Считывание: %u из %u (%lf %%)\n", v + 1, vec_num, (100.0 * (v + 1)) / vec_num);

			fread(buf, 1, 400, fl[0]);

			for(u = 0; u < dim; u++)
			{
				if(buf[u] > 127)
					vec[v][u] = 1;
				else
					vec[v][u] = 0;
			}
		}

		w_num = 50;

		fmll_centering(vec, vec_num, dim);
	
		fmll_throw_null((pca = fmll_pca_init(dim, w_num, & fmll_weight_init_random_0_01)));
		fmll_pca_so(pca, vec, vec_num, 0.0001, & fmll_timing_next_beta_step_0, 0.0000001, eigen);
		fmll_pca_save(pca, "pca");

		for(v = 0; v < dim; v++)
			printf("EV %u = %.11lf\n", v, eigen[v]);

	fmll_catch;

		ret = -1;

	fmll_finally;

		for(u = 0; u < 2; u++)
			if(fl[u] != NULL)
				fclose(fl[u]);

		fmll_free_ND(eigen);
		fmll_free_ND(vec);

		fmll_pca_destroy(pca);

	return ret;
}
