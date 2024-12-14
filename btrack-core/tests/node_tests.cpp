#include <gtest/gtest.h>
#include "nodes/system/nodes.h"

using namespace btrack::nodes::system;

class NodeStart : public Node
{
public:
	OutputValue<int> valueOutput{"valueOutput"};
	OutputValue<int> arrayOutput{"arrayOutput"};

	void process() override
	{
		
	}
};

class TestNodeStart : public MetaNode
{
public:
	TestNodeStart(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			TestNodeStart::MetaNode(_name, _friendlyName, _description) {}
	std::shared_ptr<MetaOutputValue<int>> valueOutput = std::make_shared<MetaOutputValue<int>>("valueOutput");
	std::shared_ptr<MetaOutputArray<int>> arrayOutput = std::make_shared<MetaOutputArray<int>>("arrayOutput");

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class NodeStartFloat : public Node
{
public:
	OutputValue<float> valueOutput{"valueOutput"};
	OutputValue<float> arrayOutput{"arrayOutput"};

	void process() override
	{
		
	}
};

class TestNodeStartFloat : public MetaNode
{
public:
	TestNodeStartFloat(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			TestNodeStartFloat::MetaNode(_name, _friendlyName, _description) {}
	MetaOutputValue<float> valueOutput{"valueOutput"};
	MetaOutputArray<float> arrayOutput{"arrayOutput"};

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class TestNodeMid1 : public MetaNode
{
public:
	TestNodeMid1(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			TestNodeMid1::MetaNode(_name, _friendlyName, _description) {}
	std::shared_ptr<MetaInputValue<int>> valueInput = std::make_shared<MetaInputValue<int>>("valueInput");
	std::shared_ptr<MetaInputArray<int>> arrayInput = std::make_shared<MetaInputArray<int>>("arrayInput");
	
	std::shared_ptr<MetaOutputValue<int>> valueOutput = std::make_shared<MetaOutputValue<int>>("valueOutput");
	std::shared_ptr<MetaOutputArray<int>> arrayOutput = std::make_shared<MetaOutputArray<int>>("arrayOutput");

	void generate(int count) override {}

	size_t inputCount() const override { return 2; }
	size_t outputCount() const override { return 2; }
};

class TestNodeEnd : public MetaNode
{
public:
	TestNodeEnd(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			TestNodeEnd::MetaNode(_name, _friendlyName, _description) {}
	std::shared_ptr<MetaInputValue<int>> valueInput = std::make_shared<MetaInputValue<int>>("valueInput");
	std::shared_ptr<MetaInputArray<int>> arrayInput = std::make_shared<MetaInputArray<int>>("arrayInput");

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
 * @brief Create a chain of Meta Nodes and link their input and outputs. 
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

// TEST(NodeTests, TypeConversionChain)
// {
// 	auto start = TestNodeStartFloat("start", "Start");
// 	auto mid = TestNodeMid1("mid", "Middle");

// 	(*start.valueOutput)>>mid.valueInput;
// 	(*start.arrayOutput)>>mid.arrayInput;
// }


