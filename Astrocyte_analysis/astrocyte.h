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
	uchar * data {nullptr};
	bool del { true };
public:
	int n, m, nt, img_type, type_size, nm, size;
	video_data () : data (nullptr), n (0), m (0), nt (0), img_type (0), type_size (0), nm (0), size (0)
	{};
	video_data (uchar * data_, int n_, int m_, int nt_, int img_type_, bool del_ = true) :
		data (data_), n (n_), m (m_), nt (nt_), nm (n * m), size (nm * nt), img_type (img_type_), type_size (sizeof_opencv[img_type]), del (del_)
	{};
	~video_data () { if (del && data) delete[] data; }
	void clear () { if (del && data) delete [] data; };
	void reset (uchar * data_, int n_, int m_, int nt_, int img_type_, bool del_ = true) 
	{
		clear ();
		data = data_, n = n_, m = m_, nt = nt_, nm = n * m, size = nm * nt, img_type = img_type_, type_size = sizeof_opencv[img_type], del = del_;
	}
	uchar * frame (int t) { return data + nm * t * type_size; };
	Mat image (int t) { return Mat (n, m, img_type, frame (t)); };
	uchar * get_data () { return data; };
	uchar cell (int t, int i, int j) { return data[t * nm + i * m + j]; } 

	void set_nt (int nt) { this->nt = nt; this->size = nm * nt; }
};

struct vertex { 
	int key; 
	ushort start, finish; 
	color color; 
	struct { 
		int x1, y1, x2, y2; 
		int width () { return x2 - x1; } 
		int height () { return y2 - y1; }
		int area () { return width () * height (); }
	} bounding_box;
	ushort len () const { return finish - start + 1; }; 
};

class clusters_form : public panel<false>
{
private:
	nana::rectangle rec_;
public:
	label la_status;
	listbox li_components;
	
	button bu_next;
	button bu_prev;

	picture pic_points;
	drawing dw_points;
	//label la_info;
	label la_coords;

	textbox tx_t, tx_nt, tx_a, tx_eps, tx_min_points, tx_thr_area, tx_thr_time;
	label la_t, la_nt, la_a, la_eps, la_min_points, la_thr_area, la_thr_time;
	button bu_recalc;
	slider sd_time;
	std::function<void (int)> update_t;

	class astro_panel : public panel<false>
	{
	public:
		astro_panel (nana::window w) : panel (w), pic (*this), dw (pic), ck_astrocyte (*this), ck_back_sub (*this), ck_color (*this) {};
		picture pic;
		drawing dw;
		radio_group rg;
		checkbox ck_astrocyte;
		checkbox ck_back_sub;
		checkbox ck_color;
		void show (int x, int y, unsigned int m, unsigned int n)
		{
			move (x, y);
			size (nana::size {m, n + 100});
			pic.size (nana::size { m, n });
			pic.move (0, 0);
			ck_color.caption (L"With color");
			ck_color.move (nana::rectangle { 0, (int)n + 10, 250, 30 });

			ck_astrocyte.move (nana::rectangle (20, n + 40, 250, 30));
			ck_back_sub.move (nana::rectangle (20, n + 70, 250, 30));

			ck_astrocyte.radio (true);
			ck_back_sub.radio (true);

			ck_astrocyte.caption ("Astrocyte");
			ck_back_sub.caption ("Background subtraction");

			rg.add (ck_astrocyte);
			rg.add (ck_back_sub);

			ck_astrocyte.events ().click ([this]{dw.update (); });
			ck_back_sub.events ().click ([this]{dw.update (); });
			ck_color.events ().click ([this]{dw.update (); });
		}
	} left_image, right_image;

	button bu_select_all;
	button bu_deselect_all;
	button bu_look;
	button bu_select_max;

	bool dw_upd, select_max, t_upd;
	int look { 0 }, select_all {0};
	nana::rectangle look_region;	

	clusters_form (form & main_form) : 
		panel (main_form), left_image (*this), right_image (*this),
		la_status (*this), li_components (*this), bu_next (*this), bu_prev (*this), dw_upd (true),
		pic_points (*this), dw_points (pic_points), la_coords (*this), bu_select_all (*this), bu_deselect_all (*this), bu_look (*this),
		tx_t (*this), tx_nt (*this), tx_a (*this), tx_eps (*this), tx_min_points (*this), tx_thr_area (*this), tx_thr_time (*this),
		la_t (*this), la_nt (*this), la_a (*this), la_eps (*this), la_min_points (*this), la_thr_area (*this), la_thr_time (*this),
		bu_recalc (*this), bu_select_max (*this), sd_time (*this) 
	{
		caption (L"Clusters");
		size (nana::size { 1600, 1000 });
		move (0, 20);
		
		tx_t.move (nana::rectangle { 110, 30, 40, 20 });
		tx_t.tip_string (L"t");
		tx_t.multi_lines (false);

		tx_nt.move (nana::rectangle {160, 30, 40, 20});
		tx_nt.tip_string (L"nt");
		tx_nt.multi_lines (false);

		tx_a.move (nana::rectangle { 210, 30, 40, 20 });
		tx_a.tip_string (L"a");
		tx_a.multi_lines (false);

		tx_eps.move (nana::rectangle { 260, 30, 40, 20 });
		tx_eps.tip_string (L"eps");
		tx_eps.multi_lines (false);

		tx_min_points.move (nana::rectangle { 310, 30, 40, 20 });
		tx_min_points.tip_string (L"min_p");
		tx_min_points.multi_lines (false);

		tx_thr_area.move (nana::rectangle { 360, 30, 70, 20 });
		tx_thr_area.tip_string (L"thr_area");
		tx_thr_area.multi_lines (false);

		tx_thr_time.move (nana::rectangle { 440, 30, 70, 20 });
		tx_thr_time.tip_string (L"thr_time");
		tx_thr_time.multi_lines (false);
		

		la_t.move (nana::rectangle { 110, 10, 40, 20 });
		la_t.caption (L"t");

		la_nt.move (nana::rectangle { 160, 10, 40, 20 });
		la_nt.caption (L"nt");

		la_a.move (nana::rectangle { 210, 10, 40, 20 });
		la_a.caption (L"a");

		la_eps.move (nana::rectangle { 260, 10, 40, 20 });
		la_eps.caption (L"eps");

		la_min_points.move (nana::rectangle { 310, 10, 40, 20 });
		la_min_points.caption (L"min_p");

		la_thr_area.move (nana::rectangle { 360, 10, 70, 20 });
		la_thr_area.caption (L"thr_area");

		la_thr_time.move (nana::rectangle { 440, 10, 70, 20 });
		la_thr_time.caption (L"thr_time");		


		la_status.size (nana::size { 600, 20 });
		la_status.move (520, 10);
		la_status.caption (L"Clusters waiting...");

		main_form.size (nana::size { 1600 + 20, 800 + 60});
		main_form.move (50, 100); 
	}
	
	void status (std::wstring str) { la_status.caption (str); wcout << str << " " << clock () << endl; }
	void prepare (unsigned int n, unsigned int m)
	{
		la_status.caption (L"Click next image. Click on image = drawing clusters of square with center in position");

		bu_next.size (nana::size { 40, 20 });
		bu_next.move (60, 30);
		bu_next.caption (L"Next");

		bu_prev.size (nana::size { 40, 20 });
		bu_prev.move (10, 30);
		bu_prev.caption (L"Prev");

		bu_recalc.size (nana::size { 100, 20 });
		bu_recalc.move (520, 30);
		bu_recalc.caption (L"Recalculation");

		sd_time.move (nana::rectangle { 10, 55, m, 10 });

		left_image.show (10, 70, m, n);
		right_image.show (left_image.size().width + 20, 70, m, n);

		li_components.size (nana::size {490, n});
		li_components.move (2 * m + 40, 70);
		li_components.append_header (L"#", 80);
		li_components.append_header (L"Color", 80);
		li_components.append_header (L"Start", 50);
		li_components.append_header (L"Finish", 50);
		li_components.append_header (L"Length", 50);
		li_components.append_header (L"(x, y)", 80);
		li_components.append_header (L"(w, h)", 80);
		li_components.checkable (true);
		li_components.auto_draw (true);

		pic_points.move (10, n + 210);
		pic_points.size (nana::size { m + m + 20, 50 });

		//la_info.move (m + 20, 70);
		//la_info.size (nana::size { 200, 300 });
		
		la_coords.move (nana::rectangle { 20, (int)n + 280, 200, 20 });
		la_coords.caption (L"Center of rectangle: ");

		bu_select_all.move (nana::rectangle { (int)m + 20, (int)n + 170, 100, 20 });
		bu_select_all.caption ("Select all");

		bu_deselect_all.move (nana::rectangle { (int)m + 130, (int)n + 170, 110, 20 });
		bu_deselect_all.caption ("Deselect all");

		bu_look.move (nana::rectangle { (int)m + 250, (int)n + 170, 100, 20 });
		bu_look.caption ("Look");

		bu_select_max.move (nana::rectangle { (int)m + 370, (int)n + 170, 100, 20 });
		bu_select_max.caption ("Select max");
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
	video_data * current;
	mxArray * pa;
	Mat thr_max, thr_gauss_max;
	Mat max_thr (bool gauss = true);
	
	Mat thr_min, thr_gauss_min;
	Mat min_thr (bool gauss = true);

	// Morphology
	vector<vector<Point>> contours, soma;
	vector<Vec4i> hierarchy;

	// Segmenatation
	segmentation_settings segm;

	vector <ushort> * mt { nullptr };
	vector <vertex> * graph { nullptr };
	std::map <int, vertex> components; // key - num of component
	std::unordered_map <int, bool> cur_components; // key - num of component, data - checked
	
	Mat cur_img;
	int cur_t, new_t;
	int max_component;

	clusters_form my_form;
public:	
	astrocyte (fs::path file_name_, form & main_form, std::string var_name = "");
	

	void preprocessing ();
	void clear_data () { mxDestroyArray (pa); data.clear(); };
	void background_subtraction ();
	
	// Morphology
	vector<vector<Point>> & find_morphology ();
	void draw_morphology (fs::path file_name, bool video = true);
	void find_soma ();

	// Segmentation
	void calc_clusters ();
	void dialog_clusters ();
	void dialog_dbscan ();
	void calculation ();
};


#endif