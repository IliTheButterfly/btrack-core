#include <gtest/gtest.h>
#include "nodes/system/nodes.h"
#include "nodes/utilities/math/UnaryOperation.h"
#include "nodes/system/shortcuts.h"
#include "nodes/utilities/math/Negate.h"
#include <atomic>
#include <boost/chrono.hpp>

using namespace btrack::nodes::system;
using namespace btrack::nodes::utilities::math;

class NodeStart : public Node
{
public:
	std::shared_ptr<OutputValue<int>> valueOutput;
	std::shared_ptr<OutputValue<int>> arrayOutput;

	NodeStart() : NodeStart::Node("nodeStart")
	{
		valueOutput = addOutputValue<int>("valueOutput");
		arrayOutput = addOutputValue<int>("arrayOutput");
	}

	void process() override
	{
		
	}
};

class TestNodeStart : public MetaNode
{
public:
	std::shared_ptr<MetaOutputValue<int>> valueOutput;
	std::shared_ptr<MetaOutputArray<int>> arrayOutput;
	TestNodeStart(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeStart::MetaNode(_name, _friendlyName, _description) 
			{
				valueOutput = addOutputValue<int>("valueOutput");
				arrayOutput = addOutputArray<int>("arrayOutput");
			}

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class NodeStartFloat : public Node
{
public:
	std::shared_ptr<OutputValue<float>> valueOutput;
	std::shared_ptr<OutputValue<float>> arrayOutput;

	NodeStartFloat() : NodeStartFloat::Node("nodeStartFloat")
	{
		valueOutput = addOutputValue<float>("valueOutput");
		arrayOutput = addOutputValue<float>("arrayOutput");
	}

	void process() override
	{
		
	}
};

class TestNodeStartFloat : public MetaNode
{
public:
	std::shared_ptr<MetaOutputValue<float>> valueOutput;
	std::shared_ptr<MetaOutputArray<float>> arrayOutput;
	TestNodeStartFloat(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeStartFloat::MetaNode(_name, _friendlyName, _description) 
			{
				valueOutput = addOutputValue<float>("valueOutput");
				arrayOutput = addOutputArray<float>("arrayOutput");
			}

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class TestNodeMid1 : public MetaNode
{
public:
	std::shared_ptr<MetaInputValue<int>> valueInput;
	std::shared_ptr<MetaInputArray<int>> arrayInput;
	
	std::shared_ptr<MetaOutputValue<int>> valueOutput;
	std::shared_ptr<MetaOutputArray<int>> arrayOutput;

	TestNodeMid1(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeMid1::MetaNode(_name, _friendlyName, _description) 
			{
				valueInput = addInputValue<int>("valueInput");
				arrayInput = addInputArray<int>("arrayInput");
				valueOutput = addOutputValue<int>("valueOutput");
				arrayOutput = addOutputArray<int>("arrayOutput");
			}

	void generate(int count) override {}

	size_t inputCount() const override { return 2; }
	size_t outputCount() const override { return 2; }
};

class TestNodeEnd : public MetaNode
{
public:
	TestNodeEnd(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeEnd::MetaNode(_name, _friendlyName, _description) 
			{
				valueInput = addInputValue<int>("valueInput");
				arrayInput = addInputArray<int>("arrayInput");
			}
	std::shared_ptr<MetaInputValue<int>> valueInput;
	std::shared_ptr<MetaInputArray<int>> arrayInput;

	void generate(int count) override {}

	size_t inputCount() const override { return 2; }
	size_t outputCount() const override { return 0; }
};

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
	auto start = TestNodeStart("start", "Start");
	auto mid = TestNodeMid1("mid", "Middle");
	auto end = TestNodeEnd("end", "End");

	(*start.valueOutput)>>mid.valueInput;
	(*start.arrayOutput)>>mid.arrayInput;

	(*mid.valueOutput)>>end.valueInput;
	(*mid.arrayOutput)>>end.arrayInput;
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
	auto start = TestNodeStart("start", "Start");
	auto end = TestNodeEnd("end", "End");

	(*start.valueOutput)>>end.arrayInput;
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
	auto start = TestNodeStart("start", "Start");
	auto mid = TestNodeMid1("mid", "Middle");
	auto end = TestNodeEnd("end", "End");

	(*start.valueOutput)>>mid.valueInput;
	(*start.arrayOutput)>>mid.arrayInput;

	(*mid.valueOutput)>>end.valueInput;
	(*mid.arrayOutput)>>end.arrayInput;
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

// TEST(NodeTests, TypeConversionChain)
// {
// 	auto start = TestNodeStartFloat("start", "Start");
// 	auto mid = TestNodeMid1("mid", "Middle");

// 	(*start.valueOutput)>>mid.valueInput;
// 	(*start.arrayOutput)>>mid.arrayInput;
// }

// template <typename N, typename In, typename Out>
// struct UnaryNodeTester
// {
// 	std::shared_ptr<OutputValue<In>> parameter = std::make_shared<OutputValue<In>>();
// 	std::shared_ptr<InputValue<Out>> result = std::make_shared<InputValue<Out>>();
// 	std::shared_ptr<N> node = std::make_shared<N>("TestUnary");
// 	UnaryNodeTester(SendParam_t<In> param, SendParam_t<Out> expected)
// 	{
// 		parameter >>(node->Parameter);
// 		(*result)>>node->Result;
// 		parameter<<param;
// 		ElemType_t<Out> v;
// 		node->process();
// 		(*result)>>v;
// 		GTEST_ASSERT_EQ(expected, v);
// 	}
// };

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
	auto t = boost::thread(run);
	// auto t1 = UnaryNodeTester<Negate<int>, const int, const int>(5, -5);
	auto n = std::make_shared<Negate<int, DefaultChannelTypeInfo<int>>>("UnaryTest");
	std::shared_ptr<OutputValue<int>> parameter = std::make_shared<OutputValue<int>>("parameter");
	std::shared_ptr<InputValue<int>> result = std::make_shared<InputValue<int>>("result");

	std::cout << "Connecting" << std::endl;
	(*parameter) >> n->Parameter;
	*(n->Result) >> result;
	std::cout << "Passing value" << std::endl;
	*parameter << 5;
	int res;
	std::cout << "Processing" << std::endl;
	n->process();
	std::cout << "Receiving" << std::endl;
	*result >> res;
	GTEST_ASSERT_EQ(-5, res);
	doneVar = true;
	done.notify_one();
}

