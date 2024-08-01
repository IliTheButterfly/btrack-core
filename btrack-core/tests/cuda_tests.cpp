#include <gtest/gtest.h>
#include <cuda.h>

TEST(CudaTests, VersionCheck)
{
	EXPECT_EQ(CUDA_VERSION, 12050);
}

