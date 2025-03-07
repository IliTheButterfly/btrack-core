#pragma once
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
	using InputValuePtr = type_traits::ownership::borrowed_ptr_p<InputValue<T, I>>;
	// using InputValueIterator = NodeIterator<InputValuePtr>;
	// NodeIteratorAccessorConcrete(InputValueIterator, InputValue, MetaInputValue);
protected:
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaInputValue::MetaInput(_name, NodeItemType::VALUE, _friendlyName, _description) {}

	NodeAtConcrete(InputValue, mInputs);
	NodeAtWeakCastImpl(_Input, mInputs);
	NodeAtWeakCastImpl(Input, mInputs);
};

// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::InputValueIterator MetaInputValue<T, I>::InputValueBegin() { return MetaInputValue<T, I>::InputValueIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::InputValueIterator MetaInputValue<T, I>::InputValueEnd() { return MetaInputValue<T, I>::InputValueIterator::create(mInputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::_InputIterator MetaInputValue<T, I>::_InputBegin() { return MetaInputValue<T, I>::_InputIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::_InputIterator MetaInputValue<T, I>::_InputEnd() { return MetaInputValue<T, I>::_InputIterator::create(mInputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::InputIterator MetaInputValue<T, I>::InputBegin() { return MetaInputValue<T, I>::InputIterator::create(mInputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// MetaInputValue<T, I>::InputIterator MetaInputValue<T, I>::InputEnd() { return MetaInputValue<T, I>::InputIterator::create(mInputs.end()); }


} // namespace btrack::nodes::system