#ifndef __INPUTVALUE_H__
#define __INPUTVALUE_H__


#include "nodes/system/Input.h"

namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I>
class OutputValue;

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class InputValue : public Input<T, I>
{
protected:
	struct Protected { explicit Protected() = default; };
public:
	InputValue(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			InputValue::Input(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	static std::shared_ptr<InputValue> create(
		std::shared_ptr<_Node> _parent,
		std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		)
	{
		auto ret = std::make_shared<InputValue>(Protected(), _name, _friendlyName, _description);
		ret->mParent = _parent;
		ret->mObserver = _observer;
		return ret;
	}

	InputValue<T, I>& operator>>(typename I::readonlyOut data);
	friend OutputValue<T, I>;

	virtual ~InputValue() = default;
};

template <typename T, ChannelTypeConcept<T> I>
inline InputValue<T, I>& InputValue<T, I>::operator>>(typename I::readonlyOut data)
{
	if (this->mChannel) (this->mChannel)->receive(data);
	return *this; 
}


} // namespace btrack::nodes::system
#endif // __INPUTVALUE_H__