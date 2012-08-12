
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll.h>

int xor();
int image_analysis(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
	return xor();
	/* return image_analysis(argc, argv); */
}

int image_analysis(const int argc, const char * argv[])
{
	bool flag;
	char fname[4096];
	unsigned char ** connect;
	unsigned u, v, q, num, vec_num, vec_per_class, cl_ind, yes, vec_class[5];
	double res, norm, param[2], deviation[1] = { 0.3 };
	double ** x, ** d, ** test_x, ** test_d;
	double (** fun)(double);
	double (** d_fun)(double);
	IplImage * src_teach, * src_test, * dst;
	CvSize size_teach, size_test;
	CvScalar pixel, pixel_white, pixel_red, pixel_green, pixel_black;
	fmll_random * rnd;
	fmll_ff * ff;

	/* ############################################################################ */

	if(argc != 3)
	{
		/* У C90 "проблемы" с максимальной длиной строки кода */

		printf("\nДемонстрация сети прямого распространения.\n\nЗапуск программы:\n\nex_ff DIR IMAGE_1\n\n");
		printf("Где:\n\n");
		printf("\tDIR - путь и имя каталога, в котором должны содержаться следующие файлы:\n\n");
		printf("\t\tteach.png - файл с изображением, на основании которого будет составлена обучающая выборка (R = 0, G = 237, B = 95 - класс 1; остальное - класс 0);\n");
		printf("\t\ttest.png - файл с изображением, классификация пикселей которого будет выполнена;\n\n");
		printf("\tIMAGE_1 - путь и имя файла, в который будет сохранено результирующее изображение (белый - правильно классифицированные пиксели, отнесенные к классу 1; ");
		printf("черный - правильно классифицированные пиксели, отнесенные к классу 0, зеленый и красный - неклассифицированные пиксели, относящиеся к классам 1 и 0 соответственно).\n\n");

		return -1;
	}

	/* ############################################################################ */

	memset(vec_class, 0, sizeof(unsigned) * 5);

	sprintf(fname, "%s/teach.png", argv[1]);
	src_teach = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	sprintf(fname, "%s/test.png", argv[1]);
	src_test = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	size_teach = cvGetSize(src_teach);
	size_test = cvGetSize(src_test);
	
	dst = cvCreateImage(size_test, IPL_DEPTH_8U, 3);

	pixel_white.val[0] = 255;
	pixel_white.val[1] = 255;
	pixel_white.val[2] = 255;
	pixel_white.val[3] = 0;

	pixel_red.val[0] = 0;
	pixel_red.val[1] = 0;
	pixel_red.val[2] = 255;
	pixel_red.val[3] = 0;

	pixel_green.val[0] = 0;
	pixel_green.val[1] = 255;
	pixel_green.val[2] = 0;
	pixel_green.val[3] = 0;

	pixel_black.val[0] = 0;
	pixel_black.val[1] = 0;
	pixel_black.val[2] = 0;
	pixel_black.val[3] = 0;

	vec_per_class = size_teach.height * size_teach.width / 2000;
	vec_num = vec_per_class * 5;

	x = (double **) fmll_alloc(sizeof(double), 2, vec_num, 3);
	d = (double **) fmll_alloc(sizeof(double), 2, vec_num, 1);
	test_x = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 3);
	test_d = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 1);

	for(v = 0, q = 0; v < size_teach.height; v++)
		for(u = 0; u < size_teach.width; u++)
		{
			pixel = cvGet2D(src_teach, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				cl_ind = 0;
			else if(pixel.val[0] == 10 && pixel.val[1] == 169 && pixel.val[2] == 203)
				cl_ind = 1;
			else if(pixel.val[0] == 0 && pixel.val[1] == 255 && pixel.val[2] == 255)
				cl_ind = 2;
			else if(pixel.val[0] == 255 && pixel.val[1] == 0 && pixel.val[2] == 12)
				cl_ind = 3;
			else
				cl_ind = 4;

			if(vec_class[cl_ind] < vec_per_class)
			{
				x[q][0] = pixel.val[0] / 255;
				x[q][1] = pixel.val[1] / 255;
				x[q][2] = pixel.val[2] / 255;

				vec_class[cl_ind]++;

				if(cl_ind)
					d[q][0] = 0;
				else
					d[q][0] = 1;

				q++;
			}
		}

	for(v = 0, q = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			pixel = cvGet2D(src_test, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				test_d[q][0] = 1;
			else
				test_d[q][0] = 0;

			test_x[q][0] = pixel.val[0] / 255;
			test_x[q][1] = pixel.val[1] / 255;
			test_x[q][2] = pixel.val[2] / 255;
		}

	cvReleaseImage(& src_teach);
	cvReleaseImage(& src_test);

	/* ############################################################################ */

	num = 16;

	connect = fmll_alloc(sizeof(unsigned char), 2, num, num);

	for(v = 0; v < num; v++)
		for(u = 0; u < num; u++)
			connect[v][u] = 0;

	connect[0][3] = 1;
	connect[0][4] = 1;
	connect[0][5] = 1;
	connect[0][6] = 1;
	connect[0][7] = 1;
	connect[0][8] = 1;
	connect[0][9] = 1;

	connect[1][3] = 1;
	connect[1][4] = 1;
	connect[1][5] = 1;
	connect[1][6] = 1;
	connect[1][7] = 1;
	connect[1][8] = 1;
	connect[1][9] = 1;

	connect[2][3] = 1;
	connect[2][4] = 1;
	connect[2][5] = 1;
	connect[2][6] = 1;
	connect[2][7] = 1;
	connect[2][8] = 1;
	connect[2][9] = 1;

	/* ########################################################################## */

	connect[3][10] = 1;
	connect[3][11] = 1;
	connect[3][12] = 1;
	connect[3][13] = 1;
	connect[3][14] = 1;

	connect[4][10] = 1;
	connect[4][11] = 1;
	connect[4][12] = 1;
	connect[4][13] = 1;
	connect[4][14] = 1;

	connect[5][10] = 1;
	connect[5][11] = 1;
	connect[5][12] = 1;
	connect[5][13] = 1;
	connect[5][14] = 1;

	connect[6][10] = 1;
	connect[6][11] = 1;
	connect[6][12] = 1;
	connect[6][13] = 1;
	connect[6][14] = 1;

	connect[7][10] = 1;
	connect[7][11] = 1;
	connect[7][12] = 1;
	connect[7][13] = 1;
	connect[7][14] = 1;

	connect[8][10] = 1;
	connect[8][11] = 1;
	connect[8][12] = 1;
	connect[8][13] = 1;
	connect[8][14] = 1;

	connect[9][10] = 1;
	connect[9][11] = 1;
	connect[9][12] = 1;
	connect[9][13] = 1;
	connect[9][14] = 1;

	/* ########################################################################## */

	connect[10][15] = 1;
	connect[11][15] = 1;
	connect[12][15] = 1;
	connect[13][15] = 1;
	connect[14][15] = 1;

	/* ############################################################################ */

	fun = fmll_alloc(sizeof(double (*)(double)), 1, num);
	d_fun = fmll_alloc(sizeof(double (*)(double)), 1, num);

	for(u = 0; u < 3; u++)
	{
		fun[u] = & fmll_line;
		d_fun[u] = & fmll_d_line;
	}

	for(u = 3; u < num; u++)
	{
		fun[u] = & fmll_tanh;
		d_fun[u] = & fmll_d_tanh;
	}

	/* ############################################################################ */

	param[0] = -1;
	param[1] = 1;

	rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param, time(NULL));
	ff = fmll_ff_init(num, fun, d_fun);
	fmll_ff_set_connect(ff, connect);
	fmll_ff_weight_init_random(ff, rnd);

	/* ############################################################################ */

	fmll_ff_teach_gradient_batch(ff, x, d, vec_num, 0, & fmll_timing_next_beta_step_plus_0_01, 0.9, 10000, 0.001, 0.000001);

	/* ############################################################################ */

	yes = fmll_ff_test(ff, test_x, test_d, deviation, size_test.width * size_test.height, NULL, NULL);

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */

	fmll_ff_save(ff, "ff");
	fmll_ff_destroy(ff);
	ff = fmll_ff_load("ff", fun, d_fun);

	for(v = 0, q = 0, yes = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			res = fmll_ff_run(ff, test_x[q])[0];

			if(test_d[q][0] == 1)
			{
				norm = res - 1.0;
				flag = true;
			}
			else
			{
				norm = res;
				flag = false;
			}

			if(flag && fabs(norm) <= deviation[0])
			{
				cvSet2D(dst, v, u, pixel_white);
				yes++;
			}
			else if (! flag && fabs(norm) <= deviation[0])
			{
				cvSet2D(dst, v, u, pixel_black);
				yes++;
			}
			else if(flag)
				cvSet2D(dst, v, u, pixel_red);
			else
				cvSet2D(dst, v, u, pixel_green);
		}

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */
	
	fmll_free(connect);
	fmll_free(fun);
	fmll_free(d_fun);
	fmll_free(x);
	fmll_free(d);
	fmll_free(test_x);
	fmll_free(test_d);
	fmll_ff_destroy(ff);
	fmll_random_destroy(rnd);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

int xor()
{
	int ret = 0;
	unsigned char ** connect = NULL;
	unsigned v, num;
	double deviation[1], ** vec, ** d;
	double (** fun)(double) = NULL;
	double (** d_fun)(double) = NULL;
	fmll_ff * ff = NULL;
	fmll_random * rnd = NULL;

	fmll_try;

		num = 5;

		fmll_throw_null(connect = fmll_alloc(sizeof(unsigned char), 2, num, num));
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

		fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_MT19937, FMLL_RANDOM_DISTRIBUTION_UNIFORM, NULL));
		fmll_throw_null(ff = fmll_ff_init(num, fun, d_fun));
		fmll_throw_if(fmll_ff_set_connect(ff, connect));
		fmll_throw_if(fmll_ff_weight_init_random(ff, rnd));

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

		/* ############################################################################ */

		fmll_throw_if(fmll_ff_teach_gradient_batch(ff, vec, d, 4, 1, & fmll_timing_next_beta_step_plus_0_1, 0, 1000, 0.001, 0));

		/* ############################################################################ */

		deviation[0] = 0.1;

		printf("\nXOR (%u from 4):\n\n", fmll_ff_test(ff, vec, d, deviation, 4, NULL, NULL));

		for(v = 0; v < 4; v++)
			printf("\t[%.0f, %.0f] = %.0f = %f\n", vec[v][0], vec[v][1], d[v][0], fmll_ff_run(ff, vec[v])[0]);

		/* ############################################################################ */

		fmll_throw_if(fmll_ff_save(ff, "ff"));

		fmll_ff_destroy(ff);
		ff = NULL;

		fmll_throw_null(ff = fmll_ff_load("ff", fun, d_fun));

		/* ############################################################################ */

		printf("\nXOR after load (%u from 4):\n\n", fmll_ff_test(ff, vec, d, deviation, 4, NULL, NULL));

		for(v = 0; v < 4; v++)
			printf("\t[%.0f, %.0f] = %.0f = %f\n", vec[v][0], vec[v][1], d[v][0], fmll_ff_run(ff, vec[v])[0]);

		printf("\n");

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(vec);
		fmll_free(d);
		fmll_free(connect);
		fmll_free(fun);
		fmll_free(d_fun);
		fmll_random_destroy(rnd);
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

