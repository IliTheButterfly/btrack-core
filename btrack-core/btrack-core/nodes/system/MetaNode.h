#pragma once

#include <vector>
#include <memory>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaOutputArray.h"
#include "nodes/system/MetaOutputValue.h"
#include "nodes/system/MetaInputArray.h"
#include "nodes/system/MetaInputValue.h"
#include "nodes/system/Node.h"

namespace btrack { namespace nodes { namespace system {

class _MetaNode : public _Node
{
public:
	using NodeType = Node;
	using NodePtr = std::shared_ptr<Node>;
	using NodeIt = NodeIterator<NodePtr>;
	NodeIteratorAccessor(NodeIt, Node, _MetaNode);

	using MetaOutputType = _MetaOutput;
	using MetaOutputPtr = std::weak_ptr<_MetaOutput>;
	using MetaOutputIterator = NodeIterator<MetaOutputPtr>;
	NodeIteratorAccessorConcrete(MetaOutputIterator, Outputs, _MetaNode);

	using MetaInputType = _MetaInput;
	using MetaInputPtr = std::weak_ptr<_MetaInput>;
	using MetaInputIterator = NodeIterator<MetaInputPtr>;
	NodeIteratorAccessorConcrete(MetaInputIterator, Inputs, _MetaNode);
protected:
	std::vector<MetaOutputPtr> mOutputs;
	std::vector<MetaInputPtr> mInputs;

	template <typename T>
	constexpr std::shared_ptr<MetaInputValue<T>> addInputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto input = std::make_shared<MetaInputValue<T>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaInputArray<T>> addInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto input = std::make_shared<MetaInputArray<T>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaOutputValue<T>> addOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto output = std::make_shared<MetaOutputValue<T>>(_name, _friendlyName, _description); 
		mOutputs.push_back(output);
		return output;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaOutputArray<T>> addOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto output = std::make_shared<MetaOutputArray<T>>(_name, _friendlyName, _description); 
		mOutputs.push_back(output);
		return output;
	}
public:
	_MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaNode::_Node(_name, NodeItemType::META | NodeItemType::NODE, _friendlyName, _description) {}


	MetaOutputIterator OutputsBegin() { return MetaOutputIterator::create(mOutputs.begin()); }
	MetaOutputIterator OutputsEnd() { return MetaOutputIterator::create(mOutputs.end()); }
	MetaInputIterator InputsBegin() { return MetaInputIterator::create(mInputs.begin()); }
	MetaInputIterator InputsEnd() { return MetaInputIterator::create(mInputs.end()); }

	MetaInputPtr getInput(const std::string name)
	{
		for (MetaInputPtr input : InputsIter())
		{
			if (!input.expired() && input.lock()->name() == name) return input;
		}
		return MetaInputPtr();
	}

	virtual void generate(int count) = 0;
	friend class NodeGraph;
};

class MetaNode : public _MetaNode
{
protected:
	std::vector<std::shared_ptr<Node>> mNodes;
public:
	MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNode::_MetaNode(_name, _friendlyName, _description) {}
	// using NodeIOIterator = _MetaNode::NodeIOIterator;
	using NodeIt = _MetaNode::NodeIt;

	NodeIt NodeBegin() override { return NodeIt::create(mNodes.begin()); }
	NodeIt NodeEnd() override { return NodeIt::create(mNodes.end()); }
};

}}} // namespace btrack::nodes::system