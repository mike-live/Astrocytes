//////////////////////////////////////////////////////////////////////////////
// This file contains a set of utilities for marshalling data between 
// OpenCV and MATLAB. It supports only CPP-linkage.
//
// Copyright 2014 The MathWorks, Inc.
//  
//////////////////////////////////////////////////////////////////////////////

#ifndef _OCVMEX_
#define _OCVMEX_

#ifndef LIBMWOCVMEX_API
#    define LIBMWOCVMEX_API
#endif

#ifdef MATLAB_MEX_FILE
#include "tmwtypes.h"
#else
#include "rtwtypes.h"
#endif

// no c linkage
#include "mex.h"
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"

/////////////////////////////////////////////////////////////////////////////////
// ocvCheckFeaturePointsStruct:
//  Key point struct checker
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API void ocvCheckFeaturePointsStruct(const mxArray *in);

//////////////////////////////////////////////////////////////////////////////
// ocvStructToKeyPoints:
//  Converts MATLAB's struct to OpenCV's KeyPoint
//////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API void ocvStructToKeyPoints(const mxArray * in, 
                       cv::vector<cv::KeyPoint> &keypoints);

//////////////////////////////////////////////////////////////////////////////
// ocvKeyPointsToStruct:
//  Converts OpenCV's KeyPoint to MATLAB's struct
//////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray *ocvKeyPointsToStruct(cv::vector<cv::KeyPoint> &in);

//////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToCvRect:
//  Converts from mxArray to the OpenCV CvRect structure
//////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API CvRect    ocvMxArrayToCvRect(const mxArray *in);

//////////////////////////////////////////////////////////////////////////////
// ocvCvRectToMxArray:
//  Converts from the OpenCV CvRect structure to mxArray 
//////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray  *ocvCvRectToMxArray(const CvRect *in);

//////////////////////////////////////////////////////////////////////////////
// ocvCvBox2DToMxArray:
//  Converts from the OpenCV CvBox2D structure to mxArray
//  NOTE:
//  in a CvBox2D structure, x and y represent the coordinates of the center 
//  of the rectangle and NOT the upper left corner of the rectangle as in
//  the CvRect structure or as in the MATLAB RECTANGLE function
//////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray  *ocvCvBox2DToMxArray(const CvBox2D *in);

///////////////////////////////////////////////////////////////////////////////
// cvRectToBoundingBox_T:
//  Utility to convert vector<cv::Rect> to M-by-4 mxArray of bounding boxes
///////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_double(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_single(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_uint8(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_uint16(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_uint32(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_int8(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_int16(const std::vector<cv::Rect> & rects);
extern LIBMWOCVMEX_API mxArray * ocvCvRectToBoundingBox_int32(const std::vector<cv::Rect> & rects);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToSize:
//  Utility function to convert 2-element mxArray to cv::Size.
//      IN 1: mxArray to convert 
//      IN 2: boolean flag indicates if input mxArray is [r c] or [x y].
//            Default is [r c] (i.e. [height width])
//
//      Empty input ([]) will return cv::Size(0,0);
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API cv::Size ocvMxArrayToSize(const mxArray * in, bool rcInput = true);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToImage_T:
//  Utility function to fill a given cv::Mat with the data from a given mxArray. 
//  Transpose and interleave column major mxArray data into row major cv::Mat. 
//  Supports 2D and 3D images.  Reallocates memory for the cv::Mat if needed.
//  NOTE: This is not a generic matrix conversion routine!  For 3D images, it
//        takes into account that the OpenCV format uses BGR ordering and thus
//        it manipulates the data to be compliant with that formatting.
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API void ocvMxArrayToImage_double(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_single(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_uint8(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_uint16(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_uint32(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_int8(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_int16(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_int32(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToImage_bool(const mxArray *in, cv::Mat &out);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToImage_T:
//  Utility function to convert mxArray to cv::Mat (for images) 
//
//  By default, transposes and interleaves (in the case of RGB images) column
//  major mxArray data into row major cv::Mat.  Supports 2D and 3D images.
//  Returns a smart pointer (cv::Ptr) to a cv::Mat object. 
//
//  Optionally, the copyData flag may be set to false, in which case no data is
//  copied from the mxArray to the Mat, instead, a new Mat wrapper is created,
//  and used to point to the mxArray data. Because OpenCV is row based and
//  MATLAB is column based, the columns of the mxArray become the rows of the
//  Mat. The copyData flag can only be false if the image is 2D.
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_double(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_single(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_uint8(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_uint16(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_uint32(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_int8(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_int16(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_int32(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToImage_bool(const mxArray *in, const bool copyData = true);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToMat_T:
//  Utility function to fill a given cv::Mat with the data from a given mxArray. 
//  Transpose and interleave column major mxArray data into row major cv::Mat. 
//  Supports n-channel matrices.  Reallocates memory for the cv::Mat if needed.
//  NOTE: This is a generic matrix conversion routine for any number of channels.
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API void ocvMxArrayToMat_double(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_single(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_uint8(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_uint16(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_uint32(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_int8(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_int16(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_int32(const mxArray *in, cv::Mat &out);
extern LIBMWOCVMEX_API void ocvMxArrayToMat_bool(const mxArray *in, cv::Mat &out);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayToMat_T:
//  Utility function to convert mxArray to cv::Mat 
//
//  By default, transposes and interleaves (in the case of RGB images) column
//  major mxArray data into row major cv::Mat.  Supports n-channel matrices.
//  Returns a smart pointer (cv::Ptr) to a cv::Mat object. 
//
//  Optionally, the copyData flag may be set to false, in which case no data is
//  copied from the mxArray to the Mat, instead, a new Mat wrapper is created,
//  and used to point to the mxArray data. Because OpenCV is row based and
//  MATLAB is column based, the columns of the mxArray become the rows of the
//  Mat. The copyData flag can only be false if the matrix is 2D.
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_double(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_single(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_uint8(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_uint16(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_uint32(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_int8(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_int16(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_int32(const mxArray *in, const bool copyData = true);
extern LIBMWOCVMEX_API cv::Ptr<cv::Mat> ocvMxArrayToMat_bool(const mxArray *in, const bool copyData = true);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayFromImage_T:
//  Utility function to create an mxArray from cv::Mat.
//  NOTE: This is not a generic matrix conversion routine!  For 3D images, it
//        takes into account that the OpenCV format uses BGR ordering and thus
//        it manipulates the data to be compliant with that formatting.
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_double(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_single(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_uint8(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_uint16(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_uint32(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_int8(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_int16(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_int32(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromImage_bool(const cv::Mat &in);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayFromMat_T:
//  Utility function to create an mxArray from cv::Mat.
//  NOTE: This is a generic matrix conversion routine for any number of channels
//        It does not take into account the BGR ordering
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_double(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_single(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_uint8(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_uint16(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_uint32(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_int8(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_int16(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_int32(const cv::Mat &in);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromMat_bool(const cv::Mat &in);

/////////////////////////////////////////////////////////////////////////////////
// ocvMxArrayFromVector:
//  Conversion from numeric vector<T> to mxArray
/////////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<real_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<real32_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<uint8_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<uint16_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<uint32_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<int8_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<int16_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<int32_T> &v);
extern LIBMWOCVMEX_API mxArray *ocvMxArrayFromVector(const std::vector<boolean_T> &v);

///////////////////////////////////////////////////////////////////////////////
// ocvMxArrayFromPoints2f:
//  Converts vector<Points2f> to mxArray 
//  The mxArray has to be managed by the caller
///////////////////////////////////////////////////////////////////////////////
extern mxArray *ocvMxArrayFromPoints2f(const std::vector<cv::Point2f> &points);


///////////////////////////////////////////////////////////////////////////////
// ocvMxGpuArrayToGpuMat:
//   Utility function to create cv::::gpu::GpuMat from a gpuArray. 
//
// Note: GpuMat only support 2D arrays. 
///////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_double(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_single(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint8(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint16(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint32(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int8(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int16(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int32(const mxArray * in);
extern LIBMWOCVMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_bool(const mxArray * in);

///////////////////////////////////////////////////////////////////////////////
// ocvMxGpuArrayFromGpuMat:
//   Utility function to create gpuArray from a cv::gpu::GpuMat.
//
// Note: GpuMat only supports 2D arrays.
///////////////////////////////////////////////////////////////////////////////

extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_double(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_single(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint8(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint16(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint32(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int8(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int16(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int32(const cv::gpu::GpuMat & in);
extern LIBMWOCVMEX_API mxArray * ocvMxGpuArrayFromGpuMat_bool(const cv::gpu::GpuMat & in);

#endif
