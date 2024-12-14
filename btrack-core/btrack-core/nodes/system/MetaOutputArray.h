#pragma once
#include "nodes/system/MetaOutput.h"
#include "nodes/system/OutputValue.h"
#include "nodes/system/MetaInputArray.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class MetaOutputArray : public MetaOutput<T>
{
public:
	// Grab iterators from parent
	using _OutputIterator = MetaOutput<T>::_OutputIterator;
	using OutputIterator = MetaOutput<T>::OutputIterator;
	using _MetaInputIterator = MetaOutput<T>::_MetaInputIterator;
	using MetaInputIterator = MetaOutput<T>::MetaInputIterator;

	// Define iterators
	using OutputValueType = OutputValue<T>;
	using OutputValuePtr = std::shared_ptr<OutputValue<T>>;
	using OutputValueIterator = NodeIterator<OutputValuePtr>;
	NodeIteratorAccessorConcrete(OutputValueIterator, OutputValue, MetaOutputArray<T>);
	
	using MetaInputArrayType = MetaInputArray<T>;
	using MetaInputArrayPtr = std::shared_ptr<MetaInputArray<T>>;
	using MetaInputArrayIterator = NodeIterator<MetaInputArrayPtr>;
	NodeIteratorAccessorConcrete(MetaInputArrayIterator, MetaInputArray, MetaOutputArray<T>);
protected:
	using Sender_t = BroadcastChannel<T>;
	std::shared_ptr<Sender_t> mBroadcast;
	std::vector<OutputValuePtr> mOutputs;
	std::vector<MetaInputArrayPtr> mChildren;
public:
	MetaOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaOutputArray::MetaOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	
	
	// Implement iterators
	MetaInputArrayIterator MetaInputArrayBegin();
	MetaInputArrayIterator MetaInputArrayEnd();
	MetaInputIterator MetaInputBegin() override;
	MetaInputIterator MetaInputEnd() override;
	_MetaInputIterator _MetaInputBegin() override;
	_MetaInputIterator _MetaInputEnd() override;
	OutputValueIterator OutputValueBegin();
	OutputValueIterator OutputValueEnd();
	_OutputIterator _OutputBegin() override;
    _OutputIterator _OutputEnd() override;
	OutputIterator OutputBegin() override;
	OutputIterator OutputEnd() override;

	void addSender(std::shared_ptr<Sender<T>> sender) override;
	void removeSender(std::shared_ptr<Sender<T>> sender) override;
	void broadcast(SendParam_t<T> value) override;
	MetaOutputArray<T>& operator>>(MetaInputArrayPtr input);
};


template <typename T>
inline MetaOutputArray<T>::MetaInputArrayIterator MetaOutputArray<T>::MetaInputArrayBegin() { return MetaOutputArray<T>::MetaInputArrayIterator::create(mChildren.begin()); }
template <typename T>
inline MetaOutputArray<T>::MetaInputArrayIterator MetaOutputArray<T>::MetaInputArrayEnd() { return MetaOutputArray<T>::MetaInputArrayIterator::create(mChildren.end()); }
template <typename T>
inline MetaOutputArray<T>::MetaInputIterator MetaOutputArray<T>::MetaInputBegin() { return MetaOutputArray<T>::MetaInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaOutputArray<T>::MetaInputIterator MetaOutputArray<T>::MetaInputEnd() { return MetaOutputArray<T>::MetaInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaOutputArray<T>::_MetaInputIterator MetaOutputArray<T>::_MetaInputBegin() { return MetaOutputArray<T>::_MetaInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaOutputArray<T>::_MetaInputIterator MetaOutputArray<T>::_MetaInputEnd() { return MetaOutputArray<T>::_MetaInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaOutputArray<T>::OutputValueIterator MetaOutputArray<T>::OutputValueBegin() { return MetaOutputArray<T>::OutputValueIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputArray<T>::OutputValueIterator MetaOutputArray<T>::OutputValueEnd() { return MetaOutputArray<T>::OutputValueIterator::create(mOutputs.end()); }
template <typename T>
inline MetaOutputArray<T>::_OutputIterator MetaOutputArray<T>::_OutputBegin() { return MetaOutputArray<T>::_OutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputArray<T>::_OutputIterator MetaOutputArray<T>::_OutputEnd() { return MetaOutputArray<T>::_OutputIterator::create(mOutputs.end()); }
template <typename T>
inline MetaOutputArray<T>::OutputIterator MetaOutputArray<T>::OutputBegin() { return MetaOutputArray<T>::OutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputArray<T>::OutputIterator MetaOutputArray<T>::OutputEnd() { return MetaOutputArray<T>::OutputIterator::create(mOutputs.end()); }

template <typename T>
inline void MetaOutputArray<T>::broadcast(SendParam_t<T> value)
{
	mBroadcast->send(value);
}

template <typename T>
inline void MetaOutputArray<T>::addSender(std::shared_ptr<Sender<T>> sender)
{
	mBroadcast->addChannel(sender);
}

template <typename T>
inline void MetaOutputArray<T>::removeSender(std::shared_ptr<Sender<T>> sender)
{
	mBroadcast->removeChannel(sender);
}

template <typename T>
inline MetaOutputArray<T> &MetaOutputArray<T>::operator>>(MetaOutputArray<T>::MetaInputArrayPtr input)
{
	mChildren.push_back(input);
	return *this;
}

}}} // namespace btrack::nodes::system
