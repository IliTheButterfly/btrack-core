#ifndef __METAOUTPUTARRAY_H__
#define __METAOUTPUTARRAY_H__


#include "nodes/system/MetaOutput.h"
#include "nodes/system/OutputValue.h"
#include "nodes/system/MetaInputArray.h"

namespace btrack::nodes::system {


template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaOutputArray : public MetaOutput<T, I>
{
public:
	// Grab iterators from parent
	using _OutputType = MetaOutput<T, I>::_OutputType;
	using _OutputPtr = MetaOutput<T, I>::_OutputPtr;
	using OutputType = MetaOutput<T, I>::OutputType;
	using OutputPtr = MetaOutput<T, I>::OutputPtr;
	using _MetaInputType = MetaOutput<T, I>::_MetaInputType;
	using _MetaInputPtr = MetaOutput<T, I>::_MetaInputPtr;
	using MetaInputType = MetaOutput<T, I>::MetaInputType;
	using MetaInputPtr = MetaOutput<T, I>::MetaInputPtr;

	// Define iterators
	using OutputValueType = OutputValue<T, I>;
	using OutputValuePtr = borrowed_ptr_p<OutputValue<T, I>>;
	
	using MetaInputArrayType = MetaInputArray<T, I>;
	using MetaInputArrayPtr = borrowed_ptr_p<MetaInputArray<T, I>>;
protected:
	using Sender_t = BroadcastChannel<T, I>;
	std::shared_ptr<Sender_t> mBroadcast;
	std::vector<OutputValuePtr> mOutputs;
	std::vector<MetaInputArrayPtr> mChildren;
public:
	MetaOutputArray(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) :
			MetaOutputArray::MetaOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) 
			{
				mBroadcast = std::make_shared<Sender_t>();
			}

	
	
	// Implement iterators
	NodeAtWeakConcrete(MetaInputArray, mChildren);
	NodeAtWeakCastImpl(MetaInput, mChildren);
	NodeAtWeakCastImpl(_MetaInput, mChildren);
	NodeAtWeakConcrete(OutputValue, mOutputs);
	NodeAtWeakCastImpl(_Output, mOutputs);
	NodeAtWeakCastImpl(Output, mOutputs);

	void addSender(std::shared_ptr<Sender<T, I>> sender) override;
	void removeSender(std::shared_ptr<Sender<T, I>> sender) override;
	void broadcast(typename I::readonlyRef value) override;
	MetaOutputArray<T, I>& operator>>(std::shared_ptr<MetaInputArray<T, I>> input);
	MetaOutputArray<T, I>& operator<<(typename I::readonlyRef value);
	bool connectTo(std::shared_ptr<_MetaInput> other) override;
	bool disconnectFrom(std::shared_ptr<_MetaInput> other) override;

	void attach(std::shared_ptr<_Output> output) override;
	void detach(std::shared_ptr<_Output> output) override;
};

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::broadcast(typename I::readonlyRef value)
{
	mBroadcast->send(value);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::addSender(std::shared_ptr<Sender<T, I>> sender)
{
	mBroadcast->addChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::removeSender(std::shared_ptr<Sender<T, I>> sender)
{
	mBroadcast->removeChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputArray<T, I> &MetaOutputArray<T, I>::operator>>(std::shared_ptr<MetaInputArray<T, I>> input)
{
	connectTo(input);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline bool MetaOutputArray<T, I>::connectTo(std::shared_ptr<_MetaInput> other)
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
inline MetaOutputArray<T, I> &MetaOutputArray<T, I>::operator<<(typename I::readonlyRef value)
{
	this->broadcast(value);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline bool MetaOutputArray<T, I>::disconnectFrom(std::shared_ptr<_MetaInput> other)
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
inline void MetaOutputArray<T, I>::attach(std::shared_ptr<_Output> output)
{
	if (std::find_if(mOutputs.begin(), mOutputs.end(), [&](OutputValuePtr i){ return !i.expired() && i.lock() == output; }) == mOutputs.end())
	{
		mOutputs.emplace_back(std::dynamic_pointer_cast<OutputValue<T, I>>(output));
	}
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::detach(std::shared_ptr<_Output> output)
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

#endif // __METAOUTPUTARRAY_H__