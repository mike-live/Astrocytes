#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <cstdio>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <mat.h>
#include "config.h"
#include "img_proc.h"
#include <ctime>
#include <windows.h>
#undef max
#undef min
using namespace std;
using namespace cv;

int showImages (unsigned char *data, int n, int m, int nt);
int calc_max_thr (unsigned char * data, int n, int m, int nt, int img_type, int type_size);
int show_flow (unsigned char *data, int n, int m, int nt, int img_type, int type_size);

#endif