#pragma once
#include <vector>
#include <iterator>
#include "nodes/MetaNodeIO.h"
#include "nodes/MetaNodeInput.h"
#include "nodes/NodeOutput.h"

namespace btrack { namespace nodes {

class _MetaNodeOutput : public MetaNodeIO
{
protected:
	_MetaNodeOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaNodeOutput::MetaNodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _NodeOutputType = _NodeOutput;
    using _NodeOutputPtr = _NodeOutput*;
	using _NodeOutputIterator = NodeIterator<_NodeOutputType, _NodeOutputPtr>;
	NodeIteratorAccessor(_NodeOutputIterator, _NodeOutput, _MetaNodeOutput);

	using _MetaNodeInputType = _MetaNodeInput;
    using _MetaNodeInputPtr = _MetaNodeInput*;
	using _MetaNodeInputIterator = NodeIterator<_MetaNodeInputType, _MetaNodeInputPtr>;
	NodeIteratorAccessor(_MetaNodeInputIterator, _MetaNodeInput, _MetaNodeOutput);
};

template <typename T>
class MetaNodeOutput : public _MetaNodeOutput
{
protected:
	Channel<T> mChannel;

	MetaNodeOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeOutput::_MetaNodeOutput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _NodeOutputIterator = _MetaNodeOutput::_NodeOutputIterator;
	using _MetaNodeInputIterator = _MetaNodeOutput::_MetaNodeInputIterator;

    using NodeOutputType = NodeOutput<T>;
    using NodeOutputPtr = NodeOutput<T>*;
	using NodeOutputIterator = NodeIterator<NodeOutputType, NodeOutputPtr>;
	NodeIteratorAccessor(NodeOutputIterator, NodeOutput, MetaNodeOutput<T>);

	using MetaNodeInputType = MetaNodeInput<T>;
    using MetaNodeInputPtr = MetaNodeInput<T>*;
	using MetaNodeInputIterator = NodeIterator<MetaNodeInputType, MetaNodeInputPtr>;
	NodeIteratorAccessor(MetaNodeInputIterator, MetaNodeInput, MetaNodeOutput<T>);

	const std::type_info& dataType() const override { return typeid(T); }
	
};

}} // namespace btrack::nodes