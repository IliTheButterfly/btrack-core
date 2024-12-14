#pragma once
#include "nodes/system/MetaInput.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class InputValue;

template <typename T>
class InputArray;

template <typename T>
class MetaInputValue : public MetaInput<T>
{
public:
	using _InputIterator = MetaInput<T>::_InputIterator;
	using InputIterator = MetaInput<T>::InputIterator;


	using InputValueType = InputValue<T>;
	using InputValuePtr = std::shared_ptr<InputValue<T>>;
	using InputValueIterator = NodeIterator<InputValuePtr>;
	NodeIteratorAccessorConcrete(InputValueIterator, InputValue, MetaInputValue);
protected:
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaInputValue::MetaInput(_name, NodeItemType::VALUE, _friendlyName, _description) {}

	

	InputValueIterator InputValueBegin();
	InputValueIterator InputValueEnd();
	_InputIterator _InputBegin() override;
    _InputIterator _InputEnd() override;
	InputIterator InputBegin() override;
	InputIterator InputEnd() override;
};

template <typename T>
MetaInputValue<T>::InputValueIterator MetaInputValue<T>::InputValueBegin() { return MetaInputValue<T>::InputValueIterator::create(mInputs.begin()); }
template <typename T>
MetaInputValue<T>::InputValueIterator MetaInputValue<T>::InputValueEnd() { return MetaInputValue<T>::InputValueIterator::create(mInputs.end()); }
template <typename T>
MetaInputValue<T>::_InputIterator MetaInputValue<T>::_InputBegin() { return MetaInputValue<T>::_InputIterator::create(mInputs.begin()); }
template <typename T>
MetaInputValue<T>::_InputIterator MetaInputValue<T>::_InputEnd() { return MetaInputValue<T>::_InputIterator::create(mInputs.end()); }
template <typename T>
MetaInputValue<T>::InputIterator MetaInputValue<T>::InputBegin() { return MetaInputValue<T>::InputIterator::create(mInputs.begin()); }
template <typename T>
MetaInputValue<T>::InputIterator MetaInputValue<T>::InputEnd() { return MetaInputValue<T>::InputIterator::create(mInputs.end()); }


}}} // namespace btrack::nodes::system