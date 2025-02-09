#ifndef __TEST_NODES_H__
#define __TEST_NODES_H__


#include "nodes/system/nodes.h"
#include "nodes/utilities/math/UnaryOperation.h"
#include "nodes/system/shortcuts.h"


using namespace btrack::nodes::system;
using namespace btrack::nodes::utilities::math;

class DummyGraph : public NodeObserver
{
public:
	void update() {}
};

class DummyNode : public _Node
{
public:
	DummyNode() : _Node::_Node("", NodeItemType::UNKNOWN) {}
	size_t inputCount() const { return 1; }
	size_t outputCount() const { return 1; }
};

auto dummy = std::make_shared<DummyNode>();

class NodeStart : public Node
{
private:
	struct Protected { explicit Protected() = default; };
public:
	std::shared_ptr<OutputValue<int>> valueOutput;
	std::shared_ptr<OutputValue<int>> arrayOutput;

	NodeStart(Protected _) : NodeStart::Node("nodeStart") { }

	static std::shared_ptr<NodeStart> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<NodeStart>(Protected());
		ret->valueOutput = ret->addOutputValue<int>("valueOutput");
		ret->arrayOutput = ret->addOutputValue<int>("arrayOutput");
		ret->mObserver = _observer;
		return ret;
	}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }

	void process() override
	{
	}
};

class TestNodeStart : public MetaNode
{
private:
	struct Protected { explicit Protected() = default; };
public:
	std::shared_ptr<MetaOutputValue<int>> valueOutput;
	std::shared_ptr<MetaOutputArray<int>> arrayOutput;
	TestNodeStart(Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeStart::MetaNode(_name, _friendlyName, _description) 
			{
				
			}

	static std::shared_ptr<TestNodeStart> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<TestNodeStart>(Protected(), "");
		ret->valueOutput = ret->addOutputValue<int>("valueOutput");
		ret->arrayOutput = ret->addOutputArray<int>("arrayOutput");
		ret->mObserver = _observer;
		return ret;
	}

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class NodeStartFloat : public Node
{
private:
	struct Protected { explicit Protected() = default; };
public:
	std::shared_ptr<OutputValue<float>> valueOutput;
	std::shared_ptr<OutputValue<float>> arrayOutput;

	NodeStartFloat(Protected _) : NodeStartFloat::Node("nodeStartFloat")
	{
		
	}

	static std::shared_ptr<NodeStartFloat> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<NodeStartFloat>(Protected());
		ret->valueOutput = ret->addOutputValue<float>("valueOutput");
		ret->arrayOutput = ret->addOutputValue<float>("arrayOutput");
		ret->mObserver = _observer;
		return ret;
	}
	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }

	void process() override
	{
		
	}
};

class TestNodeStartFloat : public MetaNode
{
private:
	struct Protected { explicit Protected() = default; };
public:
	std::shared_ptr<MetaOutputValue<float>> valueOutput;
	std::shared_ptr<MetaOutputArray<float>> arrayOutput;
	TestNodeStartFloat(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeStartFloat::MetaNode(_name, _friendlyName, _description) 
			{
				
			}

	static std::shared_ptr<TestNodeStartFloat> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<TestNodeStartFloat>(Protected(), "");
		ret->valueOutput = ret->addOutputValue<float>("valueOutput");
		ret->arrayOutput = ret->addOutputArray<float>("arrayOutput");
		ret->mObserver = _observer;
		return ret;
	}

	void generate(int count) override {}

	size_t inputCount() const override { return 0; }
	size_t outputCount() const override { return 2; }
};

class TestNodeMid1 : public MetaNode
{
private:
	struct Protected { explicit Protected() = default; };
public:
	std::shared_ptr<MetaInputValue<int>> valueInput;
	std::shared_ptr<MetaInputArray<int>> arrayInput;
	
	std::shared_ptr<MetaOutputValue<int>> valueOutput;
	std::shared_ptr<MetaOutputArray<int>> arrayOutput;

	TestNodeMid1(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeMid1::MetaNode(_name, _friendlyName, _description) 
			{
				
			}
	
	static std::shared_ptr<TestNodeMid1> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<TestNodeMid1>(Protected(), "");
		ret->valueInput = ret->addInputValue<int>("valueInput");
		ret->arrayInput = ret->addInputArray<int>("arrayInput");
		ret->valueOutput = ret->addOutputValue<int>("valueOutput");
		ret->arrayOutput = ret->addOutputArray<int>("arrayOutput");
		ret->mObserver = _observer;
		return ret;
	}

	void generate(int count) override {}

	size_t inputCount() const override { return 2; }
	size_t outputCount() const override { return 2; }
};

class TestNodeEnd : public MetaNode
{
private:
	struct Protected { explicit Protected() = default; };
public:
	TestNodeEnd(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			TestNodeEnd::MetaNode(_name, _friendlyName, _description) 
			{
				
			}
	std::shared_ptr<MetaInputValue<int>> valueInput;
	std::shared_ptr<MetaInputArray<int>> arrayInput;

	static std::shared_ptr<TestNodeEnd> create(std::shared_ptr<NodeObserver> _observer)
	{
		auto ret = std::make_shared<TestNodeEnd>(Protected(), "");
		ret->valueInput = ret->addInputValue<int>("valueInput");
		ret->arrayInput = ret->addInputArray<int>("arrayInput");
		ret->mObserver = _observer;
		return ret;
	}

	void generate(int count) override {}

	size_t inputCount() const override { return 2; }
	size_t outputCount() const override { return 0; }
};

#endif // __TEST_NODES_H__