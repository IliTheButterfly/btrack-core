#include <gtest/gtest.h>
#include "nodes/system/MemoryRegistry.h"

using namespace btrack::nodes::system;

TEST(RegistryTests, TestSyntax)
{
	auto ch1 = std::make_shared<Channel<int>>();
	auto ch2 = std::make_shared<Channel<const int>>();
	auto bc1 = std::make_shared<BroadcastChannel<int>>();

	bc1->addChannel(ch1);
	bc1->addChannel(ch2);

	// Can send by literal
	bc1->send(5);

	int x = 0;
	ch1->receive(x);
	GTEST_ASSERT_EQ(x, 5);

	int y = 0;
	ch2->receive(y);
	GTEST_ASSERT_EQ(y, 5);


	// Can send by variable
	int w = 10;
	bc1->send(w);

	ch1->receive(x);
	GTEST_ASSERT_EQ(x, 10);

	ch2->receive(y);
	GTEST_ASSERT_EQ(y, 10);
}

int count = 0;

struct TestObj
{
	int x = 0;
	TestObj() { ++count; }
	TestObj(const TestObj& o) { x = o.x; ++count; }
	TestObj(int v) : TestObj::TestObj() { x = v; };
};

TEST(RegistryTests, TestShared)
{
	count = 0;
	auto ch1 = std::make_shared<Channel<TestObj>>();
	auto ch2 = std::make_shared<Channel<const TestObj>>();
	auto bc1 = std::make_shared<BroadcastChannel<TestObj>>();

	bc1->addChannel(ch1);
	bc1->addChannel(ch2);

	auto x = ItemView<TestObj>::create(5);
	
	TestObj const* x_ptr = x.get();
	bc1->send(x);
	GTEST_ASSERT_EQ(x->x, 5);

	ItemValue<TestObj> y;
	ItemView<TestObj> z;
	ch1->receive(y);
	ch2->receive(z);
	GTEST_ASSERT_EQ(y->x, 5);
	GTEST_ASSERT_EQ(z->x, 5);
	GTEST_ASSERT_EQ(x_ptr, z.get());
	GTEST_ASSERT_NE(x_ptr, y.get());
	GTEST_ASSERT_EQ(count, 2);
}

TEST(RegistryTests, TestUnique)
{
	count = 0;
	auto ch1 = std::make_shared<Channel<TestObj>>();
	auto ch2 = std::make_shared<Channel<const TestObj>>();
	auto bc1 = std::make_shared<BroadcastChannel<TestObj>>();

	bc1->addChannel(ch1);
	bc1->addChannel(ch2);

	auto x = ItemValue<TestObj>::create(5);

	// Build should fail if the next line is uncommented
	// auto xcpbad = ItemView<TestObj>::create(x);

	ItemView<TestObj> xcp;
	xcp.copyFrom(x);
	
	TestObj const* x_ptr = xcp.get();
	bc1->send(xcp);
	GTEST_ASSERT_EQ(x->x, 5);

	ItemValue<TestObj> y;
	ItemView<TestObj> z;
	ch1->receive(y);
	ch2->receive(z);
	GTEST_ASSERT_EQ(y->x, 5);
	GTEST_ASSERT_EQ(z->x, 5);
	GTEST_ASSERT_EQ(x_ptr, z.get());
	GTEST_ASSERT_NE(x_ptr, y.get());
	GTEST_ASSERT_EQ(count, 3);
}


// TEST(RegistryTests, FundamentalConversion)
// {
// 	auto ch1 = std::make_shared<Channel<float>>();
// 	auto ch2 = std::make_shared<Channel<const float>>();
// 	auto bc1 = std::make_shared<BroadcastChannel<int>>();
// 	auto bc2 = std::make_shared<BroadcastChannel<float>>();

// 	bc1->addChannel(bc2);

// 	bc2->addChannel(ch1);
// 	bc2->addChannel(ch2);

// 	auto x = ItemValue<TestObj>::create(5);
// }
