#ifndef __OUTPUT_H__
#define __OUTPUT_H__


#include <vector>
#include <iterator>
#include "nodes/system/NodeIO.h"

namespace btrack::nodes::system {

class _Input;

template <typename T, ChannelTypeConcept<T> I>
class Input;

class _Output : public NodeIO
{
protected:
	_Output(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Output::NodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _InputType = _Input;
    using _InputPtr = type_traits::ownership::borrowed_ptr_p<_Input>;
	// using _InputIterator = NodeIterator<_InputPtr>;
	// NodeIteratorAccessor(_InputIterator, _Input, _Output);
	bool canConnectTo(const std::shared_ptr<_Input> other) const {
		return convertibleTo(other->dataType()) && 
			(
				(isArray() && other->isArray()) ||
				isValue()
			);
	}
	
	NodeAtVirtual(_Input)
	virtual ~_Output() = default;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Output : public _Output
{
public:
    using ChannelType = BroadcastChannel<T, I>;
    using ChannelPtr = std::shared_ptr<ChannelType>;
protected:
	ChannelPtr mChannel;
	Output(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Output::_Output(_name, _nodeType, _friendlyName, _description), mChannel{std::make_shared<ChannelType>()} {}
public:
	// using _InputIterator = _Output::_InputIterator;

    using InputType = Input<T, I>;
    using InputPtr = type_traits::ownership::borrowed_ptr_p<Input<T, I>>;
	// using InputIterator = NodeIterator<InputPtr>;
	// NodeIteratorAccessor(InputIterator, Input, Output);
	NodeAtVirtual(Input)

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual void addSender(std::shared_ptr<Sender<T, I>> sender) = 0;
	virtual void removeSender(std::shared_ptr<Sender<T, I>> sender) = 0;
	virtual void broadcast(typename I::readonlyRef value) = 0;
	friend class Input<T, I>;
	virtual ~Output() = default;
};

} // namespace btrack::nodes::system
#endif // __OUTPUT_H__