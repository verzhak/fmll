
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <fmll.h>

int main(const int argc, const char * argv[])
{
	const unsigned base = 256 * 256 * 256;
	unsigned u, v, q, cur, index_winner, N[4];
	double param[2], ** vec;
	IplImage * src, * dst_1, * dst_2, * dst_3, * dst_4;
	CvSize size;
	CvScalar pixel;
	fmll_random * rnd;
	fmll_som * som;

	/* ############################################################################ */

	if(argc != 6)
	{
		/* У C90 "проблемы" с максимальной длиной строки кода */

		printf("\nДемонстрация самоорганизующейся карты.\n\n");
		printf("Запуск программы:\n\n");
		printf("ex_som IMAGE_SRC IMAGE_1 IMAGE_2 IMAGE_3 IMAGE_4\n\n");
		printf("Где:\n\n");
		printf("\tIMAGE_SRC - путь и имя файла с исходным изображением;\n");
		printf("\tIMAGE_1 - путь и имя файла, в который будет сохранено первое результирующее изображение;\n");
		printf("\tIMAGE_2 - путь и имя файла, в который будет сохранено второе результирующее изображение;\n");
		printf("\tIMAGE_3 - путь и имя файла, в который будет сохранено третье результирующее изображение;\n");
		printf("\tIMAGE_4 - путь и имя файла, в который будет сохранено четвертое результирующее изображение.\n\n");

		return -1;
	}

	/* ############################################################################ */

	src = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	size = cvGetSize(src);

	dst_1 = cvCreateImage(size, IPL_DEPTH_8U, 3);
	dst_2 = cvCreateImage(size, IPL_DEPTH_8U, 3);
	dst_3 = cvCreateImage(size, IPL_DEPTH_8U, 3);
	dst_4 = cvCreateImage(size, IPL_DEPTH_8U, 3);

	N[0] = 5;
	N[1] = 5;
	N[2] = 5;
	N[3] = 5;

	vec = (double **) fmll_alloc(sizeof(double), 2, size.height * size.width, 3);

	for(v = 0, q = 0; v < size.height; v++)
		for(u = 0; u < size.width; u++, q++)
		{
			pixel = cvGet2D(src, v, u);

			vec[q][0] = pixel.val[0];
			vec[q][1] = pixel.val[1];
			vec[q][2] = pixel.val[2];
		}

	param[0] = 0;
	param[1] = 1;

	rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param, time(NULL));
	som = fmll_som_init(N, 4, 3, rnd, & fmll_distance_euclid, & fmll_distance_euclid);

	fmll_som_so_kohonen(som, vec, size.height * size.width, 0, & fmll_timing_next_beta_step_plus_0_1, 0.8, 0.002, & fmll_som_neighbor_radial);

	for(v = 0; v < som->num; v++)
		printf("%u = [%f, %f, %f]\n", v, som->w[v][0], som->w[v][1], som->w[v][2]);

	for(v = 0, q = 0; v < size.height; v++)
		for(u = 0; u < size.width; u++, q++)
		{
			index_winner = fmll_som_run(som, vec[q]);

			pixel.val[0] = som->w[index_winner][0];
			pixel.val[1] = som->w[index_winner][1];
			pixel.val[2] = som->w[index_winner][2];

			cvSet2D(dst_1, v, u, pixel);

			cur = (base / (double) som->num) * index_winner;

			pixel.val[0] = (cur & 0xFF0000) >> 16;
			pixel.val[1] = (cur & 0xFF00) >> 8;
			pixel.val[2] = cur & 0xFF;

			cvSet2D(dst_2, v, u, pixel);
		}

	fmll_som_save(som, "som");

	fmll_som_destroy(som);
	som = NULL;

	if(
		(som = fmll_som_load("som", & fmll_distance_euclid, & fmll_distance_euclid)) != NULL
	  )
	{
		fmll_som_save(som, "som_2");

		for(v = 0, q = 0; v < size.height; v++)
			for(u = 0; u < size.width; u++, q++)
			{
				index_winner = fmll_som_run(som, vec[q]);

				pixel.val[0] = som->w[index_winner][0];
				pixel.val[1] = som->w[index_winner][1];
				pixel.val[2] = som->w[index_winner][2];

				cvSet2D(dst_3, v, u, pixel);

				cur = (base / (double) som->num) * index_winner;

				pixel.val[0] = (cur & 0xFF0000) >> 16;
				pixel.val[1] = (cur & 0xFF00) >> 8;
				pixel.val[2] = cur & 0xFF;

				cvSet2D(dst_4, v, u, pixel);
			}
	}

	fmll_free(vec);
	fmll_som_destroy(som);
	fmll_random_destroy(rnd);

	cvSaveImage(argv[2], dst_1, NULL);
	cvSaveImage(argv[3], dst_2, NULL);
	cvSaveImage(argv[4], dst_3, NULL);
	cvSaveImage(argv[5], dst_4, NULL);

	cvReleaseImage(& src);
	cvReleaseImage(& dst_1);
	cvReleaseImage(& dst_2);
	cvReleaseImage(& dst_3);
	cvReleaseImage(& dst_4);

	return 0;
}

