#ifndef __METAINPUT_H__
#define __METAINPUT_H__


#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/Input.h"
#include <vector>
#include <memory>

namespace btrack::nodes::system {


class _MetaInput : public MetaNodeIO
{
protected:
	_MetaInput(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaInput::MetaNodeIO(runner, _name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
	using _InputType = _Input;
    using _InputPtr = type_traits::ownership::borrowed_ptr_p<_Input>;
	// using _InputIterator = NodeIterator<_InputPtr>;
	// NodeIteratorAccessor(_InputIterator, _Input, _MetaInput);
	
	virtual void attach(std::shared_ptr<_Input> input) = 0;
	virtual void detach(std::shared_ptr<_Input> input) = 0;

	NodeAtVirtual(_Input);

	virtual ~_MetaInput() = default;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaInput : public _MetaInput
{
protected:
	MetaInput(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInput::_MetaInput(runner, _name, _nodeType, _friendlyName, _description) {}
public:
	// using _InputIterator = _MetaInput::_InputIterator;

    using InputType = Input<T, I>;
    using InputPtr = std::shared_ptr<Input<T, I>>;
	// using InputIterator = NodeIterator<InputPtr>;
	// NodeIteratorAccessor(InputIterator, Input, MetaInput);

	NodeAtVirtual(Input);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual ~MetaInput() = default;
};



} // namespace btrack::nodes::system
#endif // __METAINPUT_H__