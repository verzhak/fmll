
#include "math/matrix/matrix.h"

int fmll_math_matrix_init_main_diag(double ** M, const double value, const unsigned rows, const unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			M[v][u] = (v == u) ? value : 0;

	return 0;
}

int fmll_math_matrix_copy(const double ** F, double ** T, const unsigned rows, const unsigned cols)
{
	unsigned v, u;
	
	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			T[v][u] = F[v][u];

	return 0;
}

int fmll_math_matrix_transpose(const double ** M, double ** R, const unsigned rows, const unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[u][v] = M[v][u];

	return 0;
}

double fmll_math_matrix_euclid_norm(const double ** M, const unsigned rows, const unsigned cols)
{
	unsigned v, u;
	double norm = 0;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			norm += M[v][u] * M[v][u];

	return sqrt(norm);
}

int fmll_math_matrix_sum(const double alpha_1, const double ** M1, const double alpha_2, const double ** M2, double ** R, const unsigned rows, const unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[v][u] = alpha_1 * M1[v][u] + alpha_2 * M2[v][u];

	return 0;
}

int fmll_math_matrix_mult_scalar(const double ** M, const double value, double ** R, const unsigned rows, const unsigned cols)
{
	unsigned v, u;

	for(v = 0; v < rows; v++)
		for(u = 0; u < cols; u++)
			R[v][u] = value * M[v][u];

	return 0;
}

int fmll_math_matrix_mult_vector(const double ** M, const double * V, double * R, const unsigned rows, const unsigned cols)
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

int fmll_math_matrix_mult(const double ** M1, const double ** M2, double ** R, const unsigned rows_M1, const unsigned cols_M1, const unsigned cols_M2)
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

int fmll_math_matrix_lu(const double ** M, double ** L, double ** U, const unsigned rows)
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

int fmll_math_matrix_lup(const double ** M, double ** L, double ** U, double ** P, const unsigned rows)
{
	int ret = 0;
	unsigned v, u, tv, max_ind;
	double max, temp;

	fmll_try;

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

			fmll_throw_if(max < 0.00000001);

			if(max_ind != v)
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

	fmll_catch;

		ret = -1;

	fmll_finally;

	return ret;
}

int fmll_math_matrix_inv(const double ** M, double ** MI, const unsigned rows)
{
	int ret = 0;
	double ** L, ** LI, ** U, ** UI, ** P;

	fmll_try;

		L = LI = U = UI = P = NULL;

		fmll_throw_null(L = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(LI = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(U = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(UI = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(P = fmll_alloc(sizeof(double), 2, rows, rows));

		fmll_throw_if(fmll_math_matrix_lup((const double **) M, L, U, P, rows));
		fmll_throw_if(fmll_math_matrix_inv_low_tr((const double **) L, LI, rows));
		fmll_throw_if(fmll_math_matrix_inv_high_tr((const double **) U, UI, rows));
		fmll_throw_if(fmll_math_matrix_mult((const double **) UI, (const double **) LI, L, rows, rows, rows));
		fmll_throw_if(fmll_math_matrix_mult((const double **) L, (const double **) P, MI, rows, rows, rows));

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

int fmll_math_matrix_inv_high_tr(const double ** M, double ** MI, const unsigned rows)
{
	int u, v, t;

	for(v = 0; v < rows; v++)
		for(u = 0; u < rows; u++)
			MI[v][u] = (v == u) ? 1 : 0;

	for(v = rows - 1; v >= 0; v--)
		for(u = v; u < rows; u++)
		{
			MI[v][u] /= M[v][v];

			for(t = v - 1; t >= 0; t--)
				MI[t][u] -= M[t][v] * MI[v][u];
		}

	return 0;
}

int fmll_math_matrix_inv_low_tr(const double ** M, double ** MI, const unsigned rows)
{
	int u, v, t;

	for(v = 0; v < rows; v++)
		for(u = 0; u < rows; u++)
			MI[v][u] = (v == u) ? 1 : 0;

	for(v = 0; v < rows; v++)
		for(u = 0; u <= v; u++)
		{
			MI[v][u] /= M[v][v];

			for(t = v + 1; t < rows; t++)
				MI[t][u] -= M[t][v] * MI[v][u];
		}

	return 0;
}

int fmll_math_matrix_hessenberg(const double ** M, double ** H, const unsigned rows)
{
	int ret = 0;
	const unsigned to_u = rows - 2;
	unsigned v, u, t, rows_u_1;
	double coef, norm_w, * w = NULL, ** Z = NULL, ** T = NULL;

	fmll_try;

		fmll_throw_if(rows < 2);

		fmll_throw_null(w = fmll_alloc(sizeof(double), 1, rows - 1));
		fmll_throw_null(Z = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(T = fmll_alloc(sizeof(double), 2, rows, rows));

		for(v = 0; v < rows; v++)
			for(u = 0; u < rows; u++)
				H[v][u] = M[v][u];

		for(u = 0; u < to_u; u++)
		{
			rows_u_1 = rows - u - 1;

			for(v = 0; v < rows_u_1; v++)
				w[v] = H[v + u + 1][u];

			fmll_throw_if((norm_w = fmll_math_matrix_euclid_norm((const double **) & w, 1, rows_u_1)) < 0);
			w[0] += (w[0] < 0 ? -1 : 1) * norm_w;
			fmll_throw_if((coef = fmll_math_matrix_euclid_norm((const double **) & w, 1, rows_u_1)) < 0);

			if(coef != 0)
			{
				coef = 2 / (coef * coef);

				fmll_throw_if(fmll_math_matrix_init_main_diag(Z, 1, rows, rows));

				for(v = u + 1; v < rows; v++)
					for(t = u + 1; t < rows; t++)
						Z[v][t] -= coef * w[v - u - 1] * w[t - u - 1];

				fmll_throw_if(fmll_math_matrix_mult((const double **) H, (const double **) Z, T, rows, rows, rows));
				fmll_throw_if(fmll_math_matrix_mult((const double **) Z, (const double **) T, H, rows, rows, rows));
			}
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(w);
		fmll_free(Z);
		fmll_free(T);

	return ret;
}

int fmll_math_matrix_shur(const double ** M, double ** S, const unsigned rows, const double precision)
{
	int ret = 0;
	const unsigned rows_2 = rows - 2;
	unsigned u;
	double d, cur_prec, ** H = NULL, ** Q = NULL, ** R = NULL;

	fmll_try;

		fmll_throw_if(rows < 2 || precision < 0);

		fmll_throw_null(H = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(Q = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(R = fmll_alloc(sizeof(double), 2, rows, rows));

		fmll_throw_if(fmll_math_matrix_hessenberg((const double **) M, H, rows));
		fmll_throw_if(fmll_math_matrix_copy((const double **) M, S, rows, rows));

		do
		{
			fmll_throw_if(fmll_math_matrix_QR((const double **) S, Q, R, rows, rows));
			fmll_throw_if(fmll_math_matrix_mult((const double **) R, (const double **) Q, S, rows, rows, rows));

			for(u = 0, cur_prec = 0; u < rows_2; u++)
			{
				d = fabs(S[u + 1][u] * S[u + 2][u + 1]);

				if(d > cur_prec)
					cur_prec = d;
			}
		}
		while(cur_prec > precision);

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(H);
		fmll_free(Q);
		fmll_free(R);

	return ret;
}

int fmll_math_matrix_QR(const double ** M, double ** Q, double ** R, const unsigned rows, const unsigned cols)
{
	int ret = 0;
	unsigned v, u, t, rows_u;
	double norm_w, coef, * w = NULL, ** Z = NULL, ** tZ = NULL, ** tQ = NULL, ** tR = NULL;

	fmll_try;

		fmll_throw_null(w = fmll_alloc(sizeof(double), 1, rows));
		fmll_throw_null(Z = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(tZ = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_null(tR = fmll_alloc(sizeof(double), 2, rows, cols));

		fmll_throw_if(fmll_math_matrix_copy((const double **) M, R, rows, cols));

		for(u = 0, rows_u = rows; u < cols && rows_u > 1; u++, rows_u--)
		{
			/* ############################################################################ */
			/* Обнуление поддиагональных элементов u-го столбца */

			for(v = 0; v < rows_u; v++)
				w[v] = R[v + u][u];

			fmll_throw_if((norm_w = fmll_math_matrix_euclid_norm((const double **) & w, 1, rows_u)) < 0);
			w[0] += w[0] < 0 ? - norm_w : norm_w;
			fmll_throw_if((coef = fmll_math_matrix_euclid_norm((const double **) & w, 1, rows_u)) < 0);

			if(coef != 0)
			{
				coef = 2 / (coef * coef);

				fmll_throw_if(fmll_math_matrix_init_main_diag(Z, 1, rows, rows));

				for(v = u; v < rows; v++)
					for(t = u; t < rows; t++)
						Z[v][t] -= coef * w[v - u] * w[t - u];
			}

			/* ############################################################################ */
			/* Итерация расчета матрицы R */

			fmll_throw_if(fmll_math_matrix_copy((const double **) R, tR, rows, cols));
			fmll_throw_if(fmll_math_matrix_mult((const double **) Z, (const double **) tR, R, rows, rows, cols));

			/* ############################################################################ */
			/* Итерация расчета матрицы Q */

			if(u)
			{
				fmll_throw_if(fmll_math_matrix_transpose((const double **) Z, tZ, rows, rows));
				fmll_throw_if(fmll_math_matrix_copy((const double **) Q, Z, rows, rows));
				fmll_throw_if(fmll_math_matrix_mult((const double **) Z, (const double **) tZ, Q, rows, rows, rows));
			}
			else
				fmll_throw_if(fmll_math_matrix_transpose((const double **) Z, Q, rows, rows));
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(w);
		fmll_free(Z);
		fmll_free(tZ);
		fmll_free(tQ);
		fmll_free(tR);
	
	return ret;
}

int fmll_math_matrix_eigen(const double ** M, double * eigen_real, double * eigen_complex, const unsigned rows, const double precision_shur)
{
	int ret = 0;
	const unsigned rows_1 = rows - 1;
	unsigned u = 0;
	double s, ** S = NULL;
		
	fmll_try;

		fmll_throw_if(rows < 1);

		fmll_throw_null(S = fmll_alloc(sizeof(double), 2, rows, rows));
		fmll_throw_if(fmll_math_matrix_shur((const double **) M, S, rows, precision_shur));

		while(u < rows)
		{
			if(u == rows_1 || fabs(S[u + 1][u]) < 1E-10)
			{
				eigen_real[u] = S[u][u];

				if(eigen_complex != NULL)
					eigen_complex[u] = 0;

				u++;
			}
			else
			{
				s = S[u][u] + S[u + 1][u + 1];

				eigen_real[u] = s / 2;
				eigen_real[u + 1] = eigen_real[u];

				if(eigen_complex != NULL)
				{
					eigen_complex[u] = sqrt(fabs(s * s - 4 * (S[u][u] * S[u + 1][u + 1] - S[u + 1][u] * S[u][u + 1]))) / 2;
					eigen_complex[u + 1] = - eigen_complex[u];
				}

				u += 2;
			}
		}

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(S);

	return ret;
}

