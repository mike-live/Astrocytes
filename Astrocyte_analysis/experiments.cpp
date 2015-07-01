#include "experiments.h"

static void draw_optical_flow (const Mat & flow, Mat & cflowmap, int step, const Scalar & color, const Scalar & color2)
{
	for (int y = 0; y < cflowmap.rows; y += step)
		for (int x = 0; x < cflowmap.cols; x += step) {
			const Point2f& fxy = flow.at<Point2f> (y, x);
			double coef = sqrt (fxy.x * fxy.x + fxy.y * fxy.y);
			if (coef > 1e-1) {
				line (cflowmap, Point (x, y), Point (cvRound (x + fxy.x), cvRound (y + fxy.y)), color2 * (coef / 10.0));
				//circle (cflowmap, Point (x, y), 1, color, -1);
			}
		}
}

void my_threshold (Mat & img, Mat & img_thr, bool otsu = false)
{
	Mat img_thrd, img_thru;
	if (otsu) threshold (img, img_thrd, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	else threshold (img, img_thrd, 100, 255, THRESH_BINARY);
	//threshold (img, img_thru, 130, 255, THRESH_BINARY_INV);

	vector<vector<Point> > contours;
	vector<double> area;
	vector<Vec4i> hierarchy;

	findContours (img_thrd, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	/*int idx = 0;
	for (; idx >= 0; idx = hierarchy[idx][0]) {
	Scalar color (255, 255, 255);
	drawContours (img_max_thr, contours, idx, color, 1, 8, hierarchy);
	}*/

	// Calculate area of countours
	double mxs = 0;
	for (int i = 0; i < (int)contours.size (); i++) {
		double ss = contourArea (contours[i]);
		//for (int j = 1; j < (int)contours[i].size (); j++) ss += contours[i][j].x * contours[i][j - 1].y - contours[i][j].y * contours[i][j - 1].x;
		area.push_back (ss);
		mxs = max (ss, mxs);
	}
	img_thr.create (img.rows, img.cols, CV_8UC1);
	img_thr.setTo (0);

	// Draw countours with area more than 2.5% of max area
	const double percent_area = 2.5;
	for (int i = 0; i < (int)contours.size (); i++) if (abs (area[i]) > percent_area / 100.0 * mxs) drawContours (img_thr, contours, i, Scalar (255), 1, 8);
}


int calc_max_thr (unsigned char * data, int n, int m, int nt, int img_type, int type_size)
{
	Mat img_max_gauss, img_gauss_max, img_max;
	Mat imgc_max_gauss, imgc_gauss_max, imgc_max;

	img_max.create (n, m, CV_8UC1);
	img_max.setTo (0);
	img_gauss_max.create (n, m, CV_8UC1);
	img_gauss_max.setTo (0);

	for (int i = 0; i < nt; i++) {
		Mat img (n, m, img_type, data + i * n * m * type_size);
		img = min (img, 400);
		double minVal, maxVal;
		minMaxLoc (img, &minVal, &maxVal); //find minimum and maximum intensities
		img.convertTo (img, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

		img_max = max (img, img_max);
		GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
		img_gauss_max = max (img, img_gauss_max);
	}

	GaussianBlur (img_max, img_max_gauss, cv::Size (3, 3), 1.0, 1.0);

	applyColorMap (img_max, imgc_max, COLORMAP_JET);
	applyColorMap (img_max_gauss, imgc_max_gauss, COLORMAP_JET);
	applyColorMap (img_gauss_max, imgc_gauss_max, COLORMAP_JET);

	//int x = (int)string (matfilename).find ('.');
	//string file_name = string (matfilename).substr (0, x);
	//CreateDirectory (file_name.c_str (), 0);
	//CreateDirectory ((file_name + "\\Max").c_str (), 0);

	//string path = matfiledir + "\\" + file_name + "\\Max";

	/*imwrite (path + "\\" + "1_max__" + file_name + ".bmp", img_max);
	imwrite (path + "\\" + "2_max_gauss__" + file_name + ".bmp", img_max_gauss);
	imwrite (path + "\\" + "3_gauss_max__" + file_name + ".bmp", img_gauss_max);

	imwrite (path + "\\" + "1_max_jet__" + file_name + ".bmp", imgc_max);
	imwrite (path + "\\" + "2_max_gauss_jet__" + file_name + ".bmp", imgc_max_gauss);
	imwrite (path + "\\" + "3_gauss_max_jet__" + file_name + ".bmp", imgc_gauss_max);
	*/
	Mat img_max_thr, img_max_gauss_thr, img_gauss_max_thr;

	my_threshold (img_max, img_max_thr);
	my_threshold (img_max_gauss, img_max_gauss_thr);
	my_threshold (img_gauss_max, img_gauss_max_thr);

	/*imwrite (path + "\\" + "1_max_thr_100__" + file_name + ".bmp", img_max_thr);
	imwrite (path + "\\" + "2_max_gauss_thr_100__" + file_name + ".bmp", img_max_gauss_thr);
	imwrite (path + "\\" + "3_gauss_max_thr_100__" + file_name + ".bmp", img_gauss_max_thr);

	my_threshold (img_max, img_max_thr, true);
	my_threshold (img_max_gauss, img_max_gauss_thr, true);
	my_threshold (img_gauss_max, img_gauss_max_thr, true);

	imwrite (path + "\\" + "1_max_thr_otsu__" + file_name + ".bmp", img_max_thr);
	imwrite (path + "\\" + "2_max_gauss_thr_otsu__" + file_name + ".bmp", img_max_gauss_thr);
	imwrite (path + "\\" + "3_gauss_max_thr_otsu__" + file_name + ".bmp", img_gauss_max_thr);*/
	return 0;
}

int show_flow (unsigned char *data, int n, int m, int nt, int img_type, int type_size)
{
	Point2f point;
	bool addRemovePt = false;

	VideoCapture cap;
	TermCriteria termcrit (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize (10, 10), winSize (31, 31);

	const int MAX_COUNT = 300;
	bool needToInit = false;
	bool nightMode = false;

	Mat gray, prevGray, image;
	vector<Point2f> points[2];
	Mat img_max_gauss, img_gauss_max, img_max;
	Mat imgc_max_gauss, imgc_gauss_max, imgc_max, img_gauss_max_thr;

	img_max.create (n, m, CV_8UC1);
	img_max.setTo (0);
	img_gauss_max.create (n, m, CV_8UC1);
	img_gauss_max.setTo (0);

	for (int i = 0; i < nt; i++) {
		Mat img (n, m, img_type, data + i * n * m * type_size);
		img = min (img, 400);
		double minVal, maxVal;
		minMaxLoc (img, &minVal, &maxVal); //find minimum and maximum intensities
		img.convertTo (img, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

		img_max = max (img, img_max);
		GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
		img_gauss_max = max (img, img_gauss_max);
	}

	threshold (img_gauss_max, img_gauss_max_thr, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	Mat img_gauss_max_thr_res;
	//resize (img_gauss_max_thr, img_gauss_max_thr_res, cv::Size (img_gauss_max_thr.cols * 3, img_gauss_max_thr.rows * 3), 0, 0, 0);
	Mat prev_gray, prev_img;
	//int x = (int)string (matfilename).find ('.');
	//string file_name = string (matfilename).substr (0, x);
	//string path = matfiledir + "\\" + file_name + "\\flow";
	//imwrite (path + "\\img_big\\img_gauss_max_thr_res" + +".bmp", img_gauss_max_thr_res);
	for (int i = 0; i < nt; i++) {
		printf ("i : %d\n", i);
		/*Mat frame (n, m, imgtype, data + i * n * m * type_size);
		frame = min (frame, 100);
		double minVal, maxVal;
		minMaxLoc (frame, &minVal, &maxVal);
		frame.convertTo (frame, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

		GaussianBlur (frame, frame, cv::Size (3, 3), 1.0, 1.0);
		//Mat res_img;
		//resize (frame, res_img, cv::Size (frame.cols * 3, frame.rows * 3));
		//res_img.copyTo (frame);
		Mat img, image;
		applyColorMap (frame, img, COLORMAP_JET);
		
		img.copyTo (image, img_gauss_max_thr);*/
		//frame.copyTo (gray, img_gauss_max_thr);
		/*Mat img;
		bitwise_and (image, img_gauss_max_thr, img);
		image = img;*/
		//cvtColor (image, gray, COLOR_BGR2GRAY);
		Mat flow, cflow;
		/*if (prev_gray.data != nullptr) {
		calcOpticalFlowFarneback (prev_gray, gray, flow, 0.5, 5, 10, 1, 5, 1.2, 0);
		//cvtColor (prev_img, cflow, COLOR_GRAY2BGR);
		prev_img.copyTo(cflow);
		draw_optical_flow (flow, cflow, 4, Scalar (0, 100, 0), Scalar (0, 0, 255));
		//imshow ("Flow", cflow);
		//imshow ("gray", gray);
		//imshow ("prev_gray", prev_gray);
		char c = (char)waitKey (200);
		}

		gray.copyTo (prev_gray);
		image.copyTo (prev_img);*/

		//char c = (char)waitKey (200);
		/*
		if (nightMode)
		image = Scalar::all (0);

		if (needToInit) {
		// automatic initialization
		goodFeaturesToTrack (gray, points[0], MAX_COUNT, 0.01, 10, Mat (), 3, 0, 0.04);
		cornerSubPix (gray, points[0], subPixWinSize, Size (-1, -1), termcrit);
		addRemovePt = false;
		} else if (!points[0].empty ()) {
		vector<uchar> status;
		vector<float> err;
		if (prevGray.empty ())
		gray.copyTo (prevGray);
		calcOpticalFlowPyrLK (prevGray, gray, points[0], points[1], status, err);
		//, winSize,3, termcrit, 0, 0.001);
		size_t i, k;
		for (i = k = 0; i < points[1].size (); i++) {
		if (!status[i])
		continue;

		points[1][k++] = points[1][i];
		if (points->size() > i) {
		line (image, points[1][i], points[0][i], Scalar(255, 255, 255), 2);
		}
		circle (image, points[1][i], 2, Scalar (0, 0, 0), -1, 8);
		}
		points[1].resize (k);
		}

		if (addRemovePt && points[1].size () < (size_t)MAX_COUNT) {
		vector<Point2f> tmp;
		tmp.push_back (point);
		cornerSubPix (gray, tmp, winSize, cvSize (-1, -1), termcrit);
		points[1].push_back (tmp[0]);
		addRemovePt = false;
		}

		needToInit = false;
		imshow ("LK Demo", image);
		if (i == 0) char c = (char)waitKey ();*/
		//imwrite (path + "\\img_big\\" + std::to_string (i) + ".bmp", image);
		//imwrite (path + "\\img_\\" + std::to_string (i) + ".bmp", img);
		/*char c = (char)waitKey (0);
		if (c == 27)
		break;
		switch (c) {
		case 'r':
		needToInit = true;
		break;
		case 'c':
		points[0].clear ();
		points[1].clear ();
		break;\
		case 'n':
		nightMode = !nightMode;
		break;
		case 's':
		imwrite (path + "\\" + std::to_string (i) + ".bmp", prev_img);
		}*/
	}
	destroyAllWindows ();
	return 0;
}

void conc (Mat & im1, Mat & im2, Mat & im)
{
	Size sz1 = im1.size ();
	Size sz2 = im2.size ();
	Mat im3 (sz1.height, sz1.width + sz2.width, CV_8UC3);
	Mat right (im3, Rect (sz1.width, 0, sz2.width, sz2.height));
	im2.copyTo (right);

	Mat left (im3, Rect (0, 0, sz1.width, sz1.height));
	im1.copyTo (left);

	im = im3;
}

int showImages (unsigned char *data, int n, int m, int nt)
{
	Mat img_max_gauss, img_gauss_max, img_max_bm3d, img_max;
	Mat imgc_max_gauss, imgc_gauss_max, imgc_max_bm3d, imgc_max;

	Mat img_max_gauss_canny, img_gauss_max_canny, img_max_bm3d_canny, img_max_canny;
	Mat imgc_max_gauss_canny, imgc_gauss_max_canny, imgc_max_bm3d_canny, imgc_max_canny;

	Mat img_max_gauss_watershed, img_gauss_max_watershed, img_max_bm3d_watershed, img_max_watershed;

	img_max.create (n, m, CV_8UC1);
	img_max.setTo (0);
	img_gauss_max.create (n, m, CV_8UC1);
	img_gauss_max.setTo (0);

	for (int i = 0; i < nt; i++) {
		
		/*Mat img (n, m, imgtype, data + i * n * m), grayImg;
		img = min (img, 400);
		double minVal, maxVal;
		minMaxLoc (img, &minVal, &maxVal); //find minimum and maximum intensities
		img.convertTo (img, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

		img_max = max (img, img_max);
		GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
		img_gauss_max = max (img, img_gauss_max);*/
	}

	GaussianBlur (img_max, img_max_gauss, cv::Size (3, 3), 1.0, 1.0);

	applyColorMap (img_max, imgc_max, COLORMAP_JET);
	applyColorMap (img_max_gauss, imgc_max_gauss, COLORMAP_JET);
	applyColorMap (img_gauss_max, imgc_gauss_max, COLORMAP_JET);

	//while (true) {
	//scanf ("%lf %lf", &lowThreshold, &uppThreshold);

	//do_canny (img_max, img_max_canny);
	//do_canny (img_gauss_max, img_gauss_max_canny);
	/*img_max_watershed.create (n, m, CV_32SC1);
	img_max_watershed.setTo (0);*/
	//ex_watershed (imgc_max);
	//watershed (img_gauss_max, img_gauss_max_watershed);

	//GaussianBlur (img_max, img_max_gauss, cv::Size (9, 9), 1.0, 1.0);
	//do_canny (img_max_gauss, img_max_gauss_canny);
	//watershed (img_max_gauss, img_max_gauss_watershed);
	/*

	imshow ("0. bm3d 1. max 2. jet", imgc_max);


	imshow ("0. bm3d 1. max 2. gauss: 3. jet", imgc_max_gauss);


	imshow ("0. bm3d 1. gauss 2. max: ", imgc_gauss_max);
	*/

	// Canny view
	/*
	imshow ("1. max 2. canny", img_max_canny);
	imshow ("1. max 2. gauss 3. canny", img_max_gauss_canny);
	imshow ("1. gauss 2. max 3. canny ", img_gauss_max_canny);
	*/
	/*double minVal, maxVal;
	img_max_watershed += 1;
	minMaxLoc (img_max_watershed, &minVal, &maxVal);
	Mat imgc_watershed;
	img_max_watershed.convertTo (imgc_watershed, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));*/
	//applyColorMap (imgc_watershed, imgc_watershed, COLORMAP_JET);

	// Watershed view
	//imshow ("1. max 2. watershed", imgc_watershed);
	/*imshow ("1. max 2. gauss 3. watershed", img_max_gauss_watershed);
	imshow ("1. gauss 2. max 3. watershed ", img_gauss_max_watershed);*/
	//int key = waitKey ();
	//}
	//int x = (int)string (matfilename).find ('.');
	//string file_name = string (matfilename).substr (0, x);
	//CreateDirectory (file_name.c_str (), 0);
	//CreateDirectory (, 0);
	//fs::create_directories ((file_name + "\\Max"));

	//string path = matfiledir + "\\" + file_name + "\\Max";
	//file_name = "";
	Mat img_thresh;
	//threshold (img_max, img_thresh, threshold_value, threshold_max_value, THRESH_TOZERO | CV_THRESH_OTSU);
	//imshow ("some", img_max);	
	imshow ("max thr", img_thresh);
	//threshold (img_max_gauss, img_thresh, 0, threshold_max_value, THRESH_TOZERO | CV_THRESH_OTSU);
	imshow ("max gauss thr", img_thresh);
	//threshold (img_gauss_max, img_thresh, 0, threshold_max_value, THRESH_TOZERO | CV_THRESH_OTSU);
	imshow ("gauss max thr", img_thresh);
	/*
	imwrite (path + "\\" + "1_max__" + file_name + ".bmp", img_max);
	imwrite (path + "\\" + "2_max_gauss__" + file_name + ".bmp", img_max_gauss);
	imwrite (path + "\\" + "3_gauss_max__" + file_name + ".bmp", img_gauss_max);

	imwrite (path + "\\" + "1_max_jet__" + file_name + ".bmp", imgc_max);
	imwrite (path + "\\" + "2_max_gauss_jet__" + file_name + ".bmp", imgc_max_gauss);
	imwrite (path + "\\" + "3_gauss_max_jet__" + file_name + ".bmp", imgc_gauss_max);

	Mat img_max_thr, img_max_gauss_thr, img_gauss_max_thr;

	my_threshold (img_max, img_max_thr);
	my_threshold (img_max_gauss, img_max_gauss_thr);
	my_threshold (img_gauss_max, img_gauss_max_thr);

	imwrite (path + "\\" + "1_max_thr__" + file_name + ".bmp", img_max_thr);
	imwrite (path + "\\" + "2_max_gauss_thr__" + file_name + ".bmp", img_max_gauss_thr);
	imwrite (path + "\\" + "3_gauss_max_thr__" + file_name + ".bmp", img_gauss_max_thr);
	*/
	/*// Canny save
	imwrite (path + "\\" + "1_max_canny__" + file_name + ".bmp", img_max_canny);
	imwrite (path + "\\" + "2_max_gauss_canny__" + file_name + ".bmp", img_max_gauss_canny);
	imwrite (path + "\\" + "3_gauss_max_canny__" + file_name + ".bmp", img_gauss_max_canny);*/

	int key = waitKey ();

	destroyAllWindows ();
	return 0;
}
