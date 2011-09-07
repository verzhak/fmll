
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll/fmll.h>

int xor();
int image_analysis(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
	// return xor();
	return image_analysis(argc, argv);
}

int image_analysis(const int argc, const char * argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "\nДемонстрация перцептрона.\n\nЗапуск программы:\n\nex_perceptron DIR IMAGE_1\n\nГде:\n\n\tDIR - путь и имя каталога, в котором должны содержаться следующие файлы:\n\n\t\tteach.png - файл с изображением, на основании которого будет составлена обучающая выборка (R = 0, G = 237, B = 95 - класс 1; остальное - класс 0);\n\t\ttest.png - файл с изображением, классификация пикселей которого будет выполнена;\n\n\tIMAGE_1 - путь и имя файла, в который будет сохранено результирующее изображение (белый - правильно классифицированные пиксели, отнесенные к классу 1; черный - правильно классифицированные пиксели, отнесенные к классу 0, зеленый и красный - неклассифицированные пиксели, относящиеся к классам 1 и 0 соответственно).\n\n");

		return -1;
	}

#define N_NUM 3

	bool flag;
	char fname[4096];
	unsigned u, v, q, vec_num, vec_per_class, cl_ind, vec_class[5] = {0, 0, 0, 0, 0}, N[N_NUM] = {7, 5, 1};
	double res, norm;

	// ############################################################################ 

	sprintf(fname, "%s/teach.png", argv[1]);
	IplImage * src_teach = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	sprintf(fname, "%s/test.png", argv[1]);
	IplImage * src_test = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	CvSize size_teach = cvGetSize(src_teach), size_test = cvGetSize(src_test);
	IplImage * dst = cvCreateImage(size_test, IPL_DEPTH_8U, 3);
	CvScalar pixel;
	CvScalar pixel_white = {.val = {255, 255, 255, 0}};
	CvScalar pixel_red = {.val = {0, 0, 255, 0}};
	CvScalar pixel_green = {.val = {0, 255, 0, 0}};
	CvScalar pixel_black = {.val = {0, 0, 0, 0}};

	vec_per_class = size_teach.height * size_teach.width / 2000;
	vec_num = vec_per_class * 5;

	double ** x = (double **) fmll_alloc(sizeof(double), 2, vec_num, 3);
	double ** d = (double **) fmll_alloc(sizeof(double), 2, vec_num, 1);
	double ** test_x = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 3);
	double ** test_d = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 1);

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

	// ############################################################################ 

	double (* fun[N_NUM])(double);
	double (* d_fun[N_NUM])(double);

	for(u = 0; u < N_NUM; u++)
	{
		fun[u] = & fmll_tanh;
		d_fun[u] = & fmll_d_tanh;
	}

	srand48(drand48());

	fmll_perceptron * perc = fmll_perceptron_init(3, N_NUM, N, & fmll_weight_init_random_0_1, fun, d_fun);

	// ############################################################################ 

	// fmll_perceptron_teach_gradient_batch(perc, x, d, vec_num, 0, & fmll_timing_next_beta_step_plus_0_01, 0.9,
	//		10000, 0.001, 0.000001);
	// fmll_perceptron_teach_Levenberg_Marquardt(perc, x, d, vec_num, 100000, 5, 100000, 0.0005, 0);
	fmll_perceptron_teach_conjugate_gradient(perc, x, d, vec_num, 100000, 0.0001, 0.001, 0);

	// ############################################################################ 

	double ** deviation = (double **) fmll_alloc(sizeof(double), 2, 1, 2);
	deviation[0][0] = 0.3;
	deviation[0][1] = 0.3;

	unsigned yes = fmll_perceptron_test(perc, test_x, test_d, deviation, size_test.width * size_test.height, NULL, NULL);

	printf("Верно классифицированных пикселей: %u из %u (%.7lf %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	// ############################################################################ 

	fmll_perceptron_save(perc, "perceptron");
	fmll_perceptron_destroy(perc);
	perc = fmll_perceptron_load("perceptron", fun, d_fun);

	for(v = 0, q = 0, yes = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			res = fmll_perceptron_run(perc, test_x[q])[0];

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

			if(flag && norm > - deviation[0][1] && norm < deviation[0][1])
			{
				cvSet2D(dst, v, u, pixel_white);
				yes++;
			}
			else if (! flag && norm > - deviation[0][0] && norm < deviation[0][0])
			{
				cvSet2D(dst, v, u, pixel_black);
				yes++;
			}
			else if(flag)
				cvSet2D(dst, v, u, pixel_red);
			else
				cvSet2D(dst, v, u, pixel_green);
		}

	printf("Верно классифицированных пикселей: %u из %u (%.7lf %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	// ############################################################################ 
	
	fmll_free(deviation);
	fmll_free(x);
	fmll_free(d);
	fmll_free(test_x);
	fmll_free(test_d);
	fmll_perceptron_destroy(perc);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

int xor()
{
	unsigned u, N[3] = {2, 1, 1};
	double ** vec, ** d;
	double (* fun[3])(double) = {& fmll_tanh, & fmll_tanh, & fmll_tanh};
	double (* d_fun[3])(double) = {& fmll_d_tanh, & fmll_d_tanh, & fmll_d_tanh};

	// ############################################################################ 
	
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

	// ############################################################################ 
	
	srand48(drand48());

	fmll_perceptron * perc = fmll_perceptron_init(2, 2, N, & fmll_weight_init_random_0_1, fun, d_fun);

	// ############################################################################ 

	fmll_perceptron_teach_gradient_batch(perc, vec, d, 4, 1, & fmll_timing_next_beta_step_plus_0_1, 0, 1000, 0.001, 0);
	// fmll_perceptron_teach_Levenberg_Marquardt(perc, vec, d, 4, 1000, 2, 1000, 0.001, 0.000000001);
	// fmll_perceptron_teach_conjugate_gradient(perc, vec, d, 4, 1000, 0.00001, 0.001, 0.00000001);
	
	// ############################################################################ 

	fmll_perceptron_save(perc, "perceptron");

	printf("\nXOR:\n\n");

	for(u = 0; u < 4; u++)
		printf("\t[%.0lf, %.0lf] = %.0lf = %lf\n", vec[u][0], vec[u][1], d[u][0], fmll_perceptron_run(perc, vec[u])[0]);

	printf("\n");

	fmll_perceptron_destroy(perc);

	// ############################################################################ 

	if(
		(perc = fmll_perceptron_load("perceptron", fun, d_fun)) != NULL
	  )
	{
		printf("\nXOR after load:\n\n");
		
		fmll_perceptron_save(perc, "perceptron_2");

		for(u = 0; u < 4; u++)
			printf("\t[%.0lf, %.0lf] = %.0lf = %lf\n", vec[u][0], vec[u][1], d[u][0], fmll_perceptron_run(perc, vec[u])[0]);

		printf("\n");
	}

	// ############################################################################ 

	fmll_free(vec);
	fmll_free(d);

	fmll_perceptron_destroy(perc);

	return 0;
}

