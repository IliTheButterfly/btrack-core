#ifndef __METAOUTPUTVALUE_H__
#define __METAOUTPUTVALUE_H__


#include "nodes/system/MetaOutput.h"
#include "nodes/system/MetaInputValue.h"
#include "nodes/system/MetaInputArray.h"
#include "nodes/system/OutputValue.h"


namespace btrack::nodes::system {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaOutputValue : public MetaOutput<T, I>
{
public:
	// Grab iterators from parent
	using _OutputPtr = MetaOutput<T, I>::_OutputPtr;
	using OutputPtr = MetaOutput<T, I>::OutputPtr;
	using _MetaInputPtr = MetaOutput<T, I>::_MetaInputPtr;
	using MetaInputPtr = MetaOutput<T, I>::MetaInputPtr;

	// Define iterators
	using OutputValueType = OutputValue<T, I>;
	using OutputValuePtr = borrowed_ptr_p<OutputValue<T, I>>;
protected:
	struct Protected { explicit Protected() = default; };
	using Sender_t = BroadcastChannel<T, I>;
	std::shared_ptr<Sender_t> mBroadcast;
	std::vector<OutputValuePtr> mOutputs;
	std::vector<MetaInputPtr> mChildren;
public:
	MetaOutputValue(
		Protected _,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaOutputValue::MetaOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	static std::shared_ptr<MetaOutputValue> create(
		std::shared_ptr<_Node> _parent,
		std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		)
	{
		auto ret = std::make_shared<MetaOutputValue>(Protected(), _name, _friendlyName, _description);
		ret->mParent = _parent;
		ret->mObserver = _observer;
		return ret;
	}

	NodeAtWeakConcrete(MetaInput, mChildren)
	NodeAtWeakCastImpl(_MetaInput, mChildren)
	NodeAtWeakConcrete(OutputValue, mOutputs)
	NodeAtWeakCastImpl(_Output, mOutputs)
	NodeAtWeakCastImpl(Output, mOutputs)
	

	void addSender(std::shared_ptr<Sender<T, I>> sender) override;
	void removeSender(std::shared_ptr<Sender<T, I>> sender) override;
	void broadcast(typename I::readonlyRef value) override;
	MetaOutputValue<T, I>& operator>>(std::shared_ptr<MetaInputValue<T, I>> input);
	MetaOutputValue<T, I>& operator>>(std::shared_ptr<MetaInputArray<T, I>> input);
	MetaOutputValue<T, I>& operator<<(typename I::readonlyRef value);


	bool connectTo(std::shared_ptr<_MetaInput> other) override;
	bool disconnectFrom(std::shared_ptr<_MetaInput> other) override;

	void attach(std::shared_ptr<_Output> output) override;
	void detach(std::shared_ptr<_Output> output) override;
};

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::addSender(std::shared_ptr<Sender<T, I>> sender)
{
	mBroadcast->addChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::removeSender(std::shared_ptr<Sender<T, I>> sender)
{
	mBroadcast->removeChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::broadcast(typename I::readonlyRef value)
{
	mBroadcast->send(value);
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputValue<T, I> &MetaOutputValue<T, I>::operator>>(std::shared_ptr<MetaInputValue<T, I>> input)
{
	this->connectTo(input);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputValue<T, I> &MetaOutputValue<T, I>::operator>>(std::shared_ptr<MetaInputArray<T, I>> input)
{
	this->connectTo(input);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputValue<T, I> &MetaOutputValue<T, I>::operator<<(typename I::readonlyRef value)
{
	this->broadcast(value);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline bool MetaOutputValue<T, I>::connectTo(std::shared_ptr<_MetaInput> other)
{
	if (!this->canConnectTo(other)) return false;
	if (this->dataType() == other->dataType())
	{
		mChildren.push_back(std::dynamic_pointer_cast<MetaInputArray<T, I>>(other));
		IF_WEAK_VALID(this->mObserver)->addConnection(this->shared_from_this(), other);
		return true;
	}
	return false;
}

template <typename T, ChannelTypeConcept<T> I>
inline bool MetaOutputValue<T, I>::disconnectFrom(std::shared_ptr<_MetaInput> other)
{
	for (auto i = mChildren.begin(); i != mChildren.end(); )
	{
		if (i->expired() || !i->lock()) 
		{
			mChildren.erase(i);
			IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), other);
			continue;
		}
		if (i->lock()->uuid() == other->uuid()) 
		{
			mChildren.erase(i);
			IF_WEAK_VALID(this->mObserver)->removeConnection(this->shared_from_this(), other);
			return true;
		}
		++i;
	}
	return false;
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::attach(std::shared_ptr<_Output> output)
{
	if (std::find_if(mOutputs.begin(), mOutputs.end(), [&](OutputValuePtr i){ return !i.expired() && i.lock() == output; }) == mOutputs.end())
	{
		mOutputs.emplace_back(std::dynamic_pointer_cast<OutputValue<T, I>>(output));
	}
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::detach(std::shared_ptr<_Output> output)
{
	for (auto i = mOutputs.begin(); i != mOutputs.end(); )
	{
		if (i->expired() || !i->lock()) 
		{
			mOutputs.erase(i);
			continue;
		}
		if (i->lock()->uuid() == output->uuid()) 
		{
			mOutputs.erase(i);
			return;
		}
		++i;
	}
}

} // namespace btrack::nodes::system
#endif // __METAOUTPUTVALUE_H__