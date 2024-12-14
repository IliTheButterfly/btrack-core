#pragma once
#include "nodes/system/NodeIO.h"
#include <vector>
#include <memory>

namespace btrack { namespace nodes { namespace system {

class _Input : public NodeIO
{
protected:
	_Input(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_Input::NodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
};

template <typename T>
class Input : public _Input
{
public:
    using DataPtr = std::shared_ptr<T>;
    using ChannelType = Channel<T>;
private:
	
protected:
	Input(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			Input::_Node(_name, _nodeType, _friendlyName, _description) {}
public:
	constexpr const std::type_info& dataType() const override { return typeid(T); }
};

}}} // namespace btrack::nodes::system