#pragma once

#include <opencv2/opencv.hpp>

#ifdef USE_CUDA

#include <opencv2/cudaimgproc.hpp>

#define proc cv::cuda
using procMat = cv::cuda::GpuMat;
#define procDownload(img, procImg) procImg.download(img)
#define procUpload(img, procImg) procImg.upload(img)

#else

#include <opencv2/imgproc.hpp>
#define proc cv
using procMat = cv::Mat;
#define procDownload(img, procImg) img = procImg
#define procUpload(img, procImg) procImg = img


#endif // USE_CUDA

