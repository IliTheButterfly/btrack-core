#ifndef __METANODE_H__
#define __METANODE_H__



#include <vector>
#include <memory>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaOutputArray.h"
#include "nodes/system/MetaOutputValue.h"
#include "nodes/system/MetaInputArray.h"
#include "nodes/system/MetaInputValue.h"
#include "nodes/system/Node.h"

namespace btrack::nodes::system {

using namespace type_traits::ownership;

class _MetaNode : public _Node
{
public:
	using NodeType = Node;
	using NodePtr = owned_ptr_p<Node>;
	// using NodeIt = NodeIterator<NodePtr>;
	// NodeIteratorAccessor(NodeIt, Node, _MetaNode);

	using _MetaOutputType = _MetaOutput;
	using _MetaOutputPtr = owned_ptr_p<_MetaOutput>;
	// using MetaOutputIterator = NodeIterator<MetaOutputPtr>;
	// NodeIteratorAccessorConcrete(MetaOutputIterator, Outputs, _MetaNode);

	using _MetaInputType = _MetaInput;
	using _MetaInputPtr = owned_ptr_p<_MetaInput>;
	// using MetaInputIterator = NodeIterator<MetaInputPtr>;
	// NodeIteratorAccessorConcrete(MetaInputIterator, Inputs, _MetaNode);
protected:
	std::vector<_MetaOutputPtr> mOutputs;
	std::vector<_MetaInputPtr> mInputs;

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<MetaInputValue<T, I>> addInputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = MetaInputValue<T, I>::create(this->asNode(), this->asObserver(), _name, _friendlyName, _description); 
		mInputs.push_back(input);
		IF_WEAK_VALID(mObserver)->addItem(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<MetaInputArray<T, I>> addInputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = MetaInputArray<T, I>::create(this->asNode(), this->asObserver(), _name, _friendlyName, _description); 
		mInputs.push_back(input);
		IF_WEAK_VALID(mObserver)->addItem(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<MetaOutputValue<T, I>> addOutputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = MetaOutputValue<T, I>::create(this->asNode(), this->asObserver(), _name, _friendlyName, _description); 
		mOutputs.push_back(output);
		IF_WEAK_VALID(mObserver)->addItem(output);
		return output;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<MetaOutputArray<T, I>> addOutputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = MetaOutputArray<T, I>::create(this->asNode(), this->asObserver(), _name, _friendlyName, _description); 
		mOutputs.push_back(output);
		IF_WEAK_VALID(mObserver)->addItem(output);
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


	_MetaInputPtr getInput(const std::string_view name)
	{
		auto it = std::find_if(mInputs.begin(), mInputs.end(), [&](_MetaInputPtr i){ return i->name() == name; });
		if (it != mInputs.end()) return *it;
		return _MetaInputPtr();
	}

	_MetaOutputPtr getOutput(const std::string_view name)
	{
		auto it = std::find_if(mOutputs.begin(), mOutputs.end(), [&](_MetaOutputPtr i){ return i->name() == name; });
		if (it != mOutputs.end()) return *it;
		return _MetaOutputPtr();
	}

	virtual void generate(int count) = 0;
	friend class NodeGraph;
	virtual ~_MetaNode()
	{
		for (auto input : mInputs)
		{
			IF_WEAK_VALID(mObserver)->removeItem(input);
		}
		for (auto output : mOutputs)
		{
			IF_WEAK_VALID(mObserver)->removeItem(output);
		}
	}
};

class MetaNode : public _MetaNode
{
protected:
	std::vector<std::shared_ptr<Node>> mNodes;

	template <std::derived_from<Node> NodeType>
	void generateImpl(int count)
	{
		for (int i = 0; i != count; ++i)
		{
			auto node = NodeType::create(this->asObserver(), name(), friendlyName(), description());
			IF_WEAK_VALID(mObserver)->addItem(node);
			for (int ii = 0; ii < this->inputCount(); ++ii)
			{
				this->_MetaInputAt(ii)->attach(node->_InputAt(ii));
			}
			for (int ii = 0; ii < this->outputCount(); ++ii)
			{
				this->_MetaOutputAt(ii)->attach(node->_OutputAt(ii));
			}
		}
	}
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


	virtual ~MetaNode()
	{
		for (auto node : mNodes)
		{
			IF_WEAK_VALID(mObserver)->removeItem(node);
		}
	}
};

} // namespace btrack::nodes::system
#endif // __METANODE_H__