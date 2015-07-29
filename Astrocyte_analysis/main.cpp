/*
main.cpp - файл, содержащий реализацию основной функции

*/
/*
#include <cstdio>
#include <mat.h>
#include <ctime>
#include <iostream>
*/
#include "declaration.h"


 
#include "astrocyte.h"
//#include "morphology.h"
//#include "algo_1.h"

astrocyte * astro;

int main ()
{
	form main_form { API::make_center (200, 100) };
	
	main_form.caption (L"Astrocyte analysis");

	//button bu_open (main_form, nana::rectangle { 10, 10, 50, 20 });
	//bu_open.caption (L"Open");
	
	/*button bu_clusters (main_form, nana::rectangle { 70, 10, 50, 20 });
	bu_clusters.caption (L"Clusters");
	bu_clusters.hide ();*/


	//filebox fb(true); // open
	//fb.add_filter (L"Mat File", L"*.mat");
	//if (fb ())	// When user clicked 'OK'
	//{

	//}

	std::wstring file = L"F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\bm3d_2013-05-22_fileNo03_z-max.mat";
	//std::wstring file = fb.file ();
	astro = new astrocyte (file, main_form);
	astro->preprocessing ();
	//astro->draw_morphology ("F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\0.avi", true);
	astro->dialog_clusters ();
	
	
	/*vector<vector<Point>> my_contours;
	vector<Vec4i> my_hierarchy;
	Mat thr = astro->min_thr ();
	findContours (thr, my_contours, my_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);*/

	//bu_open.events ().click ([&main_form, &bu_clusters]{
					/*segmentation_settings settings { 15, 3, 5, 0.5, 0.5 };
			//printf ("a, eps, min_points, thr_area, thr_time\n");
			FILE * bu = fopen ("F:\\PC\\GitHub\\Astrocytes\\Astrocyte_analysis\\settings.txt", "r");
			fscanf (bu, "%d%d%d%lf%lf", &settings.a, &settings.eps, &settings.min_points, &settings.thr_area, &settings.thr_time);
			fclose (bu);
			printf ("here\n");*/
						
	//});
		
	//bu_clusters.events ().click ([]{
		/*segmentation_settings settings { 15, 3, 5, 0.5, 0.5 };
		//printf ("a, eps, min_points, thr_area, thr_time\n");
		FILE * bu = fopen ("settings.txt", "r");
		fscanf (bu, "%d%d%d%lf%lf\n", settings.a, settings.eps, settings.min_points, settings.thr_area, settings.thr_time);
		fclose (bu);
		astro->dialog_clusters (settings);*/
		
	//});
	main_form.show ();
	::nana::exec ();	
	return 0;
}

