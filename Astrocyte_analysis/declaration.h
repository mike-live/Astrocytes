#pragma once

#include <opencv2/opencv.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp> 
#include "boost/regex.hpp"

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/filebox.hpp>

#include "config.h"

#include <ctime>
#include <iostream>
#include <windows.h>
#include <string>

#undef max
#undef min

namespace fs = boost::filesystem;

using namespace std;
using namespace cv;
using namespace nana;
