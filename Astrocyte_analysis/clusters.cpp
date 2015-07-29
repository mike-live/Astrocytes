#include "astrocyte.h"

inline vector <ushort> dbscan (vector <ushort> & a, int min_points, int eps)
{
	int n = (int)a.size ();
	vector <pair <int, int>> b;
	vector <ushort> c;
	b.reserve (n);
	int l = 0, r = 0;
	for (int i = 0; i < n; i++) {
		while (l < i && a[i] - a[l] > eps) l++;
		while (r < n && a[r] - a[i] <= eps) r++;
		if (r - l >= min_points) b.push_back (make_pair (a[l], a[r - 1]));
		else b.push_back (make_pair (-1, -1));
	}
	r = b[0].second;
	ushort cur = 0;
	bool flag = b[0].first != -1;
	if (flag) c.push_back (cur);
	else c.push_back (65535);
	for (int i = 1; i < n; i++) {
		if (flag) {
			if (a[i] <= r) {
				r = max (b[i].second, r);
				c.push_back (cur);
			} else {
				r = b[i].second;
				if (b[i].first == -1) r = a[i], flag = false;
				c.push_back (65535);
			}
		} else {
			if (b[i].first != -1) {
				int k = i - 1;
				cur++;
				while (k >= 0 && c[k] == 65535 && a[k] >= b[i].first) c[k] = cur, k--;
				r = b[i].second;
				c.push_back (cur);
				flag = true;
			} else {
				c.push_back (65535);
			}
		}
	}
	for (int i = 0; i < n; i++) if (c[i] == 65535) c[i] = ++cur;
	return c;
}

listbox::oresolver& operator<<(listbox::oresolver& ores, const vertex & v)
{
	ores << v.key << v.color.rgba ().value << v.start << v.finish << v.len ();
	ores << L"(" + to_wstring (v.bounding_box.x1) + L", " + to_wstring (v.bounding_box.y1) + L")";
	ores << L"(" + to_wstring (v.bounding_box.x2 - v.bounding_box.x1) + L", " + to_wstring (v.bounding_box.y2 - v.bounding_box.y1) + L")";
	return ores;
}

void astrocyte::calculation ()
{
	current = &motion;
	calc_clusters ();
	int cur_volume = 0;
	for (auto x : components) {
		int volume = x.second.len () * x.second.bounding_box.width () * x.second.bounding_box.height ();
		if (volume > cur_volume) { max_component = x.first; cur_volume = volume; }
	}
	//find_soma ();
}

void astrocyte::dialog_clusters ()
{	
	segm = { 10, 5,  3, 0.2, 0.5 };
	current = &motion;

	my_form.tx_t.caption (to_wstring (0)); my_form.sd_time.value (0);
	my_form.tx_nt.caption (to_wstring (intensity.nt)); my_form.sd_time.vmax (intensity.nt);
	my_form.tx_a.caption(to_wstring(segm.a));
	my_form.tx_eps.caption (to_wstring (segm.eps));
	my_form.tx_min_points.caption (to_wstring (segm.min_points));
	my_form.tx_thr_area.caption (to_wstring (segm.thr_area));
	my_form.tx_thr_time.caption (to_wstring (segm.thr_time));
	
	my_form.show ();

	calculation ();

	my_form.events ().destroy ([this]{
		delete[] mt;
	});

	my_form.prepare (intensity.n, intensity.m);

	my_form.left_image.dw.draw ([this] (paint::graphics& gr)
	{
		Mat square (segm.a, segm.a, CV_8UC3);
		//memset (pos, 0, current->nm * sizeof ushort);

		Mat img, imgc, img_out;
		if (my_form.left_image.ck_astrocyte.checked ()) img = intensity.image (new_t);
		else img = motion.image (new_t);
		if (my_form.left_image.ck_color.checked()) applyColorMap (img, imgc, COLORMAP_JET);
		else cv::cvtColor (img, imgc, CV_GRAY2RGB);
		imgc.copyTo (img_out);

		std::unordered_map <int, bool> new_cur;

		for (int i = 0; i < current->n - segm.a; i++) {
			for (int j = 0; j < current->m - segm.a; j++) {
				int ind = i * current->m + j;
				int l = -1, r = (int)graph[ind].size();
				while (r - l > 1) {
					int m = (r + l) / 2;
					if (graph[ind][m].finish < new_t) l = m;
					else r = m;
				}
				if (r == graph[ind].size () || graph[ind][r].start > new_t) continue;

 				int key = graph[ind][r].key;
				bool checked = false;
				if (cur_components.count (key)) checked = cur_components[key];
				if (my_form.look && my_form.look_region.is_hit (j, i)) checked = true;
				if (my_form.select_all) checked = true;
				if (my_form.select_max && key == max_component) checked = true;
				/*if (pointPolygonTest (soma[0], Point2i (j, i), false) > 1e-9 || 
					pointPolygonTest (soma[0], Point2i (j + segm.a, i), false) > 1e-9 ||
					pointPolygonTest (soma[0], Point2i (j, i + segm.a), false) > 1e-9 || 
					pointPolygonTest (soma[0], Point2i (j + segm.a, i + segm.a), false) > 1e-9) checked = false;*/
				
				new_cur.insert ({key, checked});
				if (!checked) continue;

				//int color = ((long long)cur * cur * 217313 + 1000000009) % (1 << 24);
				square.setTo (Scalar (graph[ind][r].color.b(), graph[ind][r].color.g(), graph[ind][r].color.r()));
				//square.setTo (Scalar (255, 255, 255));
				Mat tmp = imgc.colRange (j, j + segm.a).rowRange (i, i + segm.a), out = img_out.colRange (j, j + segm.a).rowRange (i, i + segm.a);
				cv::addWeighted (tmp, 0.5, square, 0.5, 0.0, out);
			}
		}
		//drawContours (img_out, soma, 0, Scalar (255, 255, 255), 2, 8);

		cur_img = img_out;

		for (int i = 0; i < cur_img.rows; i++) {
			for (int j = 0; j < cur_img.cols; j++) {
				Vec3b color = cur_img.at<Vec3b> (i, j);
				gr.set_pixel (j, i, nana::color (color[2], color[1], color[0]));
			}
		}
		if (my_form.look) gr.rectangle (my_form.look_region, false, nana::color { 0, 255, 0 });
		cur_components = new_cur;

		my_form.li_components.auto_draw (false);
		auto cat = my_form.li_components.at (0);
		set<int> selected;
		for (auto x : my_form.li_components.selected ()) selected.insert (cat.at (x.item).value_ptr <vertex> ()->key);		
		//printf ("selected: "); for (auto key : selected) printf ("%d ", key); printf ("\n");
		my_form.dw_upd = false;
		if (cur_t != new_t) {
			my_form.li_components.clear ();

			for (auto x : cur_components) {
				auto cur = components[x.first];
				auto item = cat.append (cur, true);
				item.check (x.second);
				if (selected.count (x.first)) {
					item.select (true);
					unsigned width = cur.bounding_box.x2 - cur.bounding_box.x1, height = cur.bounding_box.y2 - cur.bounding_box.y1;
					//printf ("%d : %d %d %d %d\n", x.first, cur.bounding_box.x1, cur.bounding_box.y1, (int)width, (int)height);
					gr.rectangle (nana::rectangle { cur.bounding_box.x1, cur.bounding_box.y1, width, height }, false, color { 0, 183, 255 });
				}
			}			
		} else {
			for (auto item : cat) {
				if (!item.value_ptr <vertex> ()) continue;
				int key = item.value_ptr <vertex> ()->key;
				item.check (cur_components[key]);
				//if (selected.count (key)) item.select (true);
			}
		}
		my_form.dw_upd = true;
		
		my_form.li_components.auto_draw (true);

		gr.rectangle (my_form.roi (), false, color {255, 255, 255});
		cur_t = new_t;
		my_form.right_image.dw.update ();
	});

	my_form.right_image.dw.draw ([this](paint::graphics& gr){
		Mat img, imgc;
		if (my_form.right_image.ck_astrocyte.checked ()) img = intensity.image (new_t);
		else img = motion.image (new_t);
		if (my_form.right_image.ck_color.checked ()) applyColorMap (img, imgc, COLORMAP_JET);
		else cv::cvtColor (img, imgc, CV_GRAY2RGB);
		for (int i = 0; i < imgc.rows; i++) {
			for (int j = 0; j < imgc.cols; j++) {
				Vec3b color = imgc.at<Vec3b> (i, j);
				gr.set_pixel (j, i, nana::color (color[2], color[1], color[0]));
			}
		}
	});

	// List of clusters
	{
		my_form.li_components.events ().selected ([this](const arg_listbox & events){
			if (my_form.dw_upd) my_form.left_image.dw.update ();
			events.stop_propagation ();
		});

		my_form.li_components.events ().checked ([this](const arg_listbox & events){
			auto data = events.item.value_ptr<vertex> ();
			if (!events.selected) { cur_components[data->key] = false; } else { cur_components[data->key] = true; }
			//printf ("%d %d %d %d %d\n", data->start, data->finish, data->key, data->len (), data->color.rgba ().value);
			if (my_form.dw_upd) my_form.left_image.dw.update ();
		});

		// Compare for keys
		my_form.li_components.set_sort_compare (0, [](const nana::string &, nana::any * a, const nana::string &, nana::any * b, bool reverse)->bool{
			auto value_a = a->get<vertex> ()->key;
			auto value_b = b->get<vertex> ()->key;
			return reverse ? value_a > value_b : value_a < value_b;
		});

		// Compare for time length
		my_form.li_components.set_sort_compare (4, [](const nana::string &, nana::any * a, const nana::string &, nana::any * b, bool reverse)->bool{
			auto value_a = a->get<vertex> ()->len ();
			auto value_b = b->get<vertex> ()->len ();
			return reverse ? value_a > value_b : value_a < value_b;
		});

		// Compare for left up corner
		my_form.li_components.set_sort_compare (5, [](const nana::string &, nana::any * a, const nana::string &, nana::any * b, bool reverse)->bool{
			auto & value_a = a->get<vertex> ()->bounding_box;
			auto & value_b = b->get<vertex> ()->bounding_box;
			return reverse ^ std::less <pair <int, int>> () (make_pair (value_a.x1, value_a.y1), make_pair (value_b.x1, value_b.y1));
		});

		// Compare for area
		my_form.li_components.set_sort_compare (6, [](const nana::string &, nana::any * a, const nana::string &, nana::any * b, bool reverse)->bool{
			auto value_a = a->get<vertex> ()->bounding_box.area ();
			auto value_b = b->get<vertex> ()->bounding_box.area ();
			return reverse ? value_a > value_b : value_a < value_b;
		});
	} 

	// Time line
	{
		my_form.update_t = [this](int t){
			if (my_form.t_upd) return;
			if (t == cur_t) return;
			my_form.t_upd = true;
			new_t = t;
			my_form.sd_time.value (t);
			my_form.tx_t.caption (to_wstring (t));
			my_form.left_image.dw.update ();
			my_form.t_upd = false;
		};

		my_form.bu_next.events ().click ([this]{
			my_form.update_t((cur_t + 1) % current->nt);			
		});

		my_form.bu_prev.events ().click ([this]{
			my_form.update_t ((cur_t + current->nt - 1) % current->nt);
		});

		my_form.sd_time.events ().value_changed ([this]{
			my_form.update_t (my_form.sd_time.value ());
		});

		my_form.tx_t.events ().focus ([this]{
			my_form.update_t (stol (my_form.tx_t.caption ()));
		});
		
		my_form.tx_t.events ().key_press ([this](const arg_keyboard& arg){
			if (arg.key == '\r') my_form.update_t (stol (my_form.tx_t.caption ()));
		});
	}

	// Interface for dbscan
	my_form.left_image.pic.events ().click ([this](const arg_mouse& arg){

		if (my_form.look == 0) {
			my_form.roi (nana::rectangle (arg.pos.x - segm.a / 2, arg.pos.y - segm.a / 2, segm.a, segm.a));
			my_form.left_image.dw.update ();
			my_form.dw_points.update ();
		} else {
			if (my_form.look == 1) {
				my_form.look_region.set_pos ({ arg.pos.x, arg.pos.y});
			} else {
				nana::size sz { (nana::size::value_type)abs (arg.pos.x - my_form.look_region.x), 
								(nana::size::value_type)abs (arg.pos.y - my_form.look_region.y) };
				my_form.look_region.set_size (sz);
				my_form.look_region.set_pos ({ min (my_form.look_region.x, arg.pos.x), min (my_form.look_region.y, arg.pos.y) });
				my_form.left_image.dw.update ();
			}
			my_form.look = 3 - my_form.look;
		}
	});
	my_form.dw_points.draw ([this](paint::graphics& gr){
		gr.rectangle (true, { 255, 255, 255 });
		int ind = (my_form.roi ().y * current->m + my_form.roi ().x);
		if (mt[ind].size () == 0) {
			gr.string ({ 20, 20 }, L"No local maximum");
			return;
		}
		int len = my_form.pic_points.size ().width - 40;
		int xb = 20;
		gr.line ({ xb, 20 }, { (int)my_form.pic_points.size ().width - 20, 20 }, nana::color {0,0,0});
		int center = 0;
		for (int i = 0; i < mt[ind].size (); i++) {
			int xg = (len * mt[ind][i]) / current->nt + xb;
			//gr.round_rectangle ({ xg, 17, 4, 4 }, 1, 1, { 175, 216, 255 }, true, { 175, 216, 255 });
			gr.line ({ xg, 17 }, { xg, 21 }, nana::color { 175, 216, 255 });
			center += xg;
		}
		center /= (int)mt[ind].size ();
		gr.round_rectangle ({ center, 17, 5, 5 }, 1, 1, { 160, 255, 134 }, true, { 160, 255, 134 });
		double dt = 50.0 * (double)current->nt / len;
		for (double t = 0; t < current->nt; t += dt) {
			int xg = (int)(len * t / current->nt) + xb;
			gr.string ({ xg - 3, 20 }, to_wstring ((int)t));
		}
		for (auto interval : graph[ind]) {
			int be = (int)(len * interval.start / current->nt) + xb, en = (len * interval.finish / current->nt) + xb;
			gr.line ({ be, 15 }, { en, 15 }, nana::color { 97, 255, 55 });
			gr.line ({ be, 14 }, { be, 16 }, nana::color { 255, 55, 55 });
			gr.line ({ en, 14 }, { en, 16 }, nana::color { 255, 55, 55 });
		}
	});
	my_form.bu_select_all.enable_pushed (true);

	my_form.bu_select_all.events ().click ([this]{
		if (my_form.bu_select_all.pushed ()) my_form.select_all = 1;
		else my_form.select_all = 0;
		//my_form.dw_upd = false;
		//for (auto x : my_form.li_components.at (0)) x.check (true);		
		//my_form.dw_upd = true;
		my_form.left_image.dw.update ();
	});

	my_form.bu_deselect_all.events ().click ([this]{
		if (my_form.bu_select_all.pushed ()) {
			my_form.select_all = 0; 
			my_form.bu_select_all.pushed (false);
		}
		my_form.dw_upd = false;
		for (auto x : my_form.li_components.at (0)) x.check (false);
		my_form.dw_upd = true;
		my_form.left_image.dw.update ();
	});

	my_form.bu_look.enable_pushed (true);

	my_form.bu_look.events ().click ([this]{
		if (my_form.bu_look.pushed ()) my_form.look = 1;
		else {
			my_form.look = 0;
			my_form.left_image.dw.update ();
		}
	});

	my_form.bu_select_max.enable_pushed (true);
	my_form.bu_select_max.events ().click ([this]{
		if (my_form.bu_select_max.pushed ()) my_form.select_max = true;
		else my_form.select_max = false;
		my_form.left_image.dw.update ();
	});

	my_form.bu_recalc.events ().click ([this]{
		thr_max.release ();
		thr_gauss_max.release ();
		thr_min.release ();
		thr_gauss_min.release ();		
		intensity.set_nt (stol (my_form.tx_nt.caption ())); 
		if (cur_t >= intensity.nt) {
			my_form.tx_t.caption (to_wstring (0)); 
			my_form.sd_time.value (0);
		}
		my_form.sd_time.vmax (intensity.nt);
		segm.a = stol(my_form.tx_a.caption ());
		segm.eps = stol(my_form.tx_eps.caption ());
		segm.min_points = stol(my_form.tx_min_points.caption ());
		segm.thr_area = stod(my_form.tx_thr_area.caption ());
		segm.thr_time = stod(my_form.tx_thr_time.caption ());
		calculation ();
		new_t = cur_t;
		cur_t = 0;
		my_form.update_t (new_t);
	});
}


void astrocyte::calc_clusters ()
{
	background_subtraction ();
	current = &motion;
	//printf ("calc clusters begin\n");
	// calculate local maximum (mt)
	my_form.status(L"Local maximum calculating...");	
	int l = 3, r = 3, lr = r + l + 1; // time window [l .. r]
	if (mt) delete[] mt;
	mt = new vector <ushort>[current->nm]; // list of local maximum pixel (i, j) over time // mt = max time
	for (int t = l; t < current->nt - r; t++)
		for (int i = 0; i < current->n; i++)
			for (int j = 0; j < current->m; j++) {
				uchar cur = current->cell (t, i, j);
				if (cur < 1) continue;
				// search maximum from [l .. r] 
				uchar mxt = current->cell (t + r, i, j);
				for (int k = t - l; k < t + r; k ++)
					if (k != t) mxt = max (current->cell (k, i, j), mxt);
				
				// if cur is local maximum => add it 
				if (mxt <= cur) mt[i * current->m + j].push_back (t);
			}
	
	//printf ("local maximum calculated\n");
	
	// calculate clusters and offset.
	my_form.status (L"Clusters calculating...");
	int num = 0; // number of all segments
	int * offset = new int[current->nm];
	bool * ft = new bool[current->nt]; // true if time is local max for current pixel // ft = found time
	if (graph) delete[] graph;
	graph = new vector <vertex>[current->nm];
	
	for (int i = 0; i < current->n - segm.a; i++)
		for (int j = 0; j < current->m - segm.a; j++) {			
			int ind = i * current->m + j;
			int len = (int)mt[ind].size ();
			offset[ind] = num;
			if (len == 0) continue;
			std::memset (ft, false, current->nt * sizeof (bool));
			// for all local maximum t in square a * a set true in ft
			for (int x = i; x < i + segm.a; x++)
				for (int y = j; y < j + segm.a; y++) {
					int ind2 = x * current->m + y;
					for (int k = 0; k < mt[ind2].size (); k++) ft[mt[ind2][k]] = true;
				}
			std::vector<ushort> all_locmax; // all max without repetition
			for (int t = 0; t < current->nt; t++) if (ft[t]) all_locmax.push_back (t);
			
			// get clusters with algo DBSCAN
			vector <ushort> clusters = dbscan (all_locmax, segm.min_points, segm.eps);
			vector <vertex> res;
			// get all begin/end of clusters and add it into res
			res.push_back ({ -1, all_locmax.front (), -1 });
			for (int k = 0; k < clusters.size () - 1; k++) {
				if (clusters[k] != clusters[k + 1]) {
					res.back().finish = all_locmax[k];
					res.push_back ({ -1, all_locmax[k + 1], -1 });
				}
			}
			res.back ().finish = all_locmax.back();
			mt[ind] = all_locmax;
			graph[ind] = res;
			num += (int)res.size ();
		}
	// mt now is array of local max in (i, j) by t
	delete [] ft;
	//printf ("clusters calculated\n");
	
	
	// calculate connected component
	my_form.status (L"Connected components calculating...");
	
	// disjoint_sets stores connected components. for all vertices in one component method "find" return equal value
	// The time complexity is O(q alpha(q,num)), where alpha is the inverse Ackermann's function, 
	// q is the number of disjoint-set operations (make_set(), find_set(), and link() and n is the number of elements. 
	// The alpha function grows very slowly, much more slowly than the log function.
	int * rank = new int[num], *parent = new int[num]; // for disjoint_set
	boost::disjoint_sets <int *, int *> ds = boost::disjoint_sets <int *, int *> (rank, parent);

	for (int i = 0; i < num; i++) ds.make_set (i);
	
	ushort * pos = new ushort[current->nm];
	std::memset (pos, 0, current->nm * sizeof ushort);
	for (int t = 0; t < current->nt; t++) {
		// update pos (index of segment with end greater than t)
		for (int i = 0; i < current->n - segm.a; i++)
			for (int j = 0; j < current->m - segm.a; j++) {
				int ind = i * current->m + j;
				if (pos[ind] < graph[ind].size ()) if (graph[ind][pos[ind]].finish < t) pos[ind] ++;
			}
		// implementation of algo
		for (int i = 0; i < current->n - segm.a; i++)
			for (int j = 0; j < current->m - segm.a; j++) {
				int ind = i * current->m + j;
				if (pos[ind] >= graph[ind].size () || graph[ind][pos[ind]].start > t) continue; // if segment not cover t : continue
				ushort be1 = graph[ind][pos[ind]].start, en1 = graph[ind][pos[ind]].finish; // first segment
				assert (offset[ind] + pos[ind] < num);
				for (int x = i; x < min (segm.a + i, current->n - segm.a); x++) {
					for (int y = j; y < min (segm.a + j, current->m - segm.a); y++) {
						int ind2 = x * current->m + y;
						if (pos[ind2] >= graph[ind2].size () || graph[ind2][pos[ind2]].start > t) continue; // if segment not cover t : continue
						int s = (segm.a + i - x) * (segm.a + j - y); // overlapping area
						ushort be2 = graph[ind2][pos[ind2]].start, en2 = graph[ind2][pos[ind2]].finish; // second segment
						int len = max (min (en2, en1) - max (be2, be1) + 1, 0); // overllaping time
						assert (offset[ind2] + pos[ind2] < num);
						if (s > segm.a * segm.a * segm.thr_area && len > max (en1 - be1, en2 - be2) * segm.thr_time) // threshold by area and time
							ds.union_set (offset[ind] + pos[ind], offset[ind2] + pos[ind2]); // connect vertices of segments
					}
				}
			}
	}
	//printf ("connected components calculated\n");

	// calculate for all component begin and end time
	my_form.status (L"Calculating intervals, colors for components...");
	components.clear ();
	std::memset (pos, 0, current->nm * sizeof ushort);
	for (ushort t = 0; t < current->nt; t++) {
		for (int i = 0; i < current->n - segm.a; i++) {
			for (int j = 0; j < current->m - segm.a; j++) {
				int ind = i * current->m + j;
				if (pos[ind] < graph[ind].size ()) if (graph[ind][pos[ind]].finish < t) pos[ind] ++;
				if (pos[ind] >= graph[ind].size () || graph[ind][pos[ind]].start > t) continue; // if segment not cover t : continue
				int cur = ds.find_set (offset[ind] + pos[ind]);
				graph[ind][pos[ind]].key = cur;
				if (components.count (cur) == 1) { components[cur].finish = std::max (components[cur].finish, t); }
				else components[cur] = { cur, t, t, color (rand () % 127 + 127, rand () % 127 + 127, rand () % 127 + 127), { j, i, j + segm.a, i + segm.a } };
				graph[ind][pos[ind]].color = components[cur].color;
				auto & rec = components[cur].bounding_box;
				rec = { min (rec.x1, j), min (rec.y1, i), max (rec.x2, j + segm.a), max (rec.y2, i + segm.a) };
			}
		}
	}
	delete[] pos;
	delete[] offset;
	delete[] rank;
	delete[] parent;
	my_form.status (L"Calculating end. =)");
}


/*freopen ("d:\\durations.txt", "w", stdout);
	printf ("%d\n", all.size ());
	std::vector <std::pair <int, int>> duration;
	for (auto x : all) duration.push_back (x.second);
	std::sort (duration.begin (), duration.end (), [](std::pair<int, int> a, std::pair<int, int> b){ return a.second - a.first < b.second - b.first; });	
	for (auto x : duration) printf ("%d %d\n", x.first, x.second);*/

