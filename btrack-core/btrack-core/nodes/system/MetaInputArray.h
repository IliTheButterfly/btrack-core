#pragma once
#include "nodes/system/MetaInput.h"
#include "nodes/system/InputValue.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class MetaInputArray : public MetaInput<T>
{
public:
	using _InputIterator = MetaInput<T>::_InputIterator;
	using InputIterator = MetaInput<T>::InputIterator;
	

	using InputValueType = InputValue<T>;
	using InputValuePtr = std::shared_ptr<InputValue<T>>;
	using InputValueIterator = NodeIterator<InputValuePtr>;
	NodeIteratorAccessorConcrete(InputValueIterator, InputValue, MetaInputArray<T>);
protected:
	std::vector<InputValuePtr> mInputs;
public:
	MetaInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaInputArray::MetaInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	

	InputValueIterator InputValueBegin();
	InputValueIterator InputValueEnd();
	_InputIterator _InputBegin() override;
    _InputIterator _InputEnd() override;
	InputIterator InputBegin() override;
	InputIterator InputEnd() override;
};

template <typename T> 
inline MetaInputArray<T>::InputValueIterator MetaInputArray<T>::InputValueBegin() { return MetaInputArray<T>::InputValueIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaInputArray<T>::InputValueIterator MetaInputArray<T>::InputValueEnd() { return MetaInputArray<T>::InputValueIterator::create(mInputs.end()); }
template <typename T> 
inline MetaInputArray<T>::_InputIterator MetaInputArray<T>::_InputBegin() { return MetaInputArray<T>::_InputIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaInputArray<T>::_InputIterator MetaInputArray<T>::_InputEnd() { return MetaInputArray<T>::_InputIterator::create(mInputs.end()); }
template <typename T> 
inline MetaInputArray<T>::InputIterator MetaInputArray<T>::InputBegin() { return MetaInputArray<T>::InputIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaInputArray<T>::InputIterator MetaInputArray<T>::InputEnd() { return MetaInputArray<T>::InputIterator::create(mInputs.end()); }

}}} // namespace btrack::nodes::system