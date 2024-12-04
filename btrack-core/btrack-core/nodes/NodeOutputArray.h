#pragma once
#include "nodes/NodeOutput.h"

namespace btrack { namespace nodes {

class _NodeInput;

template <typename T>
class NodeInput;

class _NodeInputArray;

template <typename T>
class NodeInputArray;

template <typename T>
class NodeOutputArray : public NodeOutput<T>
{
protected:
	std::vector<boost::shared_ptr<NodeInputArray<T>>> mChildren;
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
	using NodeInputArrayPtr = boost::shared_ptr<NodeInputArray<T>>;
	using NodeInputArrayIterator = NodeIterator<NodeOutputArray, NodeInputArrayType, NodeInputArrayPtr>;
	NodeIteratorAccessorConcrete(NodeInputArrayIterator, NodeInputArray);
	
	NodeOutputArray<T>& operator>>(NodeInputArray<T>& input);

	NodeInputArrayIterator NodeInputArrayBegin() { return NodeInputArrayIterator(mChildren.begin()); }
	NodeInputArrayIterator NodeInputArrayEnd() { return NodeInputArrayIterator(mChildren.end()); }
	_NodeInputIterator _NodeInputBegin() { return _NodeInputIterator(mChildren.begin()); }
	_NodeInputIterator _NodeInputEnd() { return _NodeInputIterator(mChildren.end()); }
};

}} // btrack::nodes
