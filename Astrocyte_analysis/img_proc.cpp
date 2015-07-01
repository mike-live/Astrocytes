#include "img_proc.h"

double lowThreshold = 70, uppThreshold = 260;

void do_sobel(Mat & source, Mat & dest)
{
	Mat xGrad, yGrad;
	static double alpha = 0.5, beta = 0.5;
	//Sobel(source, xGrad, imgddepth, 1, 0); // по Ox 
	//Sobel(source, yGrad, imgddepth, 0, 1); // по Oy
	dest = xGrad * alpha + yGrad * beta;
	xGrad.release();
	yGrad.release();
}

void do_laplac(Mat& source, Mat& dest)
{
	//Laplacian(source, dest, imgddepth);
}

void do_canny(Mat& source, Mat& dest)
{	
	double minVal, maxVal;
	minMaxLoc (source, &minVal, &maxVal);
	Canny (source, dest, lowThreshold * (maxVal - minVal) + minVal, maxVal - uppThreshold * (maxVal - minVal));
}