
#include <cstdio>
#include <opencv2/opencv.hpp>

#include <fmll.h>

using namespace std;
using namespace cv;

double fun(const double value)
{
	return (fabs(value + 1) - fabs(value - 1)) / 2;
}

double A(const double ** Y, const unsigned center_row, const unsigned center_col, const unsigned rows, const unsigned cols, const unsigned iter, const unsigned max_iter)
{
	return 2 * Y[center_row][center_col];
}

double B(const double ** M, const unsigned center_row, const unsigned center_col, const unsigned rows, const unsigned cols, const unsigned iter, const unsigned max_iter)
{
	int from_v, to_v, from_u, to_u;
	unsigned v, u, num;
	double sum;

	from_v = center_row - 5;
	to_v = center_row + 5;

	from_u = center_col - 5;
	to_u = center_col + 5;

	if(from_v < 0)
		from_v = 0;

	if(to_v >= rows)
		to_v = rows - 1;

	if(from_u < 0)
		from_u = 0;

	if(to_u >= cols)
		to_u = cols - 1;

	for(v = from_v, num = 0, sum = 0; v <= to_v; v++)
		for(u = from_u; u <= to_u; u++)
			if(! (v == center_row && u == center_col))
			{
				num ++;
				sum -= M[v][u];
			}

	sum += num * M[center_row][center_col];

	return sum;
}

double I(const double ** M, const double ** U, const unsigned center_row, const unsigned center_col, const unsigned rows, const unsigned cols, const unsigned iter, const unsigned max_iter)
{
	return -2;
}

int main(const int argc, const char * argv[])
{
	if(argc != 2)
	{
		printf("\nДемонстрация применения нелинейной ячеистой нейронной сети\n\nЗапуск программы:\n\nex_cnn IMAGE\n\n");
		printf("Где:\n\n");
		printf("\tIMAGE - путь и имя файла с исходным изображением.\n\n");

		return -1;
	}

	double a, b, _min, _max, ** M;
	unsigned v, u;
	fmll_cnn * cnn;
	Mat src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	const unsigned height = src.rows, width = src.cols;
	Mat dst(height, width, CV_64FC1);

	fmll_throw_null(M = (double **) fmll_alloc(sizeof(double), 2, height, width));

	for(v = 0; v < height; v++)
		for(u = 0; u < width; u++)
			M[v][u] = src.at<uint8_t>(v, u) / 255.;
			// M[v][u] = src.at<uint8_t>(v, u) / 127.5 - 2;

	fmll_throw_null(cnn = fmll_cnn_init(height, width, & fun, & A, & B, & I));

	const double ** r_M = fmll_cnn_run(cnn, (const double **) M, 12);

	for(v = 0; v < height; v++)
		for(u = 0; u < width; u++)
			dst.at<double>(v, u) = r_M[v][u];

	// ############################################################################ 
	
	minMaxLoc(dst, & _min, & _max);
	a = 255. / (_max - _min);
	b = - a * _min;
	dst = Mat_<uint8_t>(a * dst + b);

	printf("min = %lf, max = %lf\n", _min, _max);

	threshold(dst, dst, 150, 255, THRESH_BINARY);

	namedWindow("src", CV_WINDOW_NORMAL);
	namedWindow("dst", CV_WINDOW_NORMAL);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(-1);

	fmll_cnn_destroy(cnn);
	fmll_free(M);

	return 0;
}

