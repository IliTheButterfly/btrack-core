#pragma once
#include "nodes/system/Input.h"

namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I>
class OutputValue;

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class InputValue : public Input<T, I>
{
public:
	InputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			InputValue::Input(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	InputValue<T, I>& operator>>(typename I::readonlyOut data);
	friend OutputValue<T, I>;
};

template <typename T, ChannelTypeConcept<T> I>
inline InputValue<T, I>& InputValue<T, I>::operator>>(typename I::readonlyOut data)
{
	if (this->chan) (this->chan)->receive(data);
	return *this; 
}


} // namespace btrack::nodes::system