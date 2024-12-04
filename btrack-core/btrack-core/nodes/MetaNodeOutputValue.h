#pragma once
#include "nodes/MetaNodeOutput.h"


namespace btrack { namespace nodes {

template <typename T>
class MetaNodeInput;
template <typename T>
class MetaNodeInputArray;
template <typename T>
class MetaNodeInputValue;

template <typename T>
class NodeOutputValue;

template <typename T>
class NodeOutputArray;

template <typename T>
class MetaNodeOutputValue : public MetaNodeOutput<T>
{
protected:
	std::vector<boost::shared_ptr<NodeOutputValue<T>>> mOutputs;
	std::vector<boost::shared_ptr<MetaNodeInput<T>>> mChildren;
public:
	MetaNodeOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeOutputValue::MetaNodeOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	// Grab iterators from parent
	using _NodeOutputIterator = MetaNodeOutput<T>::_NodeOutputIterator;
	using NodeOutputIterator = MetaNodeOutput<T>::NodeOutputIterator;
	using _MetaNodeInputIterator = MetaNodeOutput<T>::_MetaNodeInputIterator;
	using MetaNodeInputIterator = MetaNodeOutput<T>::MetaNodeInputIterator;

	// Define iterators
	using NodeOutputValueType = NodeOutputValue<T>;
	using NodeOutputValuePtr = boost::shared_ptr<NodeOutputValue<T>>;
	using NodeOutputValueIterator = NodeIterator<MetaNodeOutputValue, NodeOutputValueType, NodeOutputValuePtr>;
	NodeIteratorAccessorConcrete(NodeOutputValueIterator, NodeOutputValue);
	
	// Implement iterators
	MetaNodeInputIterator MetaNodeInputBegin() override;
	MetaNodeInputIterator MetaNodeInputEnd() override;
	_MetaNodeInputIterator _MetaNodeInputBegin() override;
	_MetaNodeInputIterator _MetaNodeInputEnd() override;
	NodeOutputValueIterator NodeOutputValueBegin();
	NodeOutputValueIterator NodeOutputValueEnd();
	_NodeOutputIterator _NodeOutputBegin() override;
    _NodeOutputIterator _NodeOutputEnd() override;
	NodeOutputIterator NodeOutputBegin() override;
	NodeOutputIterator NodeOutputEnd() override;

	MetaNodeOutputValue<T>& operator>>(MetaNodeInputValue<T>& input);
	MetaNodeOutputValue<T>& operator>>(MetaNodeInputArray<T>& input);
};



}} // namespace btrack::nodes