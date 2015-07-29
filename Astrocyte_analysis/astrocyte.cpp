#include "astrocyte.h"

astrocyte::astrocyte (fs::path file_name_, form & main_form, std::string var_name) : file_name (file_name_), my_form (main_form)
{
	my_form.status (L"File reading...");
	MATFile * pmat;
	const char ** dir;
	const char * name;
	int	ndir;
	// Open file to get directory
	pmat = matOpen (file_name.string().c_str(), "r");
	if (pmat == NULL) {
		cout << "Error opening file " << file_name << endl;
		my_form.status (L"Error opening file " + file_name.wstring());
		return ;
	}

	// get directory of MAT-file
	dir = (const char **)matGetDir (pmat, &ndir);
	if (dir == NULL) {
		cout << "Error reading directory of file" << file_name << endl;
		my_form.status (L"Error reading directory of file " + file_name.wstring ());
		return ;
	}
	mxFree (dir);

	// In order to use matGetNextXXX correctly, reopen file to read in headers.
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (L"Error closing file " + file_name.wstring ());
		return ;
	}
	pmat = matOpen (file_name.string ().c_str(), "r");
	if (pmat == NULL) {
		cout << "Error reopening file " << file_name << endl;
		my_form.status (L"Error reopening file " + file_name.wstring ());
		return ;
	}

	// Get headers of all variables
	for (int i = 0; i < ndir; i++) {
		pa = matGetNextVariableInfo (pmat, &name);
		if (pa == NULL) {
			cout << "Error reading in file " << file_name << endl;
			my_form.status (L"Error reading in file " + file_name.wstring ());
			return ;
		}
		mxDestroyArray (pa);
	}

	// Reopen file to read in actual arrays.
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (L"Error closing file " + file_name.wstring ());
		return ;
	}
	pmat = matOpen (file_name.string ().c_str(), "r");
	if (pmat == NULL) {
		cout << "Error reopening file " << file_name << endl;
		my_form.status (L"Error reopening file " + file_name.wstring ());
		return ;
	}

	// Read in each array. 
	for (int i = 0; i < ndir; i++) {
		pa = matGetNextVariable (pmat, &name);
		if (pa == NULL) {
			cout << "Error reading in file " << file_name << endl;
			my_form.status (L"Error reading in file " + file_name.wstring ());
			return ;
		}
		int img_type;
		if (mxIsDouble (pa)) img_type = CV_64F;
		if (mxIsUint16 (pa)) img_type = CV_16UC1;
		if (mxIsSingle (pa)) img_type = CV_32F;
		int num = (int)mxGetNumberOfDimensions (pa);
		const mwSize *sz = mxGetDimensions (pa);
		if (std::string (name) == var_name || num == 3 || (num == 4 && sz[2] == 1)) {
			data.reset ((uchar *)mxGetData (pa), (int)sz[1], (int)sz[0], (int)((num == 4 && sz[2] == 1) ? sz[3] : sz[2]), img_type, false);
			break;
		}
	}
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (L"Error closing file " + file_name.wstring ());
		return ;
	}
	my_form.status (L"File read success!");
	return ;
}

Mat astrocyte::max_thr (bool gauss)
{
	my_form.status (L"Maximum + threshold otsu calculating...");
	if (thr_max.empty()) {
		Mat img_max, img_gauss_max; // intensity.n, intensity.m, intensity.type_size, 0
		for (int t = 0; t < intensity.nt; t++) {
			Mat img = intensity.image(t);
			if (img_max.empty ()) img_max = img;
			else img_max = max (img_max, img);
			GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
			if (img_gauss_max.empty ()) img_gauss_max = img;
			else img_gauss_max = max (img_gauss_max, img);
		}
		threshold (img_max, thr_max, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
		threshold (img_gauss_max, thr_gauss_max, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	}
	return gauss ? thr_gauss_max : thr_max;
}

Mat astrocyte::min_thr (bool gauss)
{
	my_form.status (L"Minimum + threshold otsu calculating...");
	if (thr_min.empty ()) {
		Mat img_min, img_gauss_min; // intensity.n, intensity.m, intensity.type_size, 0
		for (int t = 0; t < intensity.nt; t++) {
			Mat img = intensity.image (t);
			if (img_min.empty ()) img_min = img;
			else img_min = min (img_min, img);
			GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
			if (img_gauss_min.empty ()) img_gauss_min = img;
			else img_gauss_min = min (img_gauss_min, img);
		}
		threshold (img_min, thr_min, 25, 255, THRESH_BINARY);
		threshold (img_gauss_min, thr_gauss_min, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	}
	return gauss ? thr_gauss_min : thr_min;
}


void astrocyte::preprocessing ()
{
	intensity.reset (new uchar[data.size], data.n, data.m, data.nt, CV_8U);
	for (int t = 0; t < data.nt; t++) {
		Mat img = data.image(t);
		img = min (img, 400);
		double min_val, max_val;
		minMaxLoc (img, &min_val, &max_val);
		img.convertTo (img, CV_8U, 255.0 / (max_val - min_val), -min_val * 255.0 / (max_val - min_val));
		memcpy (intensity.frame (t), img.data, data.nm);
	}
	intensity.set_nt (100);
} 


void astrocyte::background_subtraction ()
{
	const int shift = 3;
	//printf ("background_subtraction begin\n");
	max_thr ();
	min_thr (false);

	my_form.status (L"Background subtraction calculating...");
	BackgroundSubtractorMOG2 bg_model;
	Mat fgimg, thr_inv, img_black = intensity.image(0);
	img_black.setTo (0);
	motion.reset (new uchar[intensity.size], intensity.n, intensity.m, intensity.nt, CV_8U);
	bitwise_not (thr_min, thr_inv);
	//imshow ("Inversion", thr_inv);
	//imshow ("Max", thr_max);
	for (int t = 0; t < intensity.nt; t++) {
		Mat img = intensity.image(t), imgc_max, fgmask, img_thr, img_thr_inv;

		applyColorMap (img, imgc_max, COLORMAP_JET);
		imgc_max.copyTo (img_thr, thr_max);		
		//imshow ("Thresholds", img_thr);
		//waitKey ();
		img_thr.copyTo (img_thr_inv, thr_inv);
		//imshow ("Thresholds + inv", img_thr_inv);
		
		bg_model (img_thr_inv, fgmask);

		if (fgimg.empty ()) fgimg.create (img.size (), img.type ());
		fgimg = Scalar::all (0);
		img.copyTo (fgimg, fgmask);

		Mat bgimg;
		bg_model.getBackgroundImage (bgimg);
		//imshow ("bgimg", bgimg);
		GaussianBlur (fgimg, fgimg, cv::Size (7, 7), 1.0, 1.0);
		//imshow ("fgimg", fgimg);
		//waitKey ();

		if (t >= shift)	memcpy (motion.frame(t), fgimg.data, motion.nm);
		else memcpy (motion.frame (t), img_black.data, motion.nm);
	}
	my_form.status (L"Background subtraction calculated");
	//printf ("background_subtraction end\n");	
}


//Mat img_conc, img_color;

//cvtColor (fgimg, img_color, CV_GRAY2BGR);
//conc (img_thr, img_color, img_conc);
//imshow ("asdf", img_conc);
//imwrite (path + "\\" + std::to_string (i) + ".bmp", img_conc);
//waitKey ();
