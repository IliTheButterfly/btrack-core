#pragma once

#include <vector>
#include <memory>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaOutputArray.h"
#include "nodes/system/MetaOutputValue.h"
#include "nodes/system/MetaInputArray.h"
#include "nodes/system/MetaInputValue.h"
#include "nodes/system/Node.h"

namespace btrack::nodes::system {

class _MetaNode : public _Node
{
public:
	using NodeType = Node;
	using NodePtr = type_traits::ownership::borrowed_ptr_p<Node>;
	// using NodeIt = NodeIterator<NodePtr>;
	// NodeIteratorAccessor(NodeIt, Node, _MetaNode);

	using _MetaOutputType = _MetaOutput;
	using _MetaOutputPtr = type_traits::ownership::borrowed_ptr_p<_MetaOutput>;
	// using MetaOutputIterator = NodeIterator<MetaOutputPtr>;
	// NodeIteratorAccessorConcrete(MetaOutputIterator, Outputs, _MetaNode);

	using _MetaInputType = _MetaInput;
	using _MetaInputPtr = type_traits::ownership::borrowed_ptr_p<_MetaInput>;
	// using MetaInputIterator = NodeIterator<MetaInputPtr>;
	// NodeIteratorAccessorConcrete(MetaInputIterator, Inputs, _MetaNode);
protected:
	std::vector<_MetaOutputPtr> mOutputs;
	std::vector<_MetaInputPtr> mInputs;

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	constexpr std::shared_ptr<MetaInputValue<T, I>> addInputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = std::make_shared<MetaInputValue<T, I>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	constexpr std::shared_ptr<MetaInputArray<T, I>> addInputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = std::make_shared<MetaInputArray<T, I>>(_name, _friendlyName, _description); 
		mInputs.push_back(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	constexpr std::shared_ptr<MetaOutputValue<T, I>> addOutputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = std::make_shared<MetaOutputValue<T, I>>(_name, _friendlyName, _description); 
		mOutputs.push_back(output);
		return output;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	constexpr std::shared_ptr<MetaOutputArray<T, I>> addOutputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = std::make_shared<MetaOutputArray<T, I>>(_name, _friendlyName, _description); 
		mOutputs.push_back(output);
		return output;
	}
public:
	_MetaNode(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaNode::_Node(_name, NodeItemType::META | NodeItemType::NODE, _friendlyName, _description) {}


	NodeAtVirtual(Node);
	NodeAtConcrete(_MetaOutput, mOutputs);
	NodeAtConcrete(_MetaInput, mInputs);


	_MetaInputPtr getInput(const std::string name)
	{
		for (_MetaInputPtr input : mInputs)
		{
			// if (!input.expired() && input.lock()->name() == name) return input;
		}
		return _MetaInputPtr();
	}

	_MetaOutputPtr getOutput(const std::string name)
	{
		for (_MetaOutputPtr output : mOutputs)
		{
			// if (!output.expired() && output.lock()->name() == name) return output;
		}
		return _MetaOutputPtr();
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
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaNode::_MetaNode(_name, _friendlyName, _description) {}
	// using NodeIOIterator = _MetaNode::NodeIOIterator;
	using NodePtr = typename _MetaNode::NodePtr;
	using NodeType = typename _MetaNode::NodeType;

	NodeAtImpl(Node, mNodes);
};

} // namespace btrack::nodes::system