///////////////////////////////////////////////////////////////////////////////
// This file contains exported functions from ocvgpumex 
///////////////////////////////////////////////////////////////////////////////

#ifndef _OCVGPUMEX_
#define _OCVGPUMEX_

#ifndef LIBMWOCVGPUMEX_API
#    define LIBMWOCVGPUMEX_API
#endif

// no c linkage
#include "mex.h"
#include "tmwtypes.h"
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"

///////////////////////////////////////////////////////////////////////////////
// ocvMxGpuArrayToGpuMat:
//   Utility function to create cv::::gpu::GpuMat from a gpuArray. 
//
// Note: GpuMat only support 2D arrays. 
///////////////////////////////////////////////////////////////////////////////
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_double(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_single(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint8(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint16(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_uint32(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int8(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int16(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_int32(const mxArray * in);
extern LIBMWOCVGPUMEX_API cv::Ptr<cv::gpu::GpuMat> ocvMxGpuArrayToGpuMat_bool(const mxArray * in);

///////////////////////////////////////////////////////////////////////////////
// ocvMxGpuArrayFromGpuMat:
//   Utility function to create gpuArray from a cv::gpu::GpuMat.
//
// Note: GpuMat only supports 2D arrays.
///////////////////////////////////////////////////////////////////////////////

extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_double(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_single(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint8(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint16(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_uint32(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int8(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int16(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_int32(const cv::gpu::GpuMat & in);
extern LIBMWOCVGPUMEX_API mxArray * ocvMxGpuArrayFromGpuMat_bool(const cv::gpu::GpuMat & in);

#endif
