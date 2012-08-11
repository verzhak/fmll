
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <fmll.h>

/* TODO */

int main(const int argc, const char * argv[])
{
	int ret = 0;
	char ** connect = NULL;
	unsigned v, u, num;
	double ** vec, ** d;
	double (** fun)(double) = NULL;
	double (** d_fun)(double) = NULL;
	fmll_ff * ff = NULL;

	fmll_try;

		num = 5;

		fmll_throw_null(connect = fmll_alloc(sizeof(char), 2, num, num));
		fmll_throw_null(fun = fmll_alloc(sizeof(double (*)(double)), 1, num));
		fmll_throw_null(d_fun = fmll_alloc(sizeof(double (*)(double)), 1, num));

		connect[0][0] = 0;
		connect[0][1] = 0;
		connect[0][2] = 1;
		connect[0][3] = 1;
		connect[0][4] = 0;

		connect[1][0] = 0;
		connect[1][1] = 0;
		connect[1][2] = 1;
		connect[1][3] = 1;
		connect[1][4] = 0;

		connect[2][0] = 0;
		connect[2][1] = 0;
		connect[2][2] = 0;
		connect[2][3] = 0;
		connect[2][4] = 1;

		connect[3][0] = 0;
		connect[3][1] = 0;
		connect[3][2] = 0;
		connect[3][3] = 0;
		connect[3][4] = 1;

		connect[4][0] = 0;
		connect[4][1] = 0;
		connect[4][2] = 0;
		connect[4][3] = 0;
		connect[4][4] = 0;

		for(v = 0; v < 2; v++)
		{
			fun[v] = & fmll_line;
			d_fun[v] = & fmll_d_line;
		}

		for(v = 2; v < num; v++)
		{
			fun[v] = & fmll_tanh;
			d_fun[v] = & fmll_d_tanh;
		}

		fmll_throw_null(ff = fmll_ff_init(num, fun, d_fun));
		fmll_throw_if(fmll_ff_set_connect(ff, connect));

		ff->w[0][0] = 0;
		ff->w[0][1] = 0;
		ff->w[0][2] = 1.97644634101368499;
		ff->w[0][3] = 1.10401517146539474;
		ff->w[0][4] = 0;
		ff->b[0] = 0;

		ff->w[1][0] = 0;
		ff->w[1][1] = 0;
		ff->w[1][2] = 1.91489008646816816;
		ff->w[1][3] = 1.08754616599911769;
		ff->w[1][4] = 0;
		ff->b[1] = 0;

		ff->w[2][0] = 0;
		ff->w[2][1] = 0;
		ff->w[2][2] = 0;
		ff->w[2][3] = 0;
		ff->w[2][4] = 1.85159727036234822;
		ff->b[2] = 0.59557139165683570;

		ff->w[3][0] = 0;
		ff->w[3][1] = 0;
		ff->w[3][2] = 0;
		ff->w[3][3] = 0;
		ff->w[3][4] = -1.94625095647850466;
		ff->b[3] = 1.59667395421505454;

		ff->w[4][0] = 0;
		ff->w[4][1] = 0;
		ff->w[4][2] = 0;
		ff->w[4][3] = 0;
		ff->w[4][4] = 0;
		ff->b[4] = 0.79780298488055035;

		/* ############################################################################ */

		vec = (double **) fmll_alloc(sizeof(double), 2, 4, 2);
		d = (double **) fmll_alloc(sizeof(double), 2, 4, 1);

		vec[0][0] = 0;
		vec[0][1] = 0;
		vec[1][0] = 1;
		vec[1][1] = 0;
		vec[2][0] = 0;
		vec[2][1] = 1;
		vec[3][0] = 1;
		vec[3][1] = 1;

		d[0][0] = d[3][0] = 0;
		d[1][0] = d[2][0] = 1;

		printf("\nXOR:\n\n");

		for(v = 0; v < 4; v++)
			printf("\t[%.0f, %.0f] = %.0f = %f\n", vec[v][0], vec[v][1], d[v][0], fmll_ff_run(ff, vec[v])[0]);

		printf("\n");

	/*
TODO
int fmll_ff_save(fmll_ff * ff, const char * fname_prefix);
fmll_ff * fmll_ff_load(const char * fname_prefix);
*/

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(vec);
		fmll_free(d);
		fmll_free(connect);
		fmll_free(fun);
		fmll_free(d_fun);
		fmll_ff_destroy(ff);

	return ret;
}

/*
printf("\nin_dim = %u\nout_dim = %u\n\nIN:\n\n", ff->in_dim, ff->out_dim);

for(v = 0; v < ff->in_dim; v++)
	printf("\t%u\n", ff->in[v]);

printf("\nOUT:\n\n");

for(v = 0; v < ff->out_dim; v++)
	printf("\t%u\n", ff->out[v]);

printf("\n");
*/

