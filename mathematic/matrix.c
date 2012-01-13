
#include "mathematic/matrix.h"

int fmll_math_matrix_init_main_diag(double ** M, double value, unsigned rows, unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			M[v][u] = v == u ? value : 0;

	return 0;
}

int fmll_math_matrix_transpose(double ** M, double ** R, unsigned rows, unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[u][v] = M[v][u];

	return 0;
}

int fmll_math_matrix_sum(double ** M1, double ** M2, double ** R, unsigned rows, unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[v][u] = M1[v][u] + M2[v][u];

	return 0;
}

int fmll_math_matrix_mult_scalar(double ** M, double value, double ** R, unsigned rows, unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[v][u] = value * M[v][u];

	return 0;
}

int fmll_math_matrix_mult_vector(double ** M, double * V, double * R, unsigned rows, unsigned cols)
{
	unsigned u, v;
	double sum;

	for(v = 0; v < rows; v++)
	{
		for(u = 0, sum = 0; u < cols; u++)
			sum += M[v][u] * V[u];

		R[v] = sum;
	}

	return 0;
}

int fmll_math_matrix_mult(double ** M1, double ** M2, double ** R, unsigned rows_M1, unsigned cols_M1, unsigned cols_M2)
{
	unsigned u, v, t;
	double sum;

	for(v = 0; v < rows_M1; v++)
		for(u = 0; u < cols_M2; u++)
		{
			for(t = 0, sum = 0; t < cols_M1; t++)
				sum += M1[v][t] * M2[t][u];

			R[v][u] = sum;
		}

	return 0;
}

int fmll_math_matrix_lu(double ** M, double ** L, double ** U, unsigned rows)
{
	int u, v, t;

	for(u = 0; u < rows; u++)
	{
		for(v = 0; v <= u; v++)
		{
			U[v][u] = M[v][u];

			for(t = 0; t < v; t++)
				U[v][u] -= L[v][t] * U[t][u];
		}

		for(v = u + 1; v < rows; v++)
			U[v][u] = 0;

		for(v = 0; v < u; v++)
			L[v][u] = 0;

		L[u][u] = 1;

		for(v = u + 1; v < rows; v++)
		{
			L[v][u] = M[v][u];

			for(t = 0; t < u; t++)
				L[v][u] -= L[v][t] * U[t][u];

			L[v][u] /= U[u][u];
		}
	}

	return 0;
}

int fmll_math_matrix_lup(double ** M, double ** L, double ** U, double ** P, unsigned rows)
{
	unsigned v, u, tv, max_ind;
	double max, temp;

	for(v = 0; v < rows; v++)
		for(u = 0; u < rows; u++)
		{
			U[v][u] = M[v][u];
			L[v][u] = 0;
			P[v][u] = v == u ? 1 : 0;
		}

	for(v = 0; v < rows; v++)
	{
		for(tv = v + 1, max_ind = v, max = fabs(U[v][v]); tv < rows; tv++)
			if(fabs(U[tv][v]) > max)
			{
				max_ind = tv;
				max = fabs(U[tv][v]);
			}

		if(max < 0.00000001)
			return -1;
		else if(max_ind != v)
			for(u = 0; u < rows; u++)
			{
				temp = U[max_ind][u];
				U[max_ind][u] = U[v][u];
				U[v][u] = temp;

				temp = L[max_ind][u];
				L[max_ind][u] = L[v][u];
				L[v][u] = temp;

				temp = P[max_ind][u];
				P[max_ind][u] = P[v][u];
				P[v][u] = temp;
			}

		L[v][v] = 1;

		for(tv = v + 1; tv < rows; tv++)
		{
			L[tv][v] = U[tv][v] / U[v][v];

			for(u = v; u < rows; u++)
				U[tv][u] -= L[tv][v] * U[v][u];
		}
	}

	return 0;
}

int fmll_math_matrix_inv(double ** M, double ** MI, unsigned rows)
{
	fmll_try;

		int ret = 0;
		double ** L, ** LI, ** U, ** UI, ** P;

		L = LI = U = UI = P = NULL;

		fmll_throw_null((L = fmll_alloc(sizeof(double), 2, rows, rows)));
		fmll_throw_null((LI = fmll_alloc(sizeof(double), 2, rows, rows)));
		fmll_throw_null((U = fmll_alloc(sizeof(double), 2, rows, rows)));
		fmll_throw_null((UI = fmll_alloc(sizeof(double), 2, rows, rows)));
		fmll_throw_null((P = fmll_alloc(sizeof(double), 2, rows, rows)));

		fmll_throw(fmll_math_matrix_lup(M, L, U, P, rows));
		fmll_throw(fmll_math_matrix_inv_low_tr(L, LI, rows));
		fmll_throw(fmll_math_matrix_inv_high_tr(U, UI, rows));
		fmll_throw(fmll_math_matrix_mult(UI, LI, L, rows, rows, rows));
		fmll_throw(fmll_math_matrix_mult(L, P, MI, rows, rows, rows));

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(L);
		fmll_free(LI);
		fmll_free(U);
		fmll_free(UI);
		fmll_free(P);

	return ret;
}

int fmll_math_matrix_inv_high_tr(double ** M, double ** MI, unsigned rows)
{
	int u, v, t;

	for(v = 0; v < rows; v++)
		for(u = 0; u < rows; u++)
			MI[v][u] = v == u ? 1 : 0;

	for(v = rows - 1; v >= 0; v--)
		for(u = v; u < rows; u++)
		{
			MI[v][u] /= M[v][v];

			for(t = v - 1; t >= 0; t--)
				MI[t][u] -= M[t][v] * MI[v][u];
		}

	return 0;
}

int fmll_math_matrix_inv_low_tr(double ** M, double ** MI, unsigned rows)
{
	int u, v, t;

	for(v = 0; v < rows; v++)
		for(u = 0; u < rows; u++)
			MI[v][u] = v == u ? 1 : 0;

	for(v = 0; v < rows; v++)
		for(u = 0; u <= v; u++)
		{
			MI[v][u] /= M[v][v];

			for(t = v + 1; t < rows; t++)
				MI[t][u] -= M[t][v] * MI[v][u];
		}

	return 0;
}

int fmll_math_matrix_eigen(double ** M, double * eigen, unsigned rows)
{
	unsigned v, u;

	// TODO
	
	// ############################################################################ 
	
	printf("eig([");

	for(v = 0; v < rows; v++)
	{
		for(u = 0; u < rows - 1; u++)
			printf("%f, ", M[v][u]);

		printf("%f; ", M[v][rows - 1]);
	}

	printf("])\n");

	// ############################################################################ 

	return 1;
}

