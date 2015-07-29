/*
main.cpp - файл, содержащий реализацию основной функции
*/

#include "config.h"
#include "declaration.h"
#include "astrocyte.h"
#include "form_logger.h"

astrocyte * astro;

int main ()
{
	form main_form { API::make_center (FORM_WIDTH, FORM_HEIGHT) };
	
	main_form.caption (L"Astrocyte analysis");

	button bu_open (main_form, BUTTON_OPEN);
	bu_open.caption (L"Open");
	
	bu_open.events ().click ([&main_form]{
		filebox fb(true); // open
		fb.add_filter (L"Mat File", L"*.mat");
		if (fb ())	// When user clicked 'OK'
		{
			std::wstring file = fb.file ();
			astro = new astrocyte(file, main_form, new FormLogger(main_form));
			astro->preprocessing ();
		}
	});

	button bu_clusters (main_form, BUTTON_CLUSTERS);
	bu_clusters.caption (L"Clusters");
	
	bu_clusters.events ().click ([]{
		astro->dialog_clusters (segmentation_settings { SQUARE_SIDE, 
			MIN_CLUSTER_POINTS, NUM_NEIGBOR_POINTS, AREA_THRESHOLD, TIME_THRESHOLD });
	});

	main_form.show ();
	::nana::exec ();	

	return 0;
}

