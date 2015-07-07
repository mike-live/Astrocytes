#ifndef ASTROCYTE_H
#define ASTROCYTE_H

#include "declaration.h"
#include "matlab/mat.h"

#include "boost/pending/disjoint_sets.hpp"
#include <set>

const int sizeof_opencv[7] = { 1, 1, 2, 2, 4, 4, 8 };

//CV_8U   0
//CV_8S   1
//CV_16U  2
//CV_16S  3
//CV_32S  4
//CV_32F  5
//CV_64F  6

class video_data
{
private:	
	uchar * data;
public:
	int n, m, nt, img_type, type_size, nm, size;
	video_data () : data (nullptr), n (0), m (0), nt (0), img_type (0), type_size (0), nm (0), size (0)
	{};
	video_data (uchar * data_, int n_, int m_, int nt_, int img_type_) :
		data (data_), n (n_), m (m_), nt (nt_), nm (n * m), size (nm * nt), img_type (img_type_), type_size (sizeof_opencv[img_type])
	{};
	uchar * frame (int t) { return data + nm * t * type_size; };
	Mat image (int t) { return Mat (n, m, img_type, frame (t)); };
	uchar * get_data () { return data; };
	uchar cell (int t, int i, int j) { return data[t * nm + i * m + j]; } 
};

struct vertex { int key; ushort start, finish; color color; ushort len () const { return finish - start + 1; }; };

class clusters_form : public panel<false>
{
private:
	nana::rectangle rec_;
public:
	label la_status;
	listbox li_components;
	picture pic;
	drawing dw_clusters;
	button bu_next;
	button bu_prev;

	picture pic_points;
	drawing dw_points;
	//label la_info;
	label la_coords;

	radio_group rg_picture;
	checkbox ck_astrocyte;
	checkbox ck_back_sub;

	button bu_select_all;
	button bu_deselect_all;
	button bu_find;

	bool dw_upd;
	clusters_form (form & main_form) : 
		panel (main_form), dw_clusters (pic), la_status (*this), li_components (*this), pic (*this), bu_next (*this), bu_prev(*this), dw_upd (true),		
		pic_points (*this), dw_points (pic_points), /*la_info (*this),*/ la_coords (*this), bu_select_all (*this), bu_deselect_all (*this), bu_find (*this)
	{
		caption (STR ("Clusters"));
		size (nana::size { 1000, 1000 });
		move (0, 40);
		
		la_status.size (nana::size { 400, 20 });
		la_status.move (110, 10);
		la_status.caption (STR ("Clusters waiting..."));

		main_form.size (nana::size { 1000 + 20, 800 + 60});
		main_form.move (100, 100);
	}
	
	void status (std::wstring str) { la_status.caption (str); }
	void prepare (unsigned int n, unsigned int m)
	{
		la_status.caption (STR ("Click next image. Click on image = drawing clusters of square with center in position"));

		bu_next.size (nana::size { 40, 20 });
		bu_next.move (60, 10);
		bu_next.caption ("Next");

		bu_prev.size (nana::size { 40, 20 });
		bu_prev.move (10, 10);
		bu_prev.caption ("Prev");

		pic.size (nana::size { m, n });
		pic.move (10, 40);

		li_components.size (nana::size {330, n});
		li_components.move (m + 20, 40);
		li_components.append_header (STR ("#"), 80);
		li_components.append_header (STR ("Color"), 80);
		li_components.append_header (STR ("Start"), 50);
		li_components.append_header (STR ("Finish"), 50);
		li_components.append_header (STR ("Length"), 50);
		li_components.checkable (true);
		li_components.auto_draw (true);

		pic_points.move (10, n + 50);
		pic_points.size (nana::size { m + 340, 50 });

		//la_info.move (m + 20, 70);
		//la_info.size (nana::size { 200, 300 });
		
		la_coords.move (nana::rectangle { 20, (int)n + 110, 200, 20 });
		la_coords.caption (L"Center of rectangle: ");
		
		ck_astrocyte.move (nana::rectangle (20, 20, 140, 40));
		ck_back_sub.move (nana::rectangle (20, 70, 140, 40));
		
		ck_astrocyte.radio (true);
		ck_back_sub.radio (true);

		ck_astrocyte.caption ("Astrocyte");
		ck_back_sub.caption ("Background subtraction");

		rg_picture.add (ck_astrocyte);
		rg_picture.add (ck_back_sub);

		bu_select_all.move (nana::rectangle { (int)m + 20, (int)n + 110, 100, 20 });
		bu_select_all.caption ("Select all");

		bu_deselect_all.move (nana::rectangle { (int)m + 130, (int)n + 110, 110, 20 });
		bu_deselect_all.caption ("Deselect all");

		bu_find.move (nana::rectangle { (int)m + 250, (int)n + 110, 100, 20 });
		bu_find.caption ("Look");
	}
	void roi (nana::rectangle rec)
	{
		rec_ = rec;
		la_coords.caption (L"center of rectangle: (" + to_wstring (rec_.x + rec_.width / 2) + L", " + to_wstring (rec_.y + rec_.height / 2) + L")");
	}
	nana::rectangle roi () { return rec_; }
};

struct segmentation_settings
{
	int a, min_points, eps;
	double thr_area, thr_time;
	segmentation_settings () : a (10), min_points (5), eps (5), thr_area (0.5), thr_time (0.5) {};
	// a - side of square, min_points - num of points in one cluster, eps - neighbourhood points in cluster,
	// thr_area ([0, 1]) - threshold of overlapping area , thr_time ([0, 1]) - threshold of overlapping time intervals
	segmentation_settings (int a_, int min_points_, int eps_, double thr_area_, double thr_time_) : 
		a (a_), min_points (min_points_), eps (eps_), thr_area (thr_area_), thr_time (thr_time_) {};
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

	// Segmenatation
	segmentation_settings segm_;

	vector <ushort> * mt;
	vector <vertex> * graph;
	std::map <int, vertex> components; // key - num of component
	std::set <int> cur_components;
	
	Mat cur_img;
	int cur_t, new_t;

	clusters_form my_form;
public:	
	astrocyte (fs::path file_name_, form & main_form, std::string var_name = "");
	
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


#endif