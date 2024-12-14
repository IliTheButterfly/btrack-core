#pragma once

#include <vector>
#include <memory>
#include "nodes/MetaNodeIO.h"
#include "nodes/MetaNodeOutputArray.h"
#include "nodes/MetaNodeOutputValue.h"
#include "nodes/MetaNodeInputArray.h"
#include "nodes/MetaNodeInputValue.h"
#include "nodes/Node.h"

namespace btrack { namespace nodes
{

class _MetaNode : public _Node
{
public:
	using NodeType = Node;
	using NodePtr = std::shared_ptr<Node>;
	using NodeIt = NodeIterator<NodePtr>;
	NodeIteratorAccessor(NodeIt, Node, _MetaNode);

	using MetaNodeOutputType = _MetaNodeOutput;
	using MetaNodeOutputPtr = std::weak_ptr<_MetaNodeOutput>;
	using MetaNodeOutputIterator = NodeIterator<MetaNodeOutputPtr>;
	NodeIteratorAccessorConcrete(MetaNodeOutputIterator, Outputs, _MetaNode);

	using MetaNodeInputType = _MetaNodeInput;
	using MetaNodeInputPtr = std::weak_ptr<_MetaNodeInput>;
	using MetaNodeInputIterator = NodeIterator<MetaNodeInputPtr>;
	NodeIteratorAccessorConcrete(MetaNodeInputIterator, Inputs, _MetaNode);
protected:
	std::vector<MetaNodeOutputPtr> mOutputs;
	std::vector<MetaNodeInputPtr> mInputs;

	template <typename T>
	constexpr std::shared_ptr<MetaNodeInputValue<T>> addInputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto input = std::make_shared<MetaNodeInputValue<T>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaNodeInputArray<T>> addInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto input = std::make_shared<MetaNodeInputArray<T>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaNodeOutputValue<T>> addOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto output = std::make_shared<MetaNodeOutputValue<T>>(_name, _friendlyName, _description); 
		mOutputs.push_back(output);
		return output;
	}

	template <typename T>
	constexpr std::shared_ptr<MetaNodeOutputArray<T>> addOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = "")
	{
		auto output = std::make_shared<MetaNodeOutputArray<T>>(_name, _friendlyName, _description); 
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


	MetaNodeOutputIterator OutputsBegin() { return MetaNodeOutputIterator::create(mOutputs.begin()); }
	MetaNodeOutputIterator OutputsEnd() { return MetaNodeOutputIterator::create(mOutputs.end()); }
	MetaNodeInputIterator InputsBegin() { return MetaNodeInputIterator::create(mInputs.begin()); }
	MetaNodeInputIterator InputsEnd() { return MetaNodeInputIterator::create(mInputs.end()); }

	MetaNodeInputPtr getInput(const std::string name)
	{
		for (MetaNodeInputPtr input : InputsIter())
		{
			if (!input.expired() && input.lock()->name() == name) return input;
		}
		return MetaNodeInputPtr();
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

}} // btrack::nodes