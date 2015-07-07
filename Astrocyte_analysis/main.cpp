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

	button bu_open (main_form, nana::rectangle { 10, 10, 50, 20 });
	bu_open.caption (L"Open");
	
	bu_open.events ().click ([&main_form]{
		filebox fb(true); // open
		fb.add_filter (L"Mat File", L"*.mat");
		if (fb ())	// When user clicked 'OK'
		{
			//std::wstring file = L"F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\bm3d_2013-05-22_fileNo03_z-max.mat";
			std::wstring file = fb.file ();
			astro = new astrocyte (file, main_form);
			astro->preprocessing ();
		}
	});

	button bu_clusters (main_form, nana::rectangle { 70, 10, 50, 20 });
	bu_clusters.caption (L"Clusters");
	
	bu_clusters.events ().click ([]{
		astro->dialog_clusters (segmentation_settings { 5, 3, 5, 0.5, 0.5 });
	});
	main_form.show ();
	::nana::exec ();	
	return 0;
}

