#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <thread>

TEST(OpencvTests, VersionCheck)
{
	EXPECT_EQ(CV_VERSION_MAJOR, 4);
	EXPECT_EQ(CV_VERSION_MINOR, 10);
}

TEST(OpencvTests, BuildInfo)
{
	std::cout << cv::getBuildInformation() << std::endl;
}

TEST(OpencvTests, CvtTest)
{
	std::cout << TEST_JPG_LOCATION << std::endl;
	cv::Mat img = cv::imread(TEST_JPG_LOCATION);
	cv::Mat res;
	cv::cvtColor(img, res, cv::COLOR_BGR2RGB);
}

