
#include <stdio.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll/fmll.h>

int image_analysis(const int argc, const char * argv[]);

int main(const int argc, const char * argv[])
{
	return image_analysis(argc, argv);
}

int image_analysis(const int argc, const char * argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "\nДемонстрация нейронной сети, состоящей из нескольких машин опорных векторов.\n\nЗапуск программы:\n\nex_svm DIR IMAGE_1\n\nГде:\n\n\tDIR - путь и имя каталога, в котором должны содержаться следующие файлы:\n\n\t\tteach.png - файл с изображением, на основании которого будет составлена обучающая выборка;\n\t\ttest.png - файл с изображением, классификация пикселей которого будет выполнена;\n\n\tIMAGE_1 - путь и имя файла, в который будет сохранено результирующее изображение (белый, красный, зеленый, синий, фиолетовый  - правильно классифицированные пиксели; черный - неправильно классифицированные пиксели; желтый - неклассифицированные пиксели).\n\n");

		return -1;
	}

	char fname[4096];
	unsigned u, v, q, t, vec_per_class, vec_num, cl_ind, yes, res, vec_class[5] = {0, 0, 0, 0, 0};

	// ############################################################################ 

	sprintf(fname, "%s/teach.png", argv[1]);
	IplImage * src_teach = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	sprintf(fname, "%s/test.png", argv[1]);
	IplImage * src_test = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);

	CvSize size_teach = cvGetSize(src_teach), size_test = cvGetSize(src_test);
	IplImage * dst = cvCreateImage(size_test, IPL_DEPTH_8U, 3);
	CvScalar pixel, pixel_white = {.val = {255, 255, 255, 0}}, pixel_red = {.val = {0, 0, 255, 0}};
	CvScalar pixel_green = {.val = {0, 255, 0, 0}}, pixel_blue = {.val = {255, 0, 0, 0}};
	CvScalar pixel_violet = {.val = {255, 0, 255, 0}}, pixel_black = {.val = {0, 0, 0, 0}}, pixel_yellow = {.val = {0, 255, 255, 0}};

	vec_per_class = size_teach.height * size_teach.width / 2000;
	vec_num = vec_per_class * 5;

	double ** x = (double **) fmll_alloc_2D(vec_num, 3, sizeof(double));
	unsigned * d = fmll_alloc_1D(vec_num, sizeof(unsigned));
	double ** test_x = (double **) fmll_alloc_2D(size_test.height * size_test.width, 3, sizeof(double));
	unsigned * test_d = fmll_alloc_1D(size_test.height * size_test.width, sizeof(unsigned));

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

			test_x[q][0] = pixel.val[0] / 255;
			test_x[q][1] = pixel.val[1] / 255;
			test_x[q][2] = pixel.val[2] / 255;
		}

	cvReleaseImage(& src_teach);
	cvReleaseImage(& src_test);

	// ############################################################################ 
	
	unsigned num = 5;
	double (* K[num])(const double *, const double *, unsigned);

	for(u = 0; u < num; u++)
		K[u] = & fmll_kernel_radial;

	fmll_svm_net * svm_net = fmll_svm_net_init(num, 3, K);

	// ############################################################################ 
	// TODO Обучение

	for(u = 0; u < svm_net->num; u++)
	{
		svm_net->svm[u]->num = rand() % 10 + 1;
		svm_net->svm[u]->w = fmll_alloc_1D(svm_net->svm[u]->num, sizeof(double));
		svm_net->svm[u]->s = (double **) fmll_alloc_2D(svm_net->svm[u]->num, svm_net->svm[u]->dim, sizeof(double));

		for(v = 0; v < svm_net->svm[u]->num; v++)
		{
			svm_net->svm[u]->w[v] = drand48();

			for(t = 0; t < svm_net->svm[u]->dim; t++)
				svm_net->svm[u]->s[v][t] = drand48();
		}

		svm_net->svm[u]->b = drand48();
	}

	// ############################################################################ 
	// TODO Тестирование
	
	/*
	double ** deviation = (double **) fmll_alloc_2D(1, 2, sizeof(double));
	deviation[0][0] = 0.3;
	deviation[0][1] = 0.3;

	unsigned yes = fmll_perceptron_test(perc, test_x, test_d, deviation, size_test.width * size_test.height, NULL, NULL);

	printf("Верно классифицированных пикселей: %u из %u (%.7lf %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	*/

	// ############################################################################ 

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

	printf("Верно классифицированных пикселей: %u из %u (%.7lf %%)\n",
			yes, (size_test.width * size_test.height), (100.0 * yes) / (size_test.width * size_test.height));

	// ############################################################################ 
	
	fmll_free_ND(x);
	fmll_free_ND(d);
	fmll_free_ND(test_x);
	fmll_free_ND(test_d);
	fmll_svm_net_destroy(svm_net);

	cvSaveImage(argv[2], dst, NULL);
	cvReleaseImage(& dst);

	return 0;
}

