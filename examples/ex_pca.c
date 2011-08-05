
#include <stdio.h>
#include <stdlib.h>

#include <fmll/fmll.h>

int main()
{
	unsigned u, v, vec_num = 1000, dim = 3;
	const double * y;
	double ** vec = (double **) fmll_alloc_2D(vec_num, dim, sizeof(double));

	srand48(drand48());

	for(u = 0; u < vec_num; u++)
		for(v = 0; v < dim; v++)
			vec[u][v] = mrand48() % 10000 + drand48();

	fmll_pca * pca = fmll_pca_init(dim, 3, & fmll_weight_init_random_0_00001);
	fmll_pca_so(pca, vec, vec_num, 0.00001);

	printf("\nW = [ ");

	for(u = 0; u < dim; u ++)
	{
		for(v = 0; v < dim - 1; v ++)
			printf("%.11lf, ", pca->w[u][v]);

		if(u == dim - 1)
			printf("%.11lf]\n\n", pca->w[dim - 1][dim - 1]);
		else
			printf("%.11lf; ", pca->w[u][dim - 1]);
	}

	for(u = 0; u < dim; u ++)
	{
		y = fmll_pca_run(pca, pca->w[u]);

		printf("%u: ", u);

		for(v = 0; v < dim; v ++)
			printf("%.11lf ", pca->w[u][v] / y[v]);

		printf("\n");
	}

	fmll_pca_save(pca, "pca");

	fmll_pca_destroy(pca);
	pca = NULL;

	pca = fmll_pca_load("pca");

	printf("\nW1 = [ ");

	for(u = 0; u < dim; u ++)
	{
		for(v = 0; v < dim - 1; v ++)
			printf("%.11lf, ", pca->w[u][v]);

		if(u == dim - 1)
			printf("%.11lf]\n\n", pca->w[dim - 1][dim - 1]);
		else
			printf("%.11lf; ", pca->w[u][dim - 1]);
	}

	fmll_pca_destroy(pca);
	fmll_free_ND(vec);

	return 0;
}

