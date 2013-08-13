
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
#if 1

	return xor();

#else
	
	return image_analysis(argc, argv);

#endif
}

int image_analysis(const int argc, const char * argv[])
{
#define N_NUM 3

	bool flag;
	char fname[4096];
	unsigned u, v, q, vec_num, vec_per_class, cl_ind, yes, vec_class[5], N[N_NUM];
	double res, norm, param[2], deviation[1] = { 0.3 };
	double ** x, ** d, ** test_x, ** test_d;
	double (* fun[N_NUM])(double);
	double (* d_fun[N_NUM])(double);
	IplImage * src_teach, * src_test, * dst;
	CvSize size_teach, size_test;
	CvScalar pixel, pixel_white, pixel_red, pixel_green, pixel_black;
	fmll_random * rnd;
	fmll_mlp * mlp;

	/* ############################################################################ */

	if(argc != 3)
	{
		/* У C90 "проблемы" с максимальной длиной строки кода */

		printf("\nДемонстрация перцептрона.\n\nЗапуск программы:\n\nex_mlp DIR IMAGE_1\n\n");
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

	N[0] = 7;
	N[1] = 5;
	N[2] = 1;

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

	for(u = 0; u < N_NUM; u++)
	{
		fun[u] = & fmll_tanh;
		d_fun[u] = & fmll_d_tanh;
	}

	param[0] = -1;
	param[1] = 1;

	rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param, time(NULL));
	mlp = fmll_mlp_init(3, N_NUM, N, fun, d_fun);
	fmll_mlp_weight_init_nguyen_widrow(mlp, rnd);

	/* ############################################################################ */

	/* fmll_mlp_teach_gradient_batch(mlp, (const double **) x, (const double **) d, vec_num, 0, & fmll_timing_next_beta_step_plus_0_01, 0.9, 10000, 0.001, 0.000001); */
	/* fmll_mlp_teach_Levenberg_Marquardt(mlp, (const double **) x, (const double **) d, vec_num, 100000, 5, 100000, 0.0005, 0); */
	fmll_mlp_teach_conjugate_gradient(mlp, (const double **) x, (const double **) d, vec_num, 100000, 0.001, 0.001, 0);

	/* ############################################################################ */

	yes = fmll_mlp_test(mlp, (const double **) test_x, (const double **) test_d, deviation, size_test.width * size_test.height, NULL, NULL);

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */

	fmll_mlp_save(mlp, "mlp");
	fmll_mlp_destroy(mlp);
	mlp = fmll_mlp_load("mlp", fun, d_fun);

	for(v = 0, q = 0, yes = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			res = fmll_mlp_run(mlp, test_x[q])[0];

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
	
	fmll_free(x);
	fmll_free(d);
	fmll_free(test_x);
	fmll_free(test_d);
	fmll_mlp_destroy(mlp);
	fmll_random_destroy(rnd);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

int xor()
{
	unsigned u, N[2] = {2, 1};
	double ** vec, ** d;
	double (* fun[2])(double) = {& fmll_tanh, & fmll_tanh};
	double (* d_fun[2])(double) = {& fmll_d_tanh, & fmll_d_tanh};
	fmll_random * rnd;
	fmll_mlp * mlp;

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
	
	rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_MT19937, FMLL_RANDOM_DISTRIBUTION_UNIFORM, NULL);
	mlp = fmll_mlp_init(2, 2, N, fun, d_fun);
	fmll_mlp_weight_init_nguyen_widrow(mlp, rnd);

	/* ############################################################################ */

	fmll_mlp_teach_gradient_batch(mlp, (const double **) vec, (const double **) d, 4, 1, & fmll_timing_next_beta_step_plus_0_1, 0, 1000, 0.001, 0);
	/* fmll_mlp_teach_Levenberg_Marquardt(mlp, (const double **) vec, (const double **) d, 4, 1000, 2, 1000, 0.001, 0); */
	/* fmll_mlp_teach_conjugate_gradient(mlp, (const double **) vec, (const double **) d, 4, 1000, 0.00001, 0.001, 0); */

	/* ############################################################################ */

	fmll_mlp_save(mlp, "mlp");

	printf("\nXOR:\n\n");

	for(u = 0; u < 4; u++)
		printf("\t[%.0f, %.0f] = %.0f = %f\n", vec[u][0], vec[u][1], d[u][0], fmll_mlp_run(mlp, vec[u])[0]);

	printf("\n");

	fmll_mlp_destroy(mlp);

	/* ############################################################################ */

	if(
		(mlp = fmll_mlp_load("mlp", fun, d_fun)) != NULL
	  )
	{
		printf("\nXOR after load:\n\n");
		
		fmll_mlp_save(mlp, "mlp_2");

		for(u = 0; u < 4; u++)
			printf("\t[%.0f, %.0f] = %.0f = %f\n", vec[u][0], vec[u][1], d[u][0], fmll_mlp_run(mlp, vec[u])[0]);

		printf("\n");
	}

	/* ############################################################################ */

	fmll_free(vec);
	fmll_free(d);

	fmll_mlp_destroy(mlp);
	fmll_random_destroy(rnd);

	return 0;
}

