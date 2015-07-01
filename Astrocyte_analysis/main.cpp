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

/*using namespace cv;
using namespace std;
using namespace fs;
*/

astrocyte * astro;

int main ()
{
	//RNG rng (12345);
	form main_form { API::make_center (200, 100) };
	
	main_form.caption (STR ("Astrocyte analysis"));

	button bu_open (main_form, nana::rectangle { 10, 10, 50, 20 });
	bu_open.caption (STR("Open"));
	
	bu_open.events ().click ([&main_form]{
		/*filebox fb(true);
		fb.add_filter (L"Mat File", L"*.mat");
		if (fb ())	//When user clicked 'OK'
		{*/
		std::wstring file = STR("F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\bm3d_2013-05-22_fileNo03_z-max.mat");//fb.file ();
			astro = new astrocyte (file, main_form);
			astro->preprocessing ();
		//}
	});

	button bu_clusters (main_form, nana::rectangle { 70, 10, 50, 20 });
	bu_clusters.caption (STR ("Clusters"));
	
	bu_clusters.events ().click ([]{
		
		
		astro->dialog_clusters (30, 5, 10, 0.5, 0.5);
	});
	main_form.show ();
	::nana::exec ();
	
	//li.append ({ STR ("double"), STR ("double2") });
	//li.at (0).append (std::initializer_list <std::wstring> ({ STR ("double"), STR ("double2") }));
	
	//li.insert (listbox::index_pair ({ 0, 0 }), STR ("int00") );
	//li.insert (listbox::index_pair ({ 0, 1 }), STR ("int01"));
	//li.insert (listbox::index_pair ({ 1, 0 }), STR ("int10"));
	//li.insert (listbox::index_pair ({ 1, 0 }), STR ("int10"));
		//append ({ STR ("int2") });
	
	//li.events ().checked ([]{  });
	
	//cout << clock () << endl;
	//li.show ();
	//pic.show ();
	
	

	//astrocyte astro("F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\bm3d_2013-05-22_fileNo03_z-max.mat");
	//astro.preprocessing ();
	//astro.draw_morphology ("F:\\PC\\Document Files\\UNN\\3 course\\Coursework\\Data\\2015.04.28\\2013-05-22_fileNo03\\bm3d_2013-05-22_fileNo03_z-max_morphology.avi");
	//astro.calc_clusters (30, 5, 10, 0.5, 0.5);
	//cout << "start read file" << endl;
	//current_path (fs::path (matdir));
	
	/*
	path current_dir (matdir);
	boost::regex pattern (".*.mat");
	for (recursive_directory_iterator iter (current_dir), end; iter != end; ++iter) {
		std::string name = iter->path ().leaf ().string();
		if (regex_match (name, pattern)) {
			std::cout << name << "\n";
			clock_t be = clock ();
			matfilename = name;
			matfiledir = iter->path ().branch_path ().string();
			current_path (fs::path (matfiledir));
			int img_type, type_size;
			int success = read (iter->path ().string().c_str (), matvarname, pa, data, n, m, nt, img_type, type_size);
			cout << "read file: " << clock () - be << endl;
			if (success != 0) continue;			
			calc_max_thr (data, n, m, nt, img_type, type_size);
			mxDestroyArray (pa);
		}
	}*/
	/*
	matfiledir = matdir;
	clock_t be = clock ();
	video_data astrocyte;
	int success = read ((matfiledir + matfilename).c_str (), matvarname, pa, astrocyte);

	cout << "file read: " << clock () - be << endl;
	if (success != 0) return 1;

	current_path (fs::path (matdir "0"));

	// draw_morphology (astrocyte);

	// calculate graph
	video_data motion, astrocyte_color;
	preprocessing (astrocyte, astrocyte_color);
	calc_back_sub (astrocyte_color, motion);
	mxDestroyArray (pa);
	calc_clusters (astrocyte_color, motion, 30, 5, 10, 0.5, 0.5);
	*/
	
	//Mat a, b;
	
	/*
	IplImage* image = 0;
	IplImage* templ = 0;
	IplImage* dst = 0;
	// имя картинки задаётся первым параметром
	char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// получаем картинку
	image = cvLoadImage (filename, 1);

	printf ("[i] image: %s\n", filename);
	assert (image != 0);

	// шаблон
	char* filename2 = argc >= 3 ? argv[2] : "templ.bmp";
	printf ("[i] template: %s\n", filename2);

	templ = cvLoadImage (filename2, 1);
	assert (templ != 0);

	cvNamedWindow ("origianl", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("template", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("res", CV_WINDOW_AUTOSIZE);

	dst = cvCloneImage (templ);

	// размер шаблона
	int width = templ->width;
	int height = templ->height;

	// оригинал и шаблон
	cvShowImage ("origianl", image);
	cvShowImage ("template", templ);

	int x = 0;
	int y = 0;
	// задаём весовые коэффициенты
	double alpha = 0.5;
	double beta = 0.3;
	// устанавливаем область интереса
	cvSetImageROI (image, cvRect (x, y, width, height));
	// взвешенная сумма
	cvAddWeighted (image, alpha, templ, beta, 0.0, dst);
	// освобождаем область интереса
	

	// показываем результат
	cvShowImage ("res", dst);
	cvCopy (dst, image);
	cvResetImageROI (image);
	cvShowImage ("res", image);

	// ждём нажатия клавиши
	cvWaitKey (0);

	// освобождаем ресурсы
	cvReleaseImage (&image);
	cvReleaseImage (&templ);
	cvReleaseImage (&dst);
	cvDestroyAllWindows ();*/
	return 0;
}

