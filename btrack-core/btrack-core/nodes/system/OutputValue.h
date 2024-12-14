#pragma once
#include "nodes/system/Output.h"
#include "nodes/system/InputValue.h"
#include "nodes/system/InputArray.h"


namespace btrack { namespace nodes { namespace system {

template <typename T>
class OutputValue : public Output<T>
{
protected:
	std::vector<Input<T>> mChildren;
public:
	OutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			OutputValue::Output(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	using _InputIterator = Output<T>::_InputIterator;
	using InputIterator = Output<T>::InputIterator;

	InputIterator InputBegin() { return InputIterator::create(mChildren.begin()); }
	InputIterator InputEnd() { return InputIterator::create(mChildren.end()); }
	_InputIterator _InputBegin() { return _InputIterator::create(mChildren.begin()); }
	_InputIterator _InputEnd() { return _InputIterator::create(mChildren.end()); }

	OutputValue<T>& operator>>(InputValue<T>& input);
	OutputValue<T>& operator>>(InputArray<T>& input);

};



}}} // namespace btrack::nodes::system