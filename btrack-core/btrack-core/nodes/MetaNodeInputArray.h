#pragma once
#include "nodes/MetaNodeInput.h"

namespace btrack { namespace nodes {

template <typename T>
class NodeInputArray;

template <typename T>
class MetaNodeInputArray : public MetaNodeInput<T>
{
protected:
	std::vector<boost::shared_ptr<NodeInputArray<T>>> mInputs;
public:
	MetaNodeInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeInputArray::MetaNodeInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	using _NodeInputIterator = MetaNodeInput<T>::_NodeInputIterator;
	using NodeInputIterator = MetaNodeInput<T>::NodeInputIterator;
	

	using NodeInputArrayType = NodeInputArray<T>;
	using NodeInputArrayPtr = boost::shared_ptr<NodeInputArray<T>>;
	using NodeInputArrayIterator = NodeIterator<MetaNodeInputArray, NodeInputArrayType, NodeInputArrayPtr>;
	NodeIteratorAccessorConcrete(NodeInputArrayIterator, NodeInputArray);

	NodeInputArrayIterator NodeInputArrayBegin();
	NodeInputArrayIterator NodeInputArrayEnd();
	_NodeInputIterator _NodeInputBegin() override;
    _NodeInputIterator _NodeInputEnd() override;
	NodeInputIterator NodeInputBegin() override;
	NodeInputIterator NodeInputEnd() override;
};

}} // namespace btrack::nodes