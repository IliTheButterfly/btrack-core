#pragma once
#include "nodes/system/Output.h"
#include "nodes/system/InputValue.h"

namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class OutputValue : public Output<T, I>
{
public:
	using _InputPtr = typename Output<T, I>::_InputPtr;
	using InputPtr = typename Output<T, I>::InputPtr;

	using InputValueType = InputValue<T, I>;
    using InputValuePtr = type_traits::ownership::borrowed_ptr_p<InputValue<T, I>>;
protected:
	std::vector<InputValuePtr> mChildren{};
public:
	OutputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			OutputValue::Output(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	// using InputValueIterator = NodeIterator<InputValuePtr>;
	// NodeIteratorAccessorConcrete(InputValueIterator, InputValue, OutputValue);

	NodeAtWeakCastImpl(_Input, mChildren)
	NodeAtWeakCastImpl(Input, mChildren)
	NodeAtConcrete(InputValue, mChildren)

	// InputValueIterator InputValueBegin() { return InputValueIterator::create(mChildren.begin()); }
	// InputValueIterator InputValueEnd() { return InputValueIterator::create(mChildren.end()); }
	// InputIterator InputBegin() { return InputIterator::create(mChildren.begin()); }
	// InputIterator InputEnd() { return InputIterator::create(mChildren.end()); }
	// _InputIterator _InputBegin() { return _InputIterator::create(mChildren.begin()); }
	// _InputIterator _InputEnd() { return _InputIterator::create(mChildren.end()); }


	OutputValue<T, I>& operator>>(std::weak_ptr<InputValue<T, I>> input) 
	{ 
		mChildren.push_back(input.lock());
		std::cout << "Size " << mChildren.size() << std::endl;
		return *this;
	}
	OutputValue<T, I>& operator<<(typename I::readonlyRef value)
	{
		std::cout << "Sending <<" << std::endl;
		int i = 0;
		int invalid = -1;
		for (std::weak_ptr<InputValue<T, I>> input : mChildren)
		{
			std::cout << "Sending to " << i << std::endl;

			if (!input.expired()) input.lock()->chan->send(value);
			else 
			{
				invalid = i;
				std::cout << "Expired " << i << std::endl;

			}
			++i;
		}
		if (invalid != -1)
		{
			mChildren.erase(mChildren.begin() + invalid);
		}
		return *this;
	}


};

} // namespace btrack::nodes::system