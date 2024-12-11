#include <gtest/gtest.h>
#include "nodes/MemoryRegistry.h"

using namespace btrack::nodes;

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
	ASSERT_EQ(x, 5);

	int y = 0;
	ch2->receive(y);
	ASSERT_EQ(y, 5);


	// Can send by variable
	int w = 10;
	bc1->send(w);

	ch1->receive(x);
	ASSERT_EQ(x, 10);

	ch2->receive(y);
	ASSERT_EQ(y, 10);
}

struct TestObj
{
	int x;
};

// TEST(RegistryTests, TestShared)
// {
// 	// auto ch1 = std::make_shared<Channel<TestObj>>();
// 	auto ch2 = std::make_shared<Channel<const TestObj>>();
// 	auto bc1 = std::make_shared<BroadcastChannel<TestObj>>();

// 	// bc1->addChannel(ch1);
// 	bc1->addChannel(ch2);

// 	auto x = std::make_shared<TestObj>(5);
	
// 	TestObj const* x_ptr = x.get();
// 	bc1->send(x);

// 	std::unique_ptr<TestObj> y;
// 	std::shared_ptr<const TestObj> z;
// 	// ch1->receive(y);
// 	ch2->receive(z);
// 	ASSERT_EQ(x->x, z->x);
// }
