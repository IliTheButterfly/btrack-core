#pragma once
#include <vector>
#include <iterator>
#include "nodes/NodeIO.h"

namespace btrack { namespace nodes {

class _NodeInput;

template <typename T>
class NodeInput;

class _NodeOutput : public NodeIO
{
protected:
	_NodeOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_NodeOutput::NodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _NodeInputType = _NodeInput;
    using _NodeInputPtr = std::shared_ptr<_NodeInput>;
	using _NodeInputIterator = NodeIterator<_NodeInputPtr>;
	NodeIteratorAccessor(_NodeInputIterator, _NodeInput, _NodeOutput);
};

template <typename T>
class NodeOutput : public _NodeOutput
{
public:
    using DataPtr = std::shared_ptr<T>;
    using ChannelType = Channel<T>;
protected:
	NodeOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeOutput::_NodeOutput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _NodeInputIterator = _NodeOutput::_NodeInputIterator;

    using NodeInputType = NodeInput<T>;
    using NodeInputPtr = std::shared_ptr<NodeInput<T>>;
	using NodeInputIterator = NodeIterator<NodeInputPtr>;
	NodeIteratorAccessor(NodeInputIterator, NodeInput, NodeOutput<T>);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
};

}} // namespace btrack::nodes