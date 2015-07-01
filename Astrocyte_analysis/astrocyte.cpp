#include "astrocyte.h"

astrocyte::astrocyte (fs::path file_name_, form & main_form, std::string var_name) : file_name (file_name_), my_form (main_form)
{
	my_form.status (STR ("File reading..."));
	MATFile * pmat;
	const char ** dir;
	const char * name;
	int	ndir;
	// Open file to get directory
	pmat = matOpen (file_name.string().c_str(), "r");
	if (pmat == NULL) {
		cout << "Error opening file " << file_name << endl;
		my_form.status (STR ("Error opening file " + file_name.wstring()));
		return ;
	}

	// get directory of MAT-file
	dir = (const char **)matGetDir (pmat, &ndir);
	if (dir == NULL) {
		cout << "Error reading directory of file" << file_name << endl;
		my_form.status (STR ("Error reading directory of file " + file_name.wstring ()));
		return ;
	}
	mxFree (dir);

	// In order to use matGetNextXXX correctly, reopen file to read in headers.
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (STR ("Error closing file " + file_name.wstring ()));
		return ;
	}
	pmat = matOpen (file_name.string ().c_str (), "r");
	if (pmat == NULL) {
		cout << "Error reopening file " << file_name << endl;
		my_form.status (STR ("Error reopening file " + file_name.wstring ()));
		return ;
	}

	// Get headers of all variables
	for (int i = 0; i < ndir; i++) {
		pa = matGetNextVariableInfo (pmat, &name);
		if (pa == NULL) {
			cout << "Error reading in file " << file_name << endl;
			my_form.status (STR ("Error reading in file " + file_name.wstring ()));
			return ;
		}
		mxDestroyArray (pa);
	}

	// Reopen file to read in actual arrays.
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (STR ("Error closing file " + file_name.wstring ()));
		return ;
	}
	pmat = matOpen (file_name.string ().c_str (), "r");
	if (pmat == NULL) {
		cout << "Error reopening file " << file_name << endl;
		my_form.status (STR ("Error reopening file " + file_name.wstring ()));
		return ;
	}

	// Read in each array. 
	for (int i = 0; i < ndir; i++) {
		pa = matGetNextVariable (pmat, &name);
		if (pa == NULL) {
			cout << "Error reading in file " << file_name << endl;
			my_form.status (STR ("Error reading in file " + file_name.wstring ()));
			return ;
		}
		int img_type;
		if (mxIsDouble (pa)) img_type = CV_64F;
		if (mxIsUint16 (pa)) img_type = CV_16UC1;
		if (mxIsSingle (pa)) img_type = CV_32F;
		int num = (int)mxGetNumberOfDimensions (pa);
		const mwSize *sz = mxGetDimensions (pa);
		if (std::string (name) == var_name || num == 3 || (num == 4 && sz[2] == 1)) {
			data = video_data ((uchar *)mxGetData (pa), (int)sz[1], (int)sz[0], (int)((num == 4 && sz[2] == 1) ? sz[3] : sz[2]), img_type);
			break;
		}
	}
	if (matClose (pmat) != 0) {
		cout << "Error closing file " << file_name << endl;
		my_form.status (STR ("Error closing file " + file_name.wstring ()));
		return ;
	}
	my_form.status (STR ("File read success!"));
	return ;
}

Mat astrocyte::max_thr (bool gauss)
{
	my_form.status (STR ("Maximum threshold otsu calculating..."));
	if (thr.empty()) {
		Mat img_max, img_gauss_max; // intensity.n, intensity.m, intensity.type_size, 0
		for (int t = 0; t < intensity.nt; t++) {
			Mat img = intensity.get_mat(t);
			if (img_max.empty ()) img_max = img;
			else img_max = max (img_max, img);
			GaussianBlur (img, img, cv::Size (3, 3), 1.0, 1.0);
			if (img_gauss_max.empty ()) img_gauss_max = img;
			else img_gauss_max = max (img_gauss_max, img);
		}
		threshold (img_max, thr, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
		threshold (img_gauss_max, thr_gauss, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	}
	return gauss ? thr_gauss : thr;
}


void astrocyte::preprocessing ()
{
	intensity = video_data (new uchar[data.size], data.n, data.m, data.nt, CV_8U);
	for (int t = 0; t < data.nt; t++) {
		Mat img (data.n, data.m, data.img_type, data.get_image (t));
		img = min (img, 400);
		double min_val, max_val;
		minMaxLoc (img, &min_val, &max_val);
		img.convertTo (img, CV_8U, 255.0 / (max_val - min_val), -min_val * 255.0 / (max_val - min_val));
		memcpy (intensity.get_image (t), img.data, data.nm);
	}
}


void astrocyte::background_subtraction ()
{
	//printf ("background_subtraction begin\n");
	max_thr ();

	my_form.status (STR ("Background subtraction calculating..."));
	BackgroundSubtractorMOG bg_model;
	Mat fgimg;
	motion = video_data (new uchar[intensity.size], intensity.n, intensity.m, intensity.nt, CV_8U);
	for (int t = 0; t < intensity.nt; t++) {
		Mat img = intensity.get_mat(t), imgc_max, fgmask, img_thr;

		applyColorMap (img, imgc_max, COLORMAP_JET);
		imgc_max.copyTo (img_thr, thr);
		bg_model (img_thr, fgmask);

		if (fgimg.empty ()) fgimg.create (img.size (), img.type ());
		fgimg = Scalar::all (0);
		img.copyTo (fgimg, fgmask);

		Mat bgimg;
		bg_model.getBackgroundImage (bgimg);
		GaussianBlur (fgimg, fgimg, cv::Size (7, 7), 1.0, 1.0);
		
		memcpy (motion.get_image(t), fgimg.data, motion.nm);
	}
	my_form.status (STR ("Background subtraction calculated"));
	//printf ("background_subtraction end\n");	
}


//Mat img_conc, img_color;

//cvtColor (fgimg, img_color, CV_GRAY2BGR);
//conc (img_thr, img_color, img_conc);
//imshow ("asdf", img_conc);
//imwrite (path + "\\" + std::to_string (i) + ".bmp", img_conc);
//waitKey ();
