#pragma once
#include <vector>
#include <iterator>
#include "nodes/system/NodeIO.h"

namespace btrack { namespace nodes { namespace system {

class _Input;

template <typename T>
class Input;

class _Output : public NodeIO
{
protected:
	_Output(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_Output::NodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _InputType = _Input;
    using _InputPtr = std::shared_ptr<_Input>;
	using _InputIterator = NodeIterator<_InputPtr>;
	NodeIteratorAccessor(_InputIterator, _Input, _Output);
};

template <typename T>
class Output : public _Output
{
public:
    using DataPtr = std::shared_ptr<T>;
    using ChannelType = Channel<T>;
protected:
	Output(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			Output::_Output(_name, _nodeType, _friendlyName, _description) {}
public:
	using _InputIterator = _Output::_InputIterator;

    using InputType = Input<T>;
    using InputPtr = std::shared_ptr<Input<T>>;
	using InputIterator = NodeIterator<InputPtr>;
	NodeIteratorAccessor(InputIterator, Input, Output<T>);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
};

}}} // namespace btrack::nodes::system