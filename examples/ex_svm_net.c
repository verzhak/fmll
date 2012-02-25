
#include <stdio.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll.h>

int image_analysis(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
	return image_analysis(argc, argv);
}

int image_analysis(const int argc, const char * argv[])
{
#define NUM 5

	char fname[4096];
	unsigned u, v, q, vec_per_class, vec_num, cl_ind, yes, res, vec_class[5], max_iter[NUM], * d, * test_d;
	double C[NUM], tau[NUM], epsilon[NUM], ** x, ** test_x;
	double (* K[NUM])(const double *, const double *, unsigned);
	int (* selector[NUM])(fmll_svm *, double **, char *, unsigned, int *, int *, double, double, double, double *, double *, double **);
	IplImage * src_teach, * src_test, * dst;
	CvSize size_teach, size_test;
	CvScalar pixel, pixel_white, pixel_red, pixel_green, pixel_blue, pixel_violet, pixel_black, pixel_yellow;
	fmll_svm_net * svm_net;

	/* ############################################################################ */

	if(argc != 3)
	{
		/* У C90 "проблемы" с максимальной длиной строки кода */

		printf("\nДемонстрация нейронной сети, состоящей из нескольких машин опорных векторов.\n\n");
		printf("Запуск программы:\n\n");
		printf("ex_svm_net DIR IMAGE_1\n\n");
		printf("Где:\n\n");
		printf("\tDIR - путь и имя каталога, в котором должны содержаться следующие файлы:\n\n");
		printf("\t\tteach.png - файл с изображением, на основании которого будет составлена обучающая выборка;\n");
		printf("\t\ttest.png - файл с изображением, классификация пикселей которого будет выполнена;\n\n");
		printf("\tIMAGE_1 - путь и имя файла, в который будет сохранено результирующее изображение ");
		printf("(белый, красный, зеленый, синий, фиолетовый - правильно классифицированные пиксели; черный - неправильно классифицированные пиксели; ");
		printf("желтый - неклассифицированные пиксели).\n\n");

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
	
	pixel_violet.val[0] = 255;
	pixel_violet.val[1] = 0;
	pixel_violet.val[2] = 255;
	pixel_violet.val[3] = 0;

	pixel_yellow.val[0] = 0;
	pixel_yellow.val[1] = 255;
	pixel_yellow.val[2] = 255;
	pixel_yellow.val[3] = 0;

	vec_per_class = size_teach.height * size_teach.width / 2000;
	vec_num = vec_per_class * 5;

	x = (double **) fmll_alloc(sizeof(double), 2, vec_num, 3);
	d = fmll_alloc(sizeof(unsigned), 1, vec_num);

	test_x = (double **) fmll_alloc(sizeof(double), 2, size_test.height * size_test.width, 3);
	test_d = fmll_alloc(sizeof(unsigned), 1, size_test.height * size_test.width);

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
				d[q] = cl_ind;

				vec_class[cl_ind]++;
				q++;
			}
		}

	for(v = 0, q = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			pixel = cvGet2D(src_test, v, u);

			if(pixel.val[0] == 0 && pixel.val[1] == 237 && pixel.val[2] == 95)
				test_d[q] = 0;
			else if(pixel.val[0] == 10 && pixel.val[1] == 169 && pixel.val[2] == 203)
				test_d[q] = 1;
			else if(pixel.val[0] == 0 && pixel.val[1] == 255 && pixel.val[2] == 255)
				test_d[q] = 2;
			else if(pixel.val[0] == 255 && pixel.val[1] == 0 && pixel.val[2] == 12)
				test_d[q] = 3;
			else
				test_d[q] = 4;

			test_x[q][0] = pixel.val[0];
			test_x[q][1] = pixel.val[1];
			test_x[q][2] = pixel.val[2];
		}

	cvReleaseImage(& src_teach);
	cvReleaseImage(& src_test);

	/* ############################################################################ */
	
	for(u = 0; u < NUM; u++)
		K[u] = & fmll_kernel_radial;

	svm_net = fmll_svm_net_init(NUM, 3, K);

	/* ############################################################################ */

	for(u = 0; u < NUM; u++)
	{
		C[u] = 1;
		tau[u] = 1E-12;
		selector[u] = & fmll_svm_teach_smo_selector_fan_chen_lin;
		max_iter[u] = 10000;
		epsilon[u] = 1E-3;
	}

	/* omp_set_num_threads(1); */
	fmll_svm_net_teach_smo(svm_net, x, d, vec_num, C, tau, selector, max_iter, epsilon);

	/* ############################################################################ */

	yes = fmll_svm_net_test(svm_net, test_x, test_d, size_test.width * size_test.height, NULL, NULL);
	
	printf("\nВерно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */

	fmll_svm_net_save(svm_net, "svm_net");
	fmll_svm_net_destroy(svm_net);
	svm_net = fmll_svm_net_load("svm_net", K);

	for(v = 0, q = 0, yes = 0; v < size_test.height; v++)
		for(u = 0; u < size_test.width; u++, q++)
		{
			res = fmll_svm_net_run(svm_net, test_x[q], NULL);

			if(res >= 0)
			{
				if(res == test_d[q])
				{
					yes++;

					switch(res)
					{
						case 0:
						{
							cvSet2D(dst, v, u, pixel_white);
							break;
						}
						case 1:
						{
							cvSet2D(dst, v, u, pixel_red);
							break;
						}
						case 2:
						{
							cvSet2D(dst, v, u, pixel_green);
							break;
						}
						case 3:
						{
							cvSet2D(dst, v, u, pixel_blue);
							break;
						}
						case 4:
						{
							cvSet2D(dst, v, u, pixel_violet);
							break;
						}
					}
				}
				else
					cvSet2D(dst, v, u, pixel_black);
			}
			else if(res == -1)
				cvSet2D(dst, v, u, pixel_yellow);
			else
				cvSet2D(dst, v, u, pixel_black);
		}

	printf("Верно классифицированных пикселей: %u из %u (%.7f %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	/* ############################################################################ */
	
	fmll_free(x);
	fmll_free(d);
	fmll_free(test_x);
	fmll_free(test_d);
	fmll_svm_net_destroy(svm_net);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

