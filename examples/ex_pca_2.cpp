
#include <cstdio>
#include <ctime>
#include <opencv2/opencv.hpp>

#include <fmll.h>

using namespace std;
using namespace cv;

int main(const int argc, const char * argv[])
{
	int ret = 0;
	unsigned v, u, t;
	double eigen[3], ** vec = NULL;
	fmll_random * rnd = NULL;
	fmll_pca * pca = NULL;
	
	try
	{
		fmll_throw_if(argc != 2);

		fmll_throw_null(rnd = fmll_random_init(FMLL_RANDOM_LCG, time(NULL)));
		fmll_throw_null(pca = fmll_pca_init(3, 3, & fmll_weight_init_random_0_01, rnd));

		Mat image = imread(argv[1], 1);
		fmll_throw_null(image.data);

		Size sz = image.size();

		fmll_throw_null(vec = (double **) fmll_alloc(sizeof(double), 2, sz.height * sz.width, 3));

		for(v = 0; v < sz.height; v++)
			for(u = 0; u < sz.width; u++)
				for(t = 0; t < 3; t++)
					vec[v * sz.width + u][t] = image.at<Vec3b>(v, u)[t];

		fmll_throw_if(fmll_centering(vec, sz.height * sz.width, 3));
		fmll_throw_if(fmll_pca_so(pca, vec, sz.height * sz.width, 0.0000001, & fmll_timing_next_beta_step_0, 0.0000001, eigen));

		for(v = 0; v < 3; v++)
			printf("%lf = %lf, %lf, %lf\n", eigen[v], pca->w[v][0], pca->w[v][1], pca->w[v][2]);
	}
	catch(...)
	{
		ret = -1;

		printf("\nЗапуск: ./ex_pca_2 FNAME\n\n");
	}

	fmll_free(vec);
	fmll_pca_destroy(pca);
	fmll_random_destroy(rnd);

	return ret;
}

