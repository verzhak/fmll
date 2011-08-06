
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll/fmll.h>

int xor();
int image_analysis(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
	return xor();
	// return image_analysis(argc, argv);
}

int image_analysis(const int argc, const char * argv[])
{
	if(argc != 4)
	{
		printf("\nДемонстрация перцептрона.\n\nЗапуск программы:\n\nex_perceptron IMAGE_1 IMAGE_2 IMAGE_3\n\nГде:\n\n\tIMAGE_1 - путь и имя файла с изображением, на основании которого будет составлена обучающая выборка (R = 0, G = 237, B = 95 - класс 1; остальное - класс 0);\n\tIMAGE_2 - путь и имя файла с изображением, классификация пикселей которого будет выполнена;\n\tIMAGE_3 - путь и имя файла, в который будет сохранено результирующее изображение (белый - правильно классифицированные пиксели, отнесенные к классу 1; черный - правильно классифицированные пиксели, отнесенные к классу 0, зеленый и красный - неклассифицированные пиксели, относящиеся к классам 1 и 0 соответственно).\n\n");

		return 0;
	}

#define N_NUM 3

	bool flag;
	uint16_t N[N_NUM] = {7, 5, 1};
	uint32_t u, v, q;
	double res, t_x[3];
	double (* fun[N_NUM])(double);
	double (* d_fun[N_NUM])(double);

	for(u = 0; u < N_NUM; u++)
	{
		fun[u] = & fmll_tanh;
		d_fun[u] = & fmll_d_tanh;
	}

	srand48(drand48());

	IplImage * src_teach = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	IplImage * src_test = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);
	CvSize size_teach = cvGetSize(src_teach), size_test = cvGetSize(src_test);
	IplImage * dst = cvCreateImage(size_test, IPL_DEPTH_8U, 3);
	CvScalar pixel;
	CvScalar pixel_white = {.val = {255, 255, 255, 0}};
	CvScalar pixel_red = {.val = {0, 0, 255, 0}};
	CvScalar pixel_green = {.val = {0, 255, 0, 0}};
	CvScalar pixel_black = {.val = {0, 0, 0, 0}};

	double ** x = (double **) fmll_alloc_2D(size_teach.height * size_teach.width, 3, sizeof(double));
	double ** d = (double **) fmll_alloc_2D(size_teach.height * size_teach.width, 1, sizeof(double));


	for(v = 0, q = 0; v < size_teach.height; v++)
		for(u = 0; u < size_teach.width; u++, q++)
		{
			pixel = cvGet2D(src_teach, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				d[q][0] = 1;
			else
				d[q][0] = 0;

			x[q][0] = pixel.val[0] / 255;
			x[q][1] = pixel.val[1] / 255;
			x[q][2] = pixel.val[2] / 255;
		}

	fmll_perceptron * perc = fmll_perceptron_init(3, N_NUM, N, & fmll_weight_init_random_0_1, fun, d_fun);

	fmll_perceptron_teach_gradient_batch(perc, x, d, size_teach.height * size_teach.width, 0, & fmll_timing_next_beta_step_plus_0_01, 0.9,
			10000, 0.001, 0.000001);
	// fmll_perceptron_teach_Levenberg_Marquardt(perc, x, d, size_teach.height * size_teach.width, 100000, 5, 100000, 0.0005, 0);
	// fmll_perceptron_teach_conjugate_gradient(perc, x, d, size_teach.height * size_teach.width, 100000, 0.0001, 0.001, 0);

	for(v = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++)
		{
			pixel = cvGet2D(src_test, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				flag = true;
			else
				flag = false;

			t_x[0] = pixel.val[0] / 255;
			t_x[1] = pixel.val[1] / 255;
			t_x[2] = pixel.val[2] / 255;
			res = fmll_perceptron_run(perc, t_x)[0];

			if(flag)
			{
				if(res > 0.7)
					cvSet2D(dst, v, u, pixel_white);
				else
					cvSet2D(dst, v, u, pixel_green);
			}
			else
			{
				if(res < 0.3)
					cvSet2D(dst, v, u, pixel_black);
				else
					cvSet2D(dst, v, u, pixel_red);
			}
		}

	fmll_perceptron_save(perc, "perceptron");
	
	fmll_free_ND(x);
	fmll_free_ND(d);
	fmll_perceptron_destroy(perc);

	cvSaveImage(argv[3], dst, NULL);

	cvReleaseImage(& src_teach);
	cvReleaseImage(& src_test);
	cvReleaseImage(& dst);

	return 0;
}

int xor()
{
	uint16_t N[3] = {2, 1, 1};
	uint32_t u;
	double ** vec, ** d;
	double (* fun[3])(double) = {& fmll_tanh, & fmll_tanh, & fmll_tanh};
	double (* d_fun[3])(double) = {& fmll_d_tanh, & fmll_d_tanh, & fmll_d_tanh};

	fmll_perceptron * perc = fmll_perceptron_init(2, 2, N, & fmll_weight_init_random_0_1, fun, d_fun);

	vec = (double **) fmll_alloc_2D(4, 2, sizeof(double));
	d = (double **) fmll_alloc_2D(4, 1, sizeof(double));

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

	fmll_perceptron_teach_gradient_batch(perc, vec, d, 4, 1, & fmll_timing_next_beta_step_plus_0_1, 0, 10000, 0.001, 0);
	// fmll_perceptron_teach_Levenberg_Marquardt(perc, vec, d, 4, 1000, 2, 1000, 0.001, 0.000000001);
	// fmll_perceptron_teach_conjugate_gradient(perc, vec, d, 4, 1000, 0.00001, 0.001, 0.00000001);

	fmll_perceptron_save(perc, "perceptron");

	printf("\nXOR:\n\n");

	for(u = 0; u < 4; u++)
		printf("\t[%.0lf, %.0lf] = %.0lf = %lf\n", vec[u][0], vec[u][1], d[u][0], fmll_perceptron_run(perc, vec[u])[0]);

	printf("\n");

	fmll_perceptron_destroy(perc);

	perc = NULL;

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

	fmll_free_ND(vec);
	fmll_free_ND(d);

	fmll_perceptron_destroy(perc);

	return 0;
}

