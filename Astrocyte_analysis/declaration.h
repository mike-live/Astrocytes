#ifndef DECLARATION_H
#define DECLARATION_H


#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp> 
#include "boost/regex.hpp"
//#include <opencv2/core/core.hpp>
#include <string>

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/filebox.hpp>

//#include <opencv2/opencv.hpp>

#include "config.h"
//#include "img_proc.h"
#include <ctime>
#include <iostream>

#include <windows.h>
#undef max
#undef min

namespace fs = boost::filesystem;

using namespace std;
using namespace cv;
using namespace nana;

#endif