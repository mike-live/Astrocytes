/*
img_proc - модуль, содержащий обертки для функций обработки изображений
*/
#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "config.h"

using namespace cv;

void do_sobel(Mat& source, Mat& dest);
void do_laplac(Mat& source, Mat& dest);
void do_canny(Mat& source, Mat& dest);

extern double lowThreshold, uppThreshold;