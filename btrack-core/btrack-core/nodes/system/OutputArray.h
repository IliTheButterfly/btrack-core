#pragma once
#include "nodes/system/Output.h"
#include "nodes/system/InputArray.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class OutputArray : public Output<T>
{
protected:
	using Sender_t = BroadcastChannel<T>;
	std::shared_ptr<Sender_t> mBroadcast;
	std::vector<InputArray<T>> mChildren;
public:
	OutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			OutputArray::Output(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	using _InputIterator = Output<T>::_InputIterator;
	using InputIterator = Output<T>::InputIterator;
	
	using InputArrayType = InputArray<T>;
	using InputArrayPtr = std::shared_ptr<InputArray<T>>;
	using InputArrayIterator = NodeIterator<InputArrayPtr>;
	NodeIteratorAccessorConcrete(InputArrayIterator, InputArray, OutputArray<T>);


	OutputArray<T>& operator>>(InputArray<T>& input);

	InputArrayIterator InputArrayBegin() { return InputArrayIterator::create(mChildren.begin()); }
	InputArrayIterator InputArrayEnd() { return InputArrayIterator::create(mChildren.end()); }
	_InputIterator _InputBegin() { return _InputIterator::create(mChildren.begin()); }
	_InputIterator _InputEnd() { return _InputIterator::create(mChildren.end()); }
};

}}} // namespace btrack::nodes::system
