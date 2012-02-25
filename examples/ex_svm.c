
#include <stdio.h>

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
	char * d, * test_d, fname[4096];
	unsigned u, v, q, vec_per_class, vec_num, cl_ind, yes, vec_class[5];
	double res, ** x, ** test_x;
	double (* K)(const double *, const double *, unsigned);
	IplImage * src_teach, * src_test, * dst;
	CvSize size_teach, size_test;
	CvScalar pixel, pixel_white, pixel_red, pixel_green, pixel_black, pixel_blue;
	fmll_svm * svm;

	/* ############################################################################ */

	if(argc != 3)
	{
		/* У C90 "проблемы" с максимальной длиной строки кода */

		printf("\nДемонстрация машины опорных векторов.\n\n");
		printf("Запуск программы:\n\n");
		printf("ex_svm DIR IMAGE_1\n\n");
		printf("Где:\n\n");
		printf("\tDIR - путь и имя каталога, в котором должны содержаться следующие файлы:\n\n");
		printf("\t\tteach.png - файл с изображением, на основании которого будет составлена обучающая выборка (R = 0, G = 237, B = 95 - класс 1; остальное - класс -1);\n");
		printf("\t\ttest.png - файл с изображением, классификация пикселей которого будет выполнена;\n\n");
		printf("\tIMAGE_1 - путь и имя файла, в который будет сохранено результирующее изображение (белый - правильно классифицированные пиксели, отнесенные к классу 1; ");
		printf("черный - правильно классифицированные пиксели, отнесенные к классу -1, зеленый и красный - пиксели, ошибочно отнесенные к классам 1 и -1 соответственно, ");
		printf("синий - неклассифицированные пиксели).\n\n");

		return -1;
	}

	/* ############################################################################ */

	memset(vec_class, 0, sizeof(unsigned) * 5);

	sprintf(fname, "%s/teach.png", argv[1]);
	src_teach = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);
	size_teach = cvGetSize(src_teach);

	sprintf(fname, "%s/test.png", argv[1]);
	src_test = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);
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

	pixel_blue.val[0] = 255;
	pixel_blue.val[1] = 0;
	pixel_blue.val[2] = 0;
	pixel_blue.val[3] = 0;

	vec_per_class = size_teach.height * size_teach.width / 2000;
	vec_num = vec_per_class * 5;

	x = (double **) fmll_alloc(sizeof(double), 2, vec_num, 3);
	d = fmll_alloc(sizeof(char), 1, vec_num);

	test_x = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 3);
	test_d = fmll_alloc(sizeof(char), 1, size_test.height * size_test.width);

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
				x[q][0] = pixel.val[0];
				x[q][1] = pixel.val[1];
				x[q][2] = pixel.val[2];

				vec_class[cl_ind]++;

				if(cl_ind)
					d[q] = -1;
				else
					d[q] = 1;

				q++;
			}
		}

	for(v = 0, q = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			pixel = cvGet2D(src_test, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				test_d[q] = 1;
			else
				test_d[q] = -1;

			test_x[q][0] = pixel.val[0];
			test_x[q][1] = pixel.val[1];
			test_x[q][2] = pixel.val[2];
		}

	cvReleaseImage(& src_teach);
	cvReleaseImage(& src_test);

	/* ############################################################################ */
	
	K = & fmll_kernel_radial;
	fmll_kernel_radial_sigma = 2;

	svm = fmll_svm_init(3, K);

	/* fmll_svm_teach_smo(svm, x, d, vec_num, 1, 1E-12, & fmll_svm_teach_smo_selector_keerthi, 10000, 1E-3); */
	fmll_svm_teach_smo(svm, x, d, vec_num, 1, 1E-12, & fmll_svm_teach_smo_selector_fan_chen_lin, 10000, 1E-3);

	/* ############################################################################ */
	
	yes = fmll_svm_test(svm, test_x, test_d, size_test.width * size_test.height, NULL, NULL);

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */

	fmll_svm_save(svm, "svm");
	fmll_svm_destroy(svm);
	svm = fmll_svm_load("svm", K);

	for(v = 0, q = 0, yes = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			res = fmll_svm_run(svm, test_x[q]);

			if(res > 0)
			{
				if(test_d[q] == 1)
				{
					cvSet2D(dst, v, u, pixel_white);
					yes++;
				}
				else
					cvSet2D(dst, v, u, pixel_green);
			}
			else if (res < 0)
			{
				if(test_d[q] == 1)
					cvSet2D(dst, v, u, pixel_red);
				else
				{
					cvSet2D(dst, v, u, pixel_black);
					yes++;
				}
			}
			else
			{
				printf("%f\n", res);
				cvSet2D(dst, v, u, pixel_blue);
			}
		}

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */
	
	fmll_free(x);
	fmll_free(d);
	fmll_free(test_x);
	fmll_free(test_d);
	fmll_svm_destroy(svm);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

double kernel_for_xor(const double * v1, const double * v2, unsigned dim)
{
	return (
			1
			+
			v1[0] * v1[0] * v2[0] * v2[0]
			+
			2 * v1[0] * v1[1] * v2[0] * v2[1]
			+
			v1[1] * v1[1] * v2[1] * v2[1]
			+
			2 * v1[0] * v2[0]
			+
			2 * v1[1] * v2[1]
		   );
}

int xor()
{
	char d[4];
	unsigned u;
	double ** vec;
	double (* K)(const double *, const double *, unsigned);
	fmll_svm * svm;

	vec = (double **) fmll_alloc(sizeof(double), 2, 4, 2);

	vec[0][0] = -1;
	vec[0][1] = -1;
	vec[1][0] = 1;
	vec[1][1] = -1;
	vec[2][0] = -1;
	vec[2][1] = 1;
	vec[3][0] = 1;
	vec[3][1] = 1;

	d[0] = d[3] = -1;
	d[1] = d[2] = 1;

	/* ############################################################################ */
	
	K = & kernel_for_xor;
	svm = fmll_svm_init(3, K);

	fmll_svm_teach_smo(svm, vec, d, 4, 1, 1E-12, & fmll_svm_teach_smo_selector_keerthi, 100, 1E-3);
	/* fmll_svm_teach_smo(svm, vec, d, 4, 1, 1E-12, & fmll_svm_teach_smo_selector_fan_chen_lin, 100, 1E-3); */

	/* ############################################################################ */

	fmll_svm_save(svm, "svm");

	printf("\nXOR:\n\n");

	for(u = 0; u < 4; u++)
		printf("\t[%.0f, %.0f] = %d = %f\n", vec[u][0], vec[u][1], d[u], fmll_svm_run(svm, vec[u]));

	printf("\n");

	fmll_svm_destroy(svm);

	/* ############################################################################ */

	if(
		(svm = fmll_svm_load("svm", K)) != NULL
	  )
	{
		printf("\nXOR after load:\n\n");
		
		fmll_svm_save(svm, "svm_2");

		for(u = 0; u < 4; u++)
			printf("\t[%.0f, %.0f] = %d = %f\n", vec[u][0], vec[u][1], d[u], fmll_svm_run(svm, vec[u]));

		printf("\n");
	}

	/* ############################################################################ */

	fmll_free(vec);
	fmll_svm_destroy(svm);

	return 0;
}

