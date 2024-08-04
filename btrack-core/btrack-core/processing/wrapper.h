#pragma once

#include <opencv2/opencv.hpp>

#ifdef USE_CUDA

#include <opencv2/cudaimgproc.hpp>

#define proc cv::cuda

#else

#include <opencv2/imgproc.hpp>
#define proc cv

#endif // USE_CUDA

