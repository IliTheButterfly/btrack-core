#ifndef __METAINPUTARRAY_H__
#define __METAINPUTARRAY_H__


#include "nodes/system/MetaInput.h"
#include "nodes/system/InputValue.h"

namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaInputArray : public MetaInput<T, I>
{
public:
	using _InputType = typename MetaInput<T, I>::_InputType;
	using _InputPtr = typename MetaInput<T, I>::_InputPtr;
	using InputType = typename MetaInput<T, I>::InputType;
	using InputPtr = typename MetaInput<T, I>::InputPtr;
	

	using InputValueType = InputValue<T, I>;
	using InputValuePtr = borrowed_ptr_p<InputValue<T, I>>;
protected:
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInputArray::MetaInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}


	NodeAtWeakConcrete(InputValue, mInputs);
	NodeAtWeakCastImpl(_Input, mInputs);
	NodeAtWeakCastImpl(Input, mInputs);

	void attach(std::shared_ptr<_Input> input) override;
	void detach(std::shared_ptr<_Input> input) override;

	virtual ~MetaInputArray() = default;
};

template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputArray<T, I>::attach(std::shared_ptr<_Input> input)
{
	if (std::find_if(mInputs.begin(), mInputs.end(), [&](InputValuePtr i){ return !i.expired() && i.lock() == input; }) == mInputs.end())
	{
		mInputs.emplace_back(std::reinterpret_pointer_cast<InputValue<T, I>>(input));
	}
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputArray<T, I>::detach(std::shared_ptr<_Input> input)
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
#endif // __METAINPUTARRAY_H__