#pragma once
#include "nodes/NodeOutput.h"
#include "nodes/NodeInputArray.h"

namespace btrack { namespace nodes {

template <typename T>
class NodeOutputArray : public NodeOutput<T>
{
protected:
	std::vector<NodeInputArray<T>> mChildren;
public:
	NodeOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeOutputArray::NodeOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	using _NodeInputIterator = NodeOutput<T>::_NodeInputIterator;
	using NodeInputIterator = NodeOutput<T>::NodeInputIterator;
	
	using NodeInputArrayType = NodeInputArray<T>;
	using NodeInputArrayPtr = NodeInputArray<T>*;
	using NodeInputArrayIterator = NodeIterator<NodeInputArrayType, NodeInputArrayPtr>;
	NodeIteratorAccessorConcrete(NodeInputArrayIterator, NodeInputArray, NodeOutputArray<T>);




	NodeOutputArray<T>& operator>>(NodeInputArray<T>& input);

	NodeInputArrayIterator NodeInputArrayBegin() { return NodeInputArrayIterator::create(mChildren.begin()); }
	NodeInputArrayIterator NodeInputArrayEnd() { return NodeInputArrayIterator::create(mChildren.end()); }
	_NodeInputIterator _NodeInputBegin() { return _NodeInputIterator::create(mChildren.begin()); }
	_NodeInputIterator _NodeInputEnd() { return _NodeInputIterator::create(mChildren.end()); }
};

}} // btrack::nodes
