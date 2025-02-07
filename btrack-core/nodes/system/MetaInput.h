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
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaInput::MetaNodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
	using _InputType = _Input;
    using _InputPtr = borrowed_ptr_p<_Input>;
	
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
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInput::_MetaInput(_name, _nodeType, _friendlyName, _description) {}
public:
    using InputType = Input<T, I>;
    using InputPtr = borrowed_ptr_p<Input<T, I>>;

	NodeAtVirtual(Input);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual ~MetaInput() = default;
};



} // namespace btrack::nodes::system
#endif // __METAINPUT_H__