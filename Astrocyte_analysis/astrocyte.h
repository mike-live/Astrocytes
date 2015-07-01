#ifndef ASTROCYTE_H
#define ASTROCYTE_H

#include "declaration.h"
#include <mat.h>

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
	uchar * get_image (int t) { return data + nm * t * type_size; };
	Mat get_mat (int t) { return Mat (n, m, img_type, get_image (t)); };
	uchar * get_data () { return data; };
	uchar get_cell (int t, int i, int j) { return data[t * nm + i * m + j]; } 
};

struct vertex { int key; ushort start, finish; color color; ushort len () const { return finish - start + 1; }; };

class clusters_form : public panel<false>
{
public:
	label la_status;
	listbox li_components;
	picture pic;
	drawing dw_clusters;
	button bu_next;
	button bu_prev;
	bool dw_upd;
	clusters_form (form & main_form) : panel (main_form), dw_clusters (pic), la_status (*this), li_components (*this), pic (*this), bu_next (*this), bu_prev(*this), dw_upd (true)
	{
		caption (STR ("Clusters"));
		size (nana::size { 800, 800 });
		move (0, 40);
		
		la_status.size (nana::size { 200, 20 });
		la_status.move (110, 10);
		la_status.caption (STR ("Clusters waiting..."));

		main_form.size (nana::size { 800 + 20, 800 + 60});
		main_form.move (100, 100);
	}
	
	void status (std::wstring str) { la_status.caption (str); }
	void prepare (unsigned int n, unsigned int m)
	{
		la_status.caption (STR ("Click next image"));

		bu_next.size (nana::size { 40, 20 });
		bu_next.move (60, 10);
		bu_next.caption ("Next");

		bu_prev.size (nana::size { 40, 20 });
		bu_prev.move (10, 10);
		bu_prev.caption ("Prev");

		pic.size (nana::size { m, n });
		pic.move (10, 40);

		li_components.size (nana::size {320, 700});
		li_components.move (m + 20, 40);
		li_components.append_header (STR ("#"), 80);
		li_components.append_header (STR ("Color"), 80);
		li_components.append_header (STR ("Start"), 50);
		li_components.append_header (STR ("Finish"), 50);
		li_components.append_header (STR ("Length"), 50);
		li_components.checkable (true);
		li_components.auto_draw (true);
	}
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
	void calc_clusters (int a, int min_points, int eps, double thr_square, double thr_time);
	void dialog_clusters (int a, int min_points, int eps, double thr_square, double thr_time);
};


#endif