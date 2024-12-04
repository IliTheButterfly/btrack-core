#pragma once
#include <vector>
#include <iterator>
#include "nodes/MetaNodeIO.h"

namespace btrack { namespace nodes {

class _NodeOutput;

template <typename T>
class NodeOutput;

class _NodeInput;

template <typename T>
class NodeInput;

class _MetaNodeInput;

template <typename T>
class MetaNodeInput;

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
    using _NodeOutputPtr = boost::shared_ptr<_NodeOutput>;
	using _NodeOutputIterator = NodeIterator<_MetaNodeOutput, _NodeOutputType, _NodeOutputPtr>;
	NodeIteratorAccessor(_NodeOutputIterator, _NodeOutput);

	using _MetaNodeInputType = _MetaNodeInput;
    using _MetaNodeInputPtr = boost::shared_ptr<_MetaNodeInput>;
	using _MetaNodeInputIterator = NodeIterator<_MetaNodeOutput, _MetaNodeInputType, _MetaNodeInputPtr>;
	NodeIteratorAccessor(_MetaNodeInputIterator, _MetaNodeInput);
};

template <typename T>
class MetaNodeOutput : public _MetaNodeOutput
{
protected:
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
    using NodeOutputPtr = boost::shared_ptr<NodeOutput<T>>;
	using NodeOutputIterator = NodeIterator<_MetaNodeOutput, NodeOutputType, NodeOutputPtr>;
	NodeIteratorAccessor(NodeOutputIterator, NodeOutput);

	using MetaNodeInputType = MetaNodeInput<T>;
    using MetaNodeInputPtr = boost::shared_ptr<MetaNodeInput<T>>;
	using MetaNodeInputIterator = NodeIterator<MetaNodeOutput<T>, MetaNodeInputType, MetaNodeInputPtr>;
	NodeIteratorAccessor(MetaNodeInputIterator, MetaNodeInput);

	const std::type_info& dataType() const override { return typeid(T); }
	
};

}} // namespace btrack::nodes