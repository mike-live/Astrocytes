#ifndef PLOT_H
#define PLOT_H

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/picture.hpp>

using namespace nana;

namespace nana {

	struct real_rectangle
	{
		double x, y, width, height;
		real_rectangle () : x (0.0), y (0.0), width (0.0), height (0.0)
		{}

		real_rectangle (double x, double y, double width, double height) : x (x), y (y), width (width), height (height)
		{}
		 
	};

	struct real_point
	{
		double x, y;
	};

	class plot2d
	{
	private:
		paint::graphics gr_plot;
		rectangle window_;
		real_rectangle region_;
		bool axis_x_ { true }, axis_y_ {true};
	public:
		plot2d (paint::graphics & gr) : gr_plot (gr) {}
		void window (rectangle window) { window_ = window; }
		rectangle window () { return window_; }

		void region (real_rectangle region) { region_ = region; }
		real_rectangle region () { return region_; }

		bool axis_x () { return axis_x_; }
		void axis_x (bool draw) { axis_x_ = draw; }

		bool axis_y () { return axis_y_; }
		void axis_y (bool draw) { axis_y_ = draw; }

		void line (std::vector <real_point>, int point_radius = 0);
		nana::point get_point (const real_point pt);
	};
};

#endif