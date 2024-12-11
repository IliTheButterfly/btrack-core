#pragma once
#include "nodes/MetaNodeIO.h"
#include "nodes/NodeInput.h"
#include <vector>
#include <memory>

namespace btrack { namespace nodes {


class _MetaNodeInput : public MetaNodeIO
{
protected:
	_MetaNodeInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaNodeInput::MetaNodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
	using _NodeInputType = _NodeInput;
    using _NodeInputPtr = _NodeInput*;
	using _NodeInputIterator = NodeIterator<_NodeInputType, _NodeInputPtr>;
	NodeIteratorAccessor(_NodeInputIterator, _NodeInput, _MetaNodeInput);
};

template <typename T>
class MetaNodeInput : public _MetaNodeInput
{
protected:
	MetaNodeInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeInput::_MetaNodeInput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _NodeInputIterator = _MetaNodeInput::_NodeInputIterator;

    using NodeInputType = NodeInput<T>;
    using NodeInputPtr = NodeInput<T>*;
	using NodeInputIterator = NodeIterator<NodeInputType, NodeInputPtr>;
	NodeIteratorAccessor(NodeInputIterator, NodeInput, MetaNodeInput);


	const std::type_info& dataType() const override { return typeid(T); }

};

}} // namespace btrack::nodes