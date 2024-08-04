#include <gtest/gtest.h>
#include <cuda.h>
#include <opencv2/opencv.hpp>

TEST(CudaTests, VersionCheck)
{
	EXPECT_EQ((int)(CUDA_VERSION / 10), 1205);
}

TEST(CudaTests, DeviceCheck)
{
	EXPECT_GE(cv::cuda::getCudaEnabledDeviceCount(), 1);
}


