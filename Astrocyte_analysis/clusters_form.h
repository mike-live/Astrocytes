#pragma once

#include "declaration.h"

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
	clusters_form(form & main_form) :
		panel(main_form), dw_clusters(pic), la_status(*this), li_components(*this), pic(*this), bu_next(*this), bu_prev(*this), dw_upd(true),
		pic_points(*this), dw_points(pic_points), /*la_info (*this),*/ la_coords(*this), bu_select_all(*this), bu_deselect_all(*this), bu_find(*this)
	{
		caption(STR("Clusters"));
		size(nana::size{ 1000, 1000 });
		move(0, 40);

		la_status.size(nana::size{ 400, 20 });
		la_status.move(110, 10);
		la_status.caption(STR("Clusters waiting..."));

		main_form.size(nana::size{ 1000 + 20, 800 + 60 });
		main_form.move(100, 100);
	}

	void status(std::wstring str) { la_status.caption(str); }
	void prepare(unsigned int n, unsigned int m)
	{
		la_status.caption(STR("Click next image. Click on image = drawing clusters of square with center in position"));

		bu_next.size(nana::size{ 40, 20 });
		bu_next.move(60, 10);
		bu_next.caption("Next");

		bu_prev.size(nana::size{ 40, 20 });
		bu_prev.move(10, 10);
		bu_prev.caption("Prev");

		pic.size(nana::size{ m, n });
		pic.move(10, 40);

		li_components.size(nana::size{ 330, n });
		li_components.move(m + 20, 40);
		li_components.append_header(STR("#"), 80);
		li_components.append_header(STR("Color"), 80);
		li_components.append_header(STR("Start"), 50);
		li_components.append_header(STR("Finish"), 50);
		li_components.append_header(STR("Length"), 50);
		li_components.checkable(true);
		li_components.auto_draw(true);

		pic_points.move(10, n + 50);
		pic_points.size(nana::size{ m + 340, 50 });

		//la_info.move (m + 20, 70);
		//la_info.size (nana::size { 200, 300 });

		la_coords.move(nana::rectangle{ 20, (int)n + 110, 200, 20 });
		la_coords.caption(L"Center of rectangle: ");

		ck_astrocyte.move(nana::rectangle(20, 20, 140, 40));
		ck_back_sub.move(nana::rectangle(20, 70, 140, 40));

		ck_astrocyte.radio(true);
		ck_back_sub.radio(true);

		ck_astrocyte.caption("Astrocyte");
		ck_back_sub.caption("Background subtraction");

		rg_picture.add(ck_astrocyte);
		rg_picture.add(ck_back_sub);

		bu_select_all.move(nana::rectangle{ (int)m + 20, (int)n + 110, 100, 20 });
		bu_select_all.caption("Select all");

		bu_deselect_all.move(nana::rectangle{ (int)m + 130, (int)n + 110, 110, 20 });
		bu_deselect_all.caption("Deselect all");

		bu_find.move(nana::rectangle{ (int)m + 250, (int)n + 110, 100, 20 });
		bu_find.caption("Look");
	}
	void roi(nana::rectangle rec)
	{
		rec_ = rec;
		la_coords.caption(L"center of rectangle: (" + to_wstring(rec_.x + rec_.width / 2) + L", " + to_wstring(rec_.y + rec_.height / 2) + L")");
	}
	nana::rectangle roi() { return rec_; }
};