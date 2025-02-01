#pragma once
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
	using OutputValuePtr = type_traits::ownership::borrowed_ptr_p<OutputValue<T, I>>;
	// using OutputValueIterator = NodeIterator<OutputValuePtr>;
	// NodeIteratorAccessorConcrete(OutputValueIterator, OutputValue, MetaOutputArray);
	
	using MetaInputArrayType = MetaInputArray<T, I>;
	using MetaInputArrayPtr = type_traits::ownership::borrowed_ptr_p<MetaInputArray<T, I>>;
	// using MetaInputArrayIterator = NodeIterator<MetaInputArrayPtr>;
	// NodeIteratorAccessorConcrete(MetaInputArrayIterator, MetaInputArray, MetaOutputArray);
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
			MetaOutputArray::MetaOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	
	
	// Implement iterators
	NodeAtConcrete(MetaInputArray, mChildren);
	NodeAtWeakCastImpl(MetaInput, mChildren);
	NodeAtWeakCastImpl(_MetaInput, mChildren);
	NodeAtConcrete(OutputValue, mOutputs);
	NodeAtWeakCastImpl(_Output, mOutputs);
	NodeAtWeakCastImpl(Output, mOutputs);

	void addSender(std::weak_ptr<Sender<T, I>> sender) override;
	void removeSender(std::weak_ptr<Sender<T, I>> sender) override;
	void broadcast(typename I::readonlyRef value) override;
	MetaOutputArray<T, I>& operator>>(MetaInputArrayPtr input);
};


// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::MetaInputArrayIterator MetaOutputArray<T, I>::MetaInputArrayBegin() { return MetaOutputArray<T, I>::MetaInputArrayIterator::create(mChildren.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::MetaInputArrayIterator MetaOutputArray<T, I>::MetaInputArrayEnd() { return MetaOutputArray<T, I>::MetaInputArrayIterator::create(mChildren.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::MetaInputIterator MetaOutputArray<T, I>::MetaInputBegin() { return MetaOutputArray<T, I>::MetaInputIterator::create(mChildren.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::MetaInputIterator MetaOutputArray<T, I>::MetaInputEnd() { return MetaOutputArray<T, I>::MetaInputIterator::create(mChildren.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::_MetaInputIterator MetaOutputArray<T, I>::_MetaInputBegin() { return MetaOutputArray<T, I>::_MetaInputIterator::create(mChildren.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::_MetaInputIterator MetaOutputArray<T, I>::_MetaInputEnd() { return MetaOutputArray<T, I>::_MetaInputIterator::create(mChildren.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::OutputValueIterator MetaOutputArray<T, I>::OutputValueBegin() { return MetaOutputArray<T, I>::OutputValueIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::OutputValueIterator MetaOutputArray<T, I>::OutputValueEnd() { return MetaOutputArray<T, I>::OutputValueIterator::create(mOutputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::_OutputIterator MetaOutputArray<T, I>::_OutputBegin() { return MetaOutputArray<T, I>::_OutputIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::_OutputIterator MetaOutputArray<T, I>::_OutputEnd() { return MetaOutputArray<T, I>::_OutputIterator::create(mOutputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::OutputIterator MetaOutputArray<T, I>::OutputBegin() { return MetaOutputArray<T, I>::OutputIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputArray<T, I>::OutputIterator MetaOutputArray<T, I>::OutputEnd() { return MetaOutputArray<T, I>::OutputIterator::create(mOutputs.end()); }

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::broadcast(typename I::readonlyRef value)
{
	mBroadcast->send(value);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::addSender(std::weak_ptr<Sender<T, I>> sender)
{
	mBroadcast->addChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputArray<T, I>::removeSender(std::weak_ptr<Sender<T, I>> sender)
{
	mBroadcast->removeChannel(sender);
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputArray<T, I> &MetaOutputArray<T, I>::operator>>(MetaOutputArray<T, I>::MetaInputArrayPtr input)
{
	mChildren.push_back(input);
	return *this;
}

} // namespace btrack::nodes::system
