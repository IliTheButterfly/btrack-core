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
	using InputValuePtr = type_traits::ownership::borrowed_ptr_p<InputValue<T, I>>;
	// using InputValueIterator = NodeIterator<InputValuePtr>;
	// NodeIteratorAccessorConcrete(InputValueIterator, InputValue, MetaInputArray);
protected:
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInputArray::MetaInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}


	NodeAtConcrete(InputValue, mInputs);
	NodeAtWeakCastImpl(_Input, mInputs);
	NodeAtWeakCastImpl(Input, mInputs);


	void attach(std::shared_ptr<_Input> input) override;
	void detach(std::shared_ptr<_Input> input) override;
	// InputValueIterator InputValueBegin();
	// InputValueIterator InputValueEnd();
	// _InputIterator _InputBegin() override;
    // _InputIterator _InputEnd() override;
	// InputIterator InputBegin() override;
	// InputIterator InputEnd() override;
};

// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::InputValueIterator MetaInputArray<T, I>::InputValueBegin() { return MetaInputArray<T, I>::InputValueIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::InputValueIterator MetaInputArray<T, I>::InputValueEnd() { return MetaInputArray<T, I>::InputValueIterator::create(mInputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::_InputIterator MetaInputArray<T, I>::_InputBegin() { return MetaInputArray<T, I>::_InputIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::_InputIterator MetaInputArray<T, I>::_InputEnd() { return MetaInputArray<T, I>::_InputIterator::create(mInputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::InputIterator MetaInputArray<T, I>::InputBegin() { return MetaInputArray<T, I>::InputIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaInputArray<T, I>::InputIterator MetaInputArray<T, I>::InputEnd() { return MetaInputArray<T, I>::InputIterator::create(mInputs.end()); }

template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputArray<T, I>::attach(std::shared_ptr<_Input> input)
{
	this->mInputs.emplace_back(std::reinterpret_pointer_cast<InputValue<T, I>>(input));
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaInputArray<T, I>::detach(std::shared_ptr<_Input> input)
{
	for (int i = 0; i < mInputs.size(); ++i)
	{
		if (this->mInputs.at(i)->uuid() == input->uuid())
		{
			this->mInputs.erase(mInputs.begin() + i);
			return;
		}
	}
}

} // namespace btrack::nodes::system
#endif // __METAINPUTARRAY_H__