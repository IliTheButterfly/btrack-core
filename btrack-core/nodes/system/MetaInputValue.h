#ifndef __METAINPUTVALUE_H__
#define __METAINPUTVALUE_H__


#include "nodes/system/MetaInput.h"

namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I>
class InputValue;

template <typename T, ChannelTypeConcept<T> I>
class InputArray;

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaInputValue : public MetaInput<T, I>
{
public:
	using _InputType = typename MetaInput<T, I>::_InputType;
	using _InputPtr = typename MetaInput<T, I>::_InputPtr;
	using InputType = typename MetaInput<T, I>::InputType;
	using InputPtr = typename MetaInput<T, I>::InputPtr;


	using InputValueType = InputValue<T, I>;
	using InputValuePtr = borrowed_ptr_p<InputValue<T, I>>;
	// using InputValueIterator = NodeIterator<InputValuePtr>;
	// NodeIteratorAccessorConcrete(InputValueIterator, InputValue, MetaInputValue);
protected:
	struct Protected { explicit Protected() = default; };
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputValue(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInputValue::MetaInput(_name, NodeItemType::VALUE, _friendlyName, _description) {}

	static std::shared_ptr<MetaInputValue> create(
		std::shared_ptr<_Node> _parent,
		std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		)
	{
		auto ret = std::make_shared<MetaInputValue>(Protected(), _name, _friendlyName, _description);
		ret->mParent = _parent;
		ret->mObserver = _observer;
		return ret;
	}

	NodeAtWeakConcrete(InputValue, mInputs);
	NodeAtWeakCastImpl(_Input, mInputs);
	NodeAtWeakCastImpl(Input, mInputs);

	void attach(std::shared_ptr<_Input> input) override;
	void detach(std::shared_ptr<_Input> input) override;

	virtual ~MetaInputValue() = default;
};


template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputValue<T, I>::attach(std::shared_ptr<_Input> input)
{
	if (std::find_if(mInputs.begin(), mInputs.end(), [&](InputValuePtr i){ return !i.expired() && i.lock() == input; }) == mInputs.end())
	{
		mInputs.emplace_back(std::dynamic_pointer_cast<InputValue<T, I>>(input));
	}
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputValue<T, I>::detach(std::shared_ptr<_Input> input)
{
	for (auto i = mInputs.begin(); i != mInputs.end(); )
	{
		if (i->expired() || !i->lock()) 
		{
			mInputs.erase(i);
			continue;
		}
		if (i->lock()->uuid() == input->uuid()) 
		{
			mInputs.erase(i);
			return;
		}
		++i;
	}
}


} // namespace btrack::nodes::system
#endif // __METAINPUTVALUE_H__