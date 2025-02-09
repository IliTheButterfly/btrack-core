#ifndef __OUTPUTVALUE_H__
#define __OUTPUTVALUE_H__


#include "nodes/system/Output.h"
#include "nodes/system/InputValue.h"

namespace btrack::nodes::system {

using namespace type_traits::ownership;

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class OutputValue : public Output<T, I>
{
public:
	using _InputPtr = typename Output<T, I>::_InputPtr;
	using InputPtr = typename Output<T, I>::InputPtr;

	using InputValueType = InputValue<T, I>;
    using InputValuePtr = borrowed_ptr_p<InputValue<T, I>>;
protected:
	struct Protected { explicit Protected() = default; };
	std::vector<InputValuePtr> mChildren{};
public:
	OutputValue(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			OutputValue::Output(_name, NodeItemType::VALUE, _friendlyName, _description) {}

	static std::shared_ptr<OutputValue> create(
		std::shared_ptr<_Node> _parent,
		std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
	)
	{
		auto ret = std::make_shared<OutputValue>(Protected(), _name, _friendlyName, _description);
		ret->mParent = _parent;
		ret->mObserver = _observer;
		return ret;
	}

	NodeAtWeakCastImpl(_Input, mChildren)
	NodeAtWeakCastImpl(Input, mChildren)
	NodeAtConcrete(InputValue, mChildren)

	void addSender(std::shared_ptr<Sender<T, I>> sender) override
	{
		this->mChannel->addChannel(sender);
	}
	void removeSender(std::shared_ptr<Sender<T, I>> sender) override
	{
		this->mChannel->removeChannel(sender);
	}
	void broadcast(typename I::readonlyRef value) override
	{
		this->mChannel->send(value);
	}
	bool connectTo(std::shared_ptr<_Input> other)
	{
		if (!this->canConnectTo(other)) return false;
		if (this->dataType() == other->dataType())
		{
			auto o = std::dynamic_pointer_cast<InputValue<T, I>>(other);
			mChildren.push_back(o);
			addSender(o->mChannel);
			IF_WEAK_VALID(this->mObserver)->addConnection(this->shared_from_this(), other);
			return true;
		}
		return false;
	}

	bool disconnectFrom(std::shared_ptr<_Input> other)
	{
		for (auto i = mChildren.begin(); i != mChildren.end(); )
		{
			if (i->expired() || !i->lock()) 
			{
				mChildren.erase(i);
				removeSender(i->lock()->mChannel);
				IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), other);
				continue;
			}
			if (i->lock()->uuid() == other->uuid()) 
			{
				mChildren.erase(i);
				removeSender(i->lock()->mChannel);
				IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), other);
				return true;
			}
			++i;
		}
		return false;
	}


	OutputValue<T, I>& operator>>(std::shared_ptr<InputValue<T, I>> input) 
	{
		connectTo(input);
		return *this;
	}
	OutputValue<T, I>& operator<<(typename I::readonlyRef value)
	{
		std::cout << "Sending <<" << std::endl;
		for (auto i = mChildren.begin(); i != mChildren.end(); )
		{
			if (i->expired() || !i->lock()) 
			{
				mChildren.erase(i);
				removeSender(i->lock()->mChannel);
				IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), i->lock());
				continue;
			}
			i->lock()->mChannel->send(value);
			++i;
		}
		return *this;
	}


	virtual ~OutputValue()
	{
		for (auto i : mChildren)
		{
			if (i.expired() || !i.lock()) continue;
			try
			{
				IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), i.lock());
			}
			catch(const std::bad_weak_ptr& e)
			{
			}
			removeSender(i.lock()->mChannel);
			
		}
	}
};

} // namespace btrack::nodes::system
#endif // __OUTPUTVALUE_H__