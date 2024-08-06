#include <gtest/gtest.h>
#include <processing/wrapper.h>

TEST(WrapperTests, CvtTest)
{
	std::cout << TEST_JPG_LOCATION << std::endl;
	cv::Mat img = cv::imread(TEST_JPG_LOCATION);
	procMat src, dst;
	procUpload(img, src);
	
	proc::cvtColor(src, dst, cv::COLOR_BGR2RGB);

	cv::Mat res;
	procDownload(res, dst);
}

