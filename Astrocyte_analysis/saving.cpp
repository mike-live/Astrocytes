/*
#include <memory.h>
#include "auxiliaries.h"



void save_image (Mat image, string file_name, string description)
{
	imwrite (file_name, image);
}

template <typename T> void save_mat<T> (Mat * obj, int n, string file_name, string description)
{
	MATFile * file = matOpen (file_name.c_str (), "w");
	int ndim = n > 1 ? 3 : 2;
	mwSize * dims = new mwSize[ndim];
	dims[0] = n; dims[1] = obj->rows; dims[2] = obj->cols;
	mxArray * arr = mxCreateCharArray (ndim, dims);
	T * tmp = new T[dims[0] * dims[1] * dims[2]];
	for (int i = 0; i < n; i++) {
		memcpy (tmp + dims[1] * dims[2] * i, 
	}
	mxSetData (arr, (void *)obj->data);
	matPutVariable (file, "obj", arr);
	mxArray * descr = mxCreateCharArray (1, dims);
	matPutVariable (file, "description", );
	matClose (file);
}*/