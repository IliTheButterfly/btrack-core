#include <gtest/gtest.h>
#include "nodes/nodes.h"

using namespace btrack::nodes;

class TestNodeStart : public MetaNode
{
public:
	TestNodeStart(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			TestNodeStart::MetaNode(_name, _friendlyName, _description) {}
	MetaNodeOutputValue<int> valueOutput{"valueOutput"};
	MetaNodeOutputArray<int> arrayOutput{"arrayOutput"};


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
	MetaNodeInputValue<int> valueInput{"valueInput"};
	MetaNodeInputArray<int> arrayInput{"arrayInput"};
	
	MetaNodeOutputValue<int> valueOutput{"valueOutput"};
	MetaNodeOutputArray<int> arrayOutput{"arrayOutput"};
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
	using NodeIt = MetaNode::NodeIt;
	MetaNodeInputValue<int> valueInput{"valueInput"};
	MetaNodeInputArray<int> arrayInput{"arrayInput"};
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

	start.valueOutput>>mid.valueInput;
	start.arrayOutput>>mid.arrayInput;

	mid.valueOutput>>end.valueInput;
	mid.arrayOutput>>end.arrayInput;
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

	start.valueOutput>>end.arrayInput;
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

	start.valueOutput>>mid.valueInput;
	start.arrayOutput>>mid.arrayInput;

	mid.valueOutput>>end.valueInput;
	mid.arrayOutput>>end.arrayInput;
}

