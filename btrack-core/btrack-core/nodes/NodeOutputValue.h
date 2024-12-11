#pragma once
#include "nodes/NodeOutput.h"
#include "nodes/NodeInputValue.h"
#include "nodes/NodeInputArray.h"


namespace btrack { namespace nodes {

template <typename T>
class NodeOutputValue : public NodeOutput<T>
{
protected:
	std::vector<NodeInput<T>> mChildren;
public:
	NodeOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeOutputValue::NodeOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	using _NodeInputIterator = NodeOutput<T>::_NodeInputIterator;
	using NodeInputIterator = NodeOutput<T>::NodeInputIterator;

	NodeInputIterator NodeInputBegin() { return NodeInputIterator::create(mChildren.begin()); }
	NodeInputIterator NodeInputEnd() { return NodeInputIterator::create(mChildren.end()); }
	_NodeInputIterator _NodeInputBegin() { return _NodeInputIterator::create(mChildren.begin()); }
	_NodeInputIterator _NodeInputEnd() { return _NodeInputIterator::create(mChildren.end()); }

	NodeOutputValue<T>& operator>>(NodeInputValue<T>& input);
	NodeOutputValue<T>& operator>>(NodeInputArray<T>& input);

};



}} // namespace btrack::nodes