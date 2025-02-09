#include <gtest/gtest.h>
#include "test_nodes.h"
#include "nodes/utilities/math/Negate.h"
#include <atomic>
#include <boost/chrono.hpp>

/**
 * @brief Create a chain of Meta Nodes and link their input and outputs
 * 
 * This will create a graph like so:
 * 
 *   Start         Mid            End
 *      V *------* V  V *--------* V 
 *                                   
 *      A *------* A  A *--------* A 
 */
TEST(NodeTests, SimpleChain)
{
	auto start = TestNodeStart::create(dummy);
	auto mid = TestNodeMid1::create(dummy);
	auto end = TestNodeEnd::create(dummy);

	(*start->valueOutput)>>mid->valueInput;
	(*start->arrayOutput)>>mid->arrayInput;

	(*mid->valueOutput)>>end->valueInput;
	(*mid->arrayOutput)>>end->arrayInput;
}

/**
 * @brief Create a chain of Meta Nborrowed_ptr_podes and link their input and outputs. 
 * 
 * Value outputs can feed into array or value inputs, array outputs can only feed into array inputs
 * This will create a graph like so:
 * 
 *   Start        End
 *      V *--    * V  
 *           \        
 *      A *   ---* A  
 */
TEST(NodeTests, ValueToArray)
{
	auto start = TestNodeStart::create(dummy);
	auto end = TestNodeEnd::create(dummy);

	(*start->valueOutput)>>end->arrayInput;
}

/**
 * @brief Create a chain of Meta Nodes and link their input and outputs
 * 
 * This will create a graph like so:
 * 
 *   Start         Mid            End
 *      V *------* V  V *--------* V 
 *                                   
 *      A *------* A  A *--------* A 
 */
TEST(NodeTests, OtherChain)
{
	auto start = TestNodeStart::create(dummy);
	auto mid = TestNodeMid1::create(dummy);
	auto end = TestNodeEnd::create(dummy);

	(*start->valueOutput)>>mid->valueInput;
	(*start->arrayOutput)>>mid->arrayInput;

	(*mid->valueOutput)>>end->valueInput;
	(*mid->arrayOutput)>>end->arrayInput;
}


struct Base
{
    virtual ~Base() = default;

    virtual int id() const { return 0; }
};

struct Derived : public Base
{
    int id() const override { return 1; }
};

TEST(RegistryTests, TestIters)
{
	// Container of shared_ptr<Base>
    std::vector<std::shared_ptr<Base>> vec = {
        std::make_shared<Base>(),
        std::make_shared<Derived>(),
        std::make_shared<Base>()
    };

	int expected[] = {0, 1, 0};

}

boost::condition_variable done;
boost::mutex mtx;
std::atomic_bool doneVar = false;
void run()
{
	boost::system_time const timeout = boost::get_system_time()+ boost::posix_time::seconds(5);
	doneVar = false;
	boost::unique_lock lock(mtx);
	done.timed_wait(lock, timeout);
	if (doneVar) exit(EXIT_SUCCESS);
	else exit(EXIT_SUCCESS);

}

TEST(NodeUtilitiesMathTests, NegateOperation)
{
	// auto t = boost::thread(run);
	// auto t1 = UnaryNodeTester<Negate<int>, const int, const int>(5, -5);
	auto n = Negate<int, DefaultChannelTypeInfo<int>>::create(dummy, "UnaryTest");
	std::shared_ptr<OutputValue<int>> parameter = OutputValue<int>::create(dummy, dummy, "parameter");
	std::shared_ptr<InputValue<int>> result = InputValue<int>::create(dummy, dummy, "result");

	std::cout << "Connecting" << std::endl;
	(*parameter) >> n->Parameter;
	*(n->Result) >> result;
	std::cout << "Passing value" << std::endl;
	(*parameter) << 5;
	int res;
	std::cout << "Processing" << std::endl;
	n->process();
	std::cout << "Receiving" << std::endl;
	*result >> res;
	GTEST_ASSERT_EQ(-5, res);
	doneVar = true;
	done.notify_one();
}

