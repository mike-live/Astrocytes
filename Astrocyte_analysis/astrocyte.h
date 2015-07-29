#pragma once

#include "declaration.h"
#include "matlab/mat.h"
#include "clusters_form.h"
#include "form_logger.h"

#include "boost/pending/disjoint_sets.hpp"
#include <set>

const int sizeof_opencv[7] = { 1, 1, 2, 2, 4, 4, 8 };

class video_data
{
private:	
	uchar * data;
public:
	int n, m, nt, img_type, type_size, nm, size;
	video_data () : data (nullptr), n (0), m (0), nt (0), 
		img_type (0), type_size (0), nm (0), size (0)
	{};
	video_data (uchar * data_, int n_, int m_, int nt_, int img_type_) :
		data (data_), n (n_), m (m_), nt (nt_), nm (n * m), size (nm * nt), 
		img_type (img_type_), type_size (sizeof_opencv[img_type])
	{};
	uchar * frame (int t) { return data + nm * t * type_size; };
	Mat image (int t) { return Mat (n, m, img_type, frame (t)); };
	uchar * get_data () { return data; };
	uchar cell (int t, int i, int j) { return data[t * nm + i * m + j]; } 
};

struct vertex 
{ 
	int key; 
	ushort start, finish; 
	color color; 
	ushort len () const { return finish - start + 1; }; 
};



struct segmentation_settings
{
	int a, min_points, eps;
	double thr_area, thr_time;
	segmentation_settings () : a (10), min_points (5), eps (5), 
		thr_area (0.5), thr_time (0.5) {};
	// a - side of square, min_points - num of points in one cluster, eps - neighbourhood points in cluster,
	// thr_area ([0, 1]) - threshold of overlapping area , thr_time ([0, 1]) - threshold of overlapping time intervals
	segmentation_settings (int a_, int min_points_, int eps_, double thr_area_, 
		double thr_time_) : a (a_), min_points (min_points_), eps (eps_), 
						    thr_area (thr_area_), thr_time (thr_time_) {};
};

class astrocyte
{
private:
	fs::path file_name;
	video_data data, motion, intensity;
	mxArray * pa;
	Mat thr, thr_gauss;
	Mat max_thr (bool gauss = true);
	
	// Morphology
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// Segmentation
	segmentation_settings segm_;

	vector <ushort> * mt;
	vector <vertex> * graph;
	std::map <int, vertex> components; // key - num of component
	std::set <int> cur_components;
	
	Mat cur_img;
	int cur_t, new_t;

	clusters_form my_form;
	Logger *logger;
public:	
	astrocyte(fs::path file_name_, form & main_form, Logger *logg, std::string var_name = "");
	~astrocyte() { delete logger; }
	
	void preprocessing ();
	void clear_data () { mxDestroyArray (pa); data = video_data (); };
	void background_subtraction ();
	
	// Morphology
	vector<vector<Point>> & find_morphology ();
	void draw_morphology (fs::path file_name, bool video = true);

	// Segmentation
	void calc_clusters ();
	void dialog_clusters (segmentation_settings segm);
	void dialog_dbscan ();
};
