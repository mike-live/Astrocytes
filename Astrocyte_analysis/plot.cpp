#include "plot.h"

namespace nana {

	nana::point plot2d::get_point (const real_point pt)
	{
		return nana::point {(int)(window_.x + window_.width * (pt.x - region_.x) / region_.width), 
							(int)(window_.y + window_.height* (pt.y - region_.y) / region_.height) };
	}

	void plot2d::line (std::vector <real_point> points, int point_radius)
	{
		if (!points.size ()) return;
		nana::point be = get_point (points[0]);
		gr_plot.line_begin (be.x, be.y);
		for (int i = 1; i < points.size (); i++) {
			nana::point cur = get_point(points[i]);
			gr_plot.line_to (cur);
			gr_plot.round_rectangle (rectangle { cur, nana::size {5,5} }, point_radius, point_radius, nana::color { 0, 0, 255 }, true, nana::color { 0, 0, 255 });
			//	point_radius
		}
	}
} 