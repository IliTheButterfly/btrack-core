#pragma once
#include "nodes/NodeOutput.h"


namespace btrack { namespace nodes {

template <typename T>
class NodeInput;
template <typename T>
class NodeInputArray;
template <typename T>
class NodeInputValue;

template <typename T>
class NodeOutputValue : public NodeOutput<T>
{
protected:
	std::vector<boost::shared_ptr<NodeInput<T>>> mChildren;
public:
	NodeOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeOutputValue::NodeOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	using _NodeInputIterator = NodeOutput<T>::_NodeInputIterator;
	using NodeInputIterator = NodeOutput<T>::NodeInputIterator;

	NodeInputIterator NodeInputBegin() { return NodeInputIterator(mChildren.begin()); }
	NodeInputIterator NodeInputEnd() { return NodeInputIterator(mChildren.end()); }
	_NodeInputIterator _NodeInputBegin() { return _NodeInputIterator(mChildren.begin()); }
	_NodeInputIterator _NodeInputEnd() { return _NodeInputIterator(mChildren.end()); }

	NodeOutputValue<T>& operator>>(NodeInputValue<T>& input);
	NodeOutputValue<T>& operator>>(NodeInput<T>& input);

};



}} // namespace btrack::nodes