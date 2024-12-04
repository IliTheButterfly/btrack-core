#include <gtest/gtest.h>
#include "nodes/MemoryRegistry.h"

using namespace btrack::nodes;

TEST(RegistryTests, FundamentalValue)
{
	FundamentalSimpleRegister<int> r1;
	FundamentalSimpleRegister<int> r2;


	r1.push(10);
	r1>>r2;

	ASSERT_EQ(r2.pull(), 10);
}
