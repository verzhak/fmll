
#include <stdio.h>
#include <math.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

#include "memory.h"
#include "distance.h"
#include "various.h"
#include "som.h"

int main(const int argc, const char * argv[])
{
	if(argc != 4)
	{
		printf("\nДемонстрация самоорганизующейся карты.\n\nЗапуск программы:\n\nex_som IMAGE_1 IMAGE_2 IMAGE_3\n\nГде:\n\n\tIMAGE_1 - путь и имя файла с первым изображением;\n\tIMAGE_2 - путь и имя файла, в который будет сохранено первое результирующее изображение;\n\tIMAGE_3 - путь и имя файла, в который будет сохранено второе результирующее изображение.\n\n");

		return 0;
	}

	IplImage * src = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	CvSize size = cvGetSize(src);
	IplImage * dst_1 = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage * dst_2 = cvCreateImage(size, IPL_DEPTH_8U, 3);
	CvScalar pixel;
	unsigned u, v, q;
	uint8_t N[2] = {10, 10};
	uint32_t index_winner;
	double ** vec = (double **) fmll_alloc_2D(size.height * size.width, 3, sizeof(double));

	for(v = 0, q = 0; v < size.height; v++)
		for(u = 0; u < size.width; u++, q++)
		{
			pixel = cvGet2D(src, v, u);

			vec[q][0] = pixel.val[0];
			vec[q][1] = pixel.val[1];
			vec[q][2] = pixel.val[2];
		}

	fmll_som * som = fmll_som_init(N, 2, 3, & fmll_som_weight_init_null, & fmll_distance_euclid, & fmll_distance_euclid);

	fmll_som_so_kohonen(som, vec, size.height * size.width, 0, & fmll_som_next_beta_step_0_1, 0.8, 0.002, & fmll_som_neighbor_radial);

	for(v = 0; v < som->num; v++)
		printf("%u = [%lf, %lf, %lf]\n", v, som->w[v][0], som->w[v][1], som->w[v][2]);

	uint32_t cur, base = 256 * 256 * 256;

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

	fmll_som_destroy(som);

	fmll_free_ND(vec);

	cvSaveImage(argv[2], dst_1, NULL);
	cvSaveImage(argv[3], dst_2, NULL);

	cvReleaseImage(& src);
	cvReleaseImage(& dst_1);
	cvReleaseImage(& dst_2);

	return 0;
}

