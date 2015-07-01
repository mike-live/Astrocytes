#include "astrocyte.h"

inline vector <ushort> dbscan (vector <ushort> & a, int min_points, int eps)
{
	vector <ushort> b (a.size ());
	int n = (int)a.size ();
	int l = 0, r = 0;
	for (int i = 0; i < n; i++) {
		while (l < i && (a[i] - a[l]) > eps) l++;
		while (r < n && (a[r] - a[i]) <= eps) r++;
		b[i] = r - l;
	}
	int pl = -1, pr = 0;
	ushort cluster_num = 1;
	l = 0, r = 0;
	bool flag = true;
	for (int i = 0; i < n; i++) {
		while (l < i && (a[i] - a[l]) > eps) l++;
		while (r < n && (a[r] - a[i]) <= eps) { if (b[r] >= min_points) pr = r; r++; }
		int num = r - l;
		if (pl >= l) {
			b[i] = b[pl];
		}
		if (num < min_points) {
			if (pl < l) {
				if (pr >= l) {
					b[i] = cluster_num++;
				} else {
					b[i] = cluster_num;
					cluster_num += 2;
				}
				flag = true;
			}
		} else {
			if (pl < l) {
				if (!flag) b[i] = cluster_num++;
				else b[i] = cluster_num - 1;
				flag = false;
			}
			pl = i;
		}
	}
	return b;
}


listbox::oresolver& operator<<(listbox::oresolver& ores, const vertex & v)
{
	return ores << v.key << v.color.rgba ().value << v.start << v.finish << v.len ();
}

void astrocyte::dialog_clusters (int a, int min_points, int eps, double thr_square, double thr_time)
{
	my_form.show ();

	calc_clusters (a, min_points, eps, thr_square, thr_time);

	my_form.prepare (intensity.n, intensity.m);

	my_form.dw_clusters.draw ([this, a](paint::graphics& gr)
	{
		my_form.li_components.auto_draw (false);

		if (cur_t != new_t) my_form.li_components.clear (), cur_components.clear ();
		Mat square (a, a, CV_8UC3);
		//memset (pos, 0, motion.nm * sizeof ushort);

		Mat img = intensity.get_mat (new_t), imgc, img_out;
		applyColorMap (img, imgc, COLORMAP_JET);
		imgc.copyTo (img_out);
		for (int i = 0; i < motion.n - a; i++) {
			for (int j = 0; j < motion.m - a; j++) {
				int ind = i * motion.m + j;
				int l = -1, r = (int)graph[ind].size();
				while (r - l > 1) {
					int m = (r + l) / 2;
					if (graph[ind][m].finish < new_t) l = m;
					else r = m;
				}
				if (r == graph[ind].size () || graph[ind][r].start > new_t) continue;
 				int cur = graph[ind][r].key;
				if (cur_t != new_t) cur_components.insert (cur);
				else if (cur_components.count (cur) == 0) continue;
				//int color = ((long long)cur * cur * 217313 + 1000000009) % (1 << 24);
				square.setTo (Scalar (0, 255, 0));
				Mat tmp = imgc.colRange (j, j + a).rowRange (i, i + a), out = img_out.colRange (j, j + a).rowRange (i, i + a);
				addWeighted (tmp, 0.5, square, 0.5, 0.0, out);
			}
		}
		if (cur_t != new_t) {
			auto cat = my_form.li_components.at (0);
			my_form.dw_upd = false;
			for (auto x : cur_components) {
				cat.append (components[x], true);
				cat.back ().check (true);
			}
			my_form.dw_upd = true;
		}
		my_form.li_components.auto_draw (true);
		cur_img = img_out;

		for (int i = 0; i < cur_img.rows; i++) {
			for (int j = 0; j < cur_img.cols; j++) {
				Vec3b color = cur_img.at<Vec3b> (i, j);
				gr.set_pixel (i, j, nana::color (color[2], color[1], color[0]));
			}
		}
		cur_t = new_t;
	});

	my_form.li_components.events ().checked ([this](const arg_listbox & events){
		//printf ("listbox event item : %d\n", events.item.value<int>);
		auto data = events.item.value_ptr<vertex> ();
		if (!events.selected) { cur_components.erase (data->key); }
		else { cur_components.insert (data->key); }
		//printf ("%d %d %d %d %d\n", data->start, data->finish, data->key, data->len (), data->color.rgba ().value);
		if (my_form.dw_upd) my_form.dw_clusters.update ();
	});

	my_form.bu_next.events ().click ([this]{
		new_t = (cur_t + 1) % motion.nt;
		my_form.dw_clusters.update ();
		my_form.la_status.caption (STR ("Current time frame = " + to_wstring (cur_t)));
	});

	my_form.bu_prev.events ().click ([this]{
		new_t = (cur_t - 1) % motion.nt;
		my_form.dw_clusters.update ();
		my_form.la_status.caption (STR ("Current time frame = " + to_wstring (cur_t)));
	});
}


void astrocyte::calc_clusters (int a, int min_points, int eps, double thr_square, double thr_time)
{
	background_subtraction ();
	//printf ("calc clusters begin\n");
	// calculate local maximum (mt)
	my_form.status(STR ("Local maximum calculating..."));
	int l = 3, r = 3, lr = r + l + 1; // time window [l .. r]
	vector <ushort>*  mt = new vector <ushort>[motion.nm]; // list of local maximum pixel (i, j) over time // mt = max time
	for (int t = l; t < motion.nt - r; t++)
		for (int i = 0; i < motion.n; i++)
			for (int j = 0; j < motion.m; j++) {
				uchar cur = motion.get_cell (t, i, j);
				if (cur < 1) continue;
				// search maximum from [l .. r] 
				uchar mxt = motion.get_cell (t + r, i, j);
				for (int k = t - l; k < t + r; k ++)
					if (k != t) mxt = max (motion.get_cell (k, i, j), mxt);
				
				// if cur is local maximum => add it 
				if (mxt <= cur) mt[i * motion.m + j].push_back (t);
			}
	
	//printf ("local maximum calculated\n");
	
	// calculate clusters and offset.
	my_form.status (STR ("Clusters calculating..."));
	int num = 0; // number of all segments
	int * offset = new int[motion.nm];
	bool * ft = new bool[motion.nt]; // true if time is local max for current pixel // ft = found time
	graph = new vector <vertex>[motion.nm];
	for (int i = 0; i < motion.n - a; i++)
		for (int j = 0; j < motion.m - a; j++) {
			int ind = i * motion.m + j;
			offset[ind] = num;
			if (mt[ind].size () == 0) continue;
			std::memset (ft, false, motion.nt * sizeof (bool));
			// for all local maximum t in square a * a set true in ft
			for (int x = i; x < i + a; x++) 
				for (int y = j; y < j + a; y++) {
					int ind2 = x * motion.m + y;
					for (int k = 0; k < mt[ind2].size (); k++) ft[mt[ind2][k]] = true;
				}
			std::vector<ushort> all_locmax; // all max without repetition
			for (int t = 0; t < motion.nt; t++) if (ft[t]) all_locmax.push_back (t);
			
			// get clusters with algo DBSCAN
			vector <ushort> clusters = dbscan (all_locmax, min_points, eps);
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
			//mt[ind] = res; // now mt stores begin/end of clusters of local maxima
			mt[ind].clear ();
			graph[ind] = res;
			num += (int)res.size ();
		}
	delete [] ft;
	delete [] mt;
	//printf ("clusters calculated\n");
	
	// ???!!! mt now is array of vectors of clustered segments for squares a * a with corner in (i, j)
	// calculate connected component
	my_form.status (STR ("Connected components calculating..."));
	
	// disjoint_sets stores connected components. for all vertices in one component method "find" return equal value
	// The time complexity is O(q alpha(q,num)), where alpha is the inverse Ackermann's function, 
	// q is the number of disjoint-set operations (make_set(), find_set(), and link() and n is the number of elements. 
	// The alpha function grows very slowly, much more slowly than the log function.
	int * rank = new int[num], *parent = new int[num]; // for disjoint_set
	boost::disjoint_sets <int *, int *> ds = boost::disjoint_sets <int *, int *> (rank, parent);

	for (int i = 0; i < num; i++) ds.make_set (i);
	
	ushort * pos = new ushort[motion.nm];
	std::memset (pos, 0, motion.nm * sizeof ushort);
	for (int t = 0; t < motion.nt; t++) {
		// update pos (index of segment with end greater than t)
		for (int i = 0; i < motion.n - a; i++)
			for (int j = 0; j < motion.m - a; j++) {
				int ind = i * motion.m + j;
				if (pos[ind] < graph[ind].size ()) if (graph[ind][pos[ind]].finish < t) pos[ind] ++;
			}
		// implementation of algo
		for (int i = 0; i < motion.n - a; i++)
			for (int j = 0; j < motion.m - a; j++) {
				int ind = i * motion.m + j;
				if (pos[ind] >= graph[ind].size () || graph[ind][pos[ind]].start > t) continue; // if segment not cover t : continue
				ushort be1 = graph[ind][pos[ind]].start, en1 = graph[ind][pos[ind]].finish; // first segment
				assert (offset[ind] + pos[ind] < num);
				for (int x = i; x < min (a + i, motion.n - a); x++) {
					for (int y = j; y < min (a + j, motion.m - a); y++) {
						int ind2 = x * motion.m + y;
						if (pos[ind2] >= graph[ind2].size () || graph[ind2][pos[ind2]].start > t) continue; // if segment not cover t : continue
						int s = (a + i - x) * (a + j - y); // overlapping area
						ushort be2 = graph[ind2][pos[ind2]].start, en2 = graph[ind2][pos[ind2]].finish; // second segment
						int len = max (min (en2, en1) - max (be2, be1) + 1, 0); // overllaping time
						assert (offset[ind2] + pos[ind2] < num);
						if (s > a * a * thr_square && len > max (en1 - be1, en2 - be2) * thr_time) // threshold by area and time
							ds.union_set (offset[ind] + pos[ind], offset[ind2] + pos[ind2]); // connect vertices of segments
					}
				}
			}
	}
	//printf ("connected components calculated\n");

	// calculate for all component begin and end time
	my_form.status (STR ("Calculating intervals, colors for components..."));
	components.clear ();
	std::memset (pos, 0, motion.nm * sizeof ushort);
	for (ushort t = 0; t < motion.nt; t++) {
		for (int i = 0; i < motion.n - a; i++) {
			for (int j = 0; j < motion.m - a; j++) {
				int ind = i * motion.m + j;
				if (pos[ind] < graph[ind].size ()) if (graph[ind][pos[ind]].finish < t) pos[ind] ++;
				if (pos[ind] >= graph[ind].size () || graph[ind][pos[ind]].start > t) continue; // if segment not cover t : continue
				int cur = ds.find_set (offset[ind] + pos[ind]);
				graph[ind][pos[ind]].key = cur;
				if (components.count (cur) == 1) { components[cur].finish = std::max (components[cur].finish, t); }
				else components[cur] = { cur, t, t, color (rand () % 127 + 127, rand () % 127 + 127, rand () % 127 + 127) };
				graph[ind][pos[ind]].color = components[cur].color;
			}
		}
	}
	delete[] pos;
	delete[] offset;
	delete[] rank;
	delete[] parent;
	my_form.status (STR ("Calculating end. =)"));
	//system ("pause");
	// print to file durations of all events (difference of max)
	
}


/*freopen ("d:\\durations.txt", "w", stdout);
	printf ("%d\n", all.size ());
	std::vector <std::pair <int, int>> duration;
	for (auto x : all) duration.push_back (x.second);
	std::sort (duration.begin (), duration.end (), [](std::pair<int, int> a, std::pair<int, int> b){ return a.second - a.first < b.second - b.first; });	
	for (auto x : duration) printf ("%d %d\n", x.first, x.second);*/

