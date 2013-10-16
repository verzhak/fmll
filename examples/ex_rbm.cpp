
#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>

#include <fmll.h>

using namespace std;
using namespace cv;

typedef vector< pair<char, Mat> > TSymb;

unsigned height, width;

TSymb load(const string dname);
double ** create_vec(const TSymb & symb);
Mat from_vec_to_Mat(const double * vec);
char find(const TSymb & symb, const Mat & img, const double threshold);

int main(const int argc, const char * argv[])
{
	if(argc != 3)
	{
		printf("\nДемонстрация ограниченной машины Больцмана\n\nЗапуск программы:\n\nex_rbm DIR ERRORS_NUM\n\n");
		printf("Где:\n\n");
		printf("\tDIR - путь и имя каталога, в котором должны содержаться изображения букв латинского алфавита;\n");
		printf("\tERRORS_NUM - зашумленность образа при тестировании восстановления зашумленных образов.\n\n");

		return -1;
	}

	TSymb symb = load(argv[1]);
	const unsigned max_iter = 100, errors_num = atoi(argv[2]);
	unsigned v, ind, pos;
	double param[2] = { -0.1, 0.1 }, ** vec = create_vec(symb);
	fmll_rbm * rbm;
	fmll_random * rnd;

	fmll_throw_null(rnd = fmll_random_init_default_seed(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param));
	fmll_throw_null(rbm = fmll_rbm_init(height * width, 100, 0.2));
	fmll_throw_if(fmll_rbm_weight_init_random(rbm, rnd));

	fmll_throw_if(fmll_rbm_CD_k(rbm, (const double **) vec, symb.size(), 10, 10, max_iter, 1, & fmll_timing_next_beta_step_minus_0_001));

	printf("A = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[0], max_iter)), .01));
	printf("B = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[1], max_iter)), .01));
	printf("Q = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[2], max_iter)), .01));
	printf("W = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[3], max_iter)), .01));
	printf("X = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[4], max_iter)), .01));

	// ############################################################################ 
	// Зашумленные образы

	printf("-----> Тестирование восстановления зашумленных образов <-----\n");

	for(ind = 0; ind < symb.size(); ind++)
		for(v = 0; v < errors_num; v++)
		{
			pos = rand() % 20 * width + rand() % 20;
			vec[ind][pos] *= -1;
		}

	printf("A = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[0], max_iter)), .01));
	printf("B = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[1], max_iter)), .01));
	printf("Q = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[2], max_iter)), .01));
	printf("W = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[3], max_iter)), .01));
	printf("X = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[4], max_iter)), .01));

#if 0

	for(ind = 0; ind < symb.size(); ind++)
	{
		namedWindow("source", CV_WINDOW_NORMAL);
		namedWindow("noise", CV_WINDOW_NORMAL);
		namedWindow("result", CV_WINDOW_NORMAL);

		imshow("source", symb[ind].second);
		imshow("noise", from_vec_to_Mat(vec[ind]));
		imshow("result", from_vec_to_Mat(fmll_rbm_run(rbm, vec[ind], max_iter)));

		waitKey(-1);
	}

#endif

	// ############################################################################ 

	fmll_throw_if(fmll_rbm_save(rbm, "rbm"));
	
	fmll_rbm_destroy(rbm);
	rbm = NULL;

	fmll_throw_null(rbm = fmll_rbm_load("rbm"));

	printf("A = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[0], max_iter)), .01));
	printf("B = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[1], max_iter)), .01));
	printf("Q = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[2], max_iter)), .01));
	printf("W = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[3], max_iter)), .01));
	printf("X = %c\n", find(symb, from_vec_to_Mat(fmll_rbm_run(rbm, vec[4], max_iter)), .01));

	fmll_random_destroy(rnd);
	fmll_rbm_destroy(rbm);
	fmll_free(vec);

	return 0;
}

void load_symb(TSymb & data, const string dname, const char symb)
{
	string fname;
	pair<char, Mat> t_pair;

	fname = dname + "/";
	fname += symb;
	fname += ".png";

	t_pair.first = symb;
	t_pair.second = Mat_<double>(imread(fname, CV_LOAD_IMAGE_GRAYSCALE)) / 127.5 - 1;
	data.push_back(t_pair);
}

TSymb load(const string dname)
{
	TSymb symb;

	load_symb(symb, dname, 'A');
	load_symb(symb, dname, 'B');
	load_symb(symb, dname, 'Q');
	load_symb(symb, dname, 'W');
	load_symb(symb, dname, 'X');

	height = symb[0].second.rows;
	width = symb[0].second.cols;
	
	return symb;
}

double ** create_vec(const TSymb & symb)
{
	const unsigned vec_num = symb.size();
	unsigned v, u, tv, tu;
	double ** vec;

	fmll_throw_null(vec = (double **) fmll_alloc(sizeof(double), 2, vec_num, height * width));

	for(v = 0; v < vec_num; v++)
		for(tv = 0, u = 0; tv < height; tv++)
			for(tu = 0; tu < width; tu++, u++)
				vec[v][u] = symb[v].second.at<double>(tv, tu);

	return vec;
}

Mat from_vec_to_Mat(const double * vec)
{
	unsigned v, u, t;
	Mat ret(height, width, CV_64F);

	for(v = 0, t = 0; v < height; v++)
		for(u = 0; u < width; u++, t++)
			ret.at<double>(v, u) = vec[t];

	return ret;
}

char find(const TSymb & symb, const Mat & img, const double threshold)
{
	const unsigned height = symb[0].second.rows, width = symb[0].second.cols;
	unsigned v, u;

	fmll_throw_if(img.rows != height);
	fmll_throw_if(img.cols != width);

	for(auto & it : symb)
	{
		for(v = 0; v < height; v++)
			for(u = 0; u < width; u++)
				if(fabs(it.second.at<double>(v, u) - img.at<double>(v, u)) > threshold)
					goto next;

		return it.first;

		next:

		;
	}
}

