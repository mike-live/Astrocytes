#include "astrocyte.h"

vector<vector<Point>> & astrocyte::find_morphology ()
{
	max_thr ();
	if (contours.empty()) findContours (thr_gauss, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	return contours;
}

void astrocyte::draw_morphology (fs::path file_name, bool video)
{
	vector<vector<Point>> countours_draw;
	find_morphology ();

	// Calculate area of countours
	double mxs = 0;
	for (int i = 0; i < (int)contours.size (); i++) {
		double ss = contourArea (contours[i]);
		mxs = max (ss, mxs);
	}

	// Select countours with area more than 2.5% of max area
	const double percent_area = 2.5;
	for (int i = 0; i < (int)contours.size (); i++) 
		if (abs (contourArea (contours[i])) > percent_area / 100.0 * mxs)
			countours_draw.push_back (contours[i]);

	// Create a video file
	VideoWriter video_writer (file_name.string(), -1, 2, Size (intensity.n, intensity.m));
	
	// Draw countours	
	for (int t = 0; t < intensity.nt; t++) {
		Mat img = intensity.image(t), img_morph;
		applyColorMap (img, img_morph, COLORMAP_JET);
		for (int j = 0; j < (int)countours_draw.size (); j++) drawContours (img_morph, countours_draw, j, Scalar (255, 255, 255), 2, 8);
		//imwrite (to_string (i) + ".bmp", img_morph);
		if (video) video_writer.write (img_morph);
	}
}