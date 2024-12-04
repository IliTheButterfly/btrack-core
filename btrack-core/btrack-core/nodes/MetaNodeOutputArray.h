#pragma once
#include "nodes/MetaNodeOutput.h"

namespace btrack { namespace nodes {

template <typename T>
class MetaNodeInputArray;

template <typename T>
class NodeOutputArray;

template <typename T>
class MetaNodeOutputArray : public MetaNodeOutput<T>
{
protected:
	std::vector<boost::shared_ptr<NodeOutputArray<T>>> mOutputs;
	std::vector<boost::shared_ptr<MetaNodeInputArray<T>>> mChildren;
public:
	MetaNodeOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeOutputArray::MetaNodeOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	// Grab iterators from parent
	using _NodeOutputIterator = MetaNodeOutput<T>::_NodeOutputIterator;
	using NodeOutputIterator = MetaNodeOutput<T>::NodeOutputIterator;
	using _MetaNodeInputIterator = MetaNodeOutput<T>::_MetaNodeInputIterator;
	using MetaNodeInputIterator = MetaNodeOutput<T>::MetaNodeInputIterator;

	// Define iterators
	using NodeOutputArrayType = NodeOutputArray<T>;
	using NodeOutputArrayPtr = boost::shared_ptr<NodeOutputArray<T>>;
	using NodeOutputArrayIterator = NodeIterator<MetaNodeOutputArray, NodeOutputArrayType, NodeOutputArrayPtr>;
	NodeIteratorAccessorConcrete(NodeOutputArrayIterator, NodeOutputArray);
	
	using MetaNodeInputArrayType = MetaNodeInputArray<T>;
	using MetaNodeInputArrayPtr = boost::shared_ptr<MetaNodeInputArray<T>>;
	using MetaNodeInputArrayIterator = NodeIterator<MetaNodeOutputArray, MetaNodeInputArrayType, MetaNodeInputArrayPtr>;
	NodeIteratorAccessorConcrete(MetaNodeInputArrayIterator, MetaNodeInputArray);
	
	// Implement iterators
	MetaNodeInputArrayIterator MetaNodeInputArrayBegin();
	MetaNodeInputArrayIterator MetaNodeInputArrayEnd();
	MetaNodeInputIterator MetaNodeInputBegin() override;
	MetaNodeInputIterator MetaNodeInputEnd() override;
	_MetaNodeInputIterator _MetaNodeInputBegin() override;
	_MetaNodeInputIterator _MetaNodeInputEnd() override;
	NodeOutputArrayIterator NodeOutputArrayBegin();
	NodeOutputArrayIterator NodeOutputArrayEnd();
	_NodeOutputIterator _NodeOutputBegin() override;
    _NodeOutputIterator _NodeOutputEnd() override;
	NodeOutputIterator NodeOutputBegin() override;
	NodeOutputIterator NodeOutputEnd() override;

	MetaNodeOutputArray<T>& operator>>(MetaNodeInputArray<T>& input);
};

}} // btrack::nodes
