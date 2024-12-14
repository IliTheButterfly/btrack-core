#pragma once
#include "nodes/NodeIO.h"
#include <vector>
#include <memory>

namespace btrack { namespace nodes {

class _NodeInput : public NodeIO
{
protected:
	_NodeInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_NodeInput::NodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
};

template <typename T>
class NodeInput : public _NodeInput
{
public:
    using DataPtr = std::shared_ptr<T>;
    using ChannelType = Channel<T>;
private:
	
protected:
	NodeInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeInput::_Node(_name, _nodeType, _friendlyName, _description) {}
public:
	constexpr const std::type_info& dataType() const override { return typeid(T); }
};

}} // namespace btrack::nodes