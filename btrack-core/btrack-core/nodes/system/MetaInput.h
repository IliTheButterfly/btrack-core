#pragma once
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/Input.h"
#include <vector>
#include <memory>

namespace btrack { namespace nodes { namespace system {


class _MetaInput : public MetaNodeIO
{
protected:
	_MetaInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaInput::MetaNodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
	using _InputType = _Input;
    using _InputPtr = std::shared_ptr<_Input>;
	using _InputIterator = NodeIterator<_InputPtr>;
	NodeIteratorAccessor(_InputIterator, _Input, _MetaInput);
};

template <typename T>
class MetaInput : public _MetaInput
{
protected:
	MetaInput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaInput::_MetaInput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _InputIterator = _MetaInput::_InputIterator;

    using InputType = Input<T>;
    using InputPtr = std::shared_ptr<Input<T>>;
	using InputIterator = NodeIterator<InputPtr>;
	NodeIteratorAccessor(InputIterator, Input, MetaInput);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
};

}}} // namespace btrack::nodes::system