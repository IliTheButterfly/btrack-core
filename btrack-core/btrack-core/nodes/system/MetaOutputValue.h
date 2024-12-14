#pragma once
#include "nodes/system/MetaOutput.h"
#include "nodes/system/MetaInputValue.h"
#include "nodes/system/MetaInputArray.h"
#include "nodes/system/OutputValue.h"


namespace btrack { namespace nodes { namespace system {

template <typename T>
class MetaOutputValue : public MetaOutput<T>
{
public:
	// Grab iterators from parent
	using _OutputIterator = MetaOutput<T>::_OutputIterator;
	using OutputIterator = MetaOutput<T>::OutputIterator;
	using _MetaInputIterator = MetaOutput<T>::_MetaInputIterator;
	using MetaInputIterator = MetaOutput<T>::MetaInputIterator;
	using MetaInputPtr = MetaOutput<T>::MetaInputPtr;

	// Define iterators
	using OutputValueType = OutputValue<T>;
	using OutputValuePtr = std::shared_ptr<OutputValue<T>>;
	using OutputValueIterator = NodeIterator<OutputValuePtr>;
	NodeIteratorAccessorConcrete(OutputValueIterator, OutputValue, MetaOutputValue<T>);
protected:
	std::vector<OutputValuePtr> mOutputs;
	std::vector<MetaInputPtr> mChildren;
public:
	MetaOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaOutputValue::MetaOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	
	
	// Implement iterators
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
	MetaOutputValue<T>& operator>>(std::shared_ptr<MetaInputValue<T>> input);
	MetaOutputValue<T>& operator>>(std::shared_ptr<MetaInputArray<T>> input);
};

template <typename T>
inline MetaOutputValue<T>::MetaInputIterator MetaOutputValue<T>::MetaInputBegin() { return MetaOutputValue<T>::MetaInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaOutputValue<T>::MetaInputIterator MetaOutputValue<T>::MetaInputEnd() { return MetaOutputValue<T>::MetaInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaOutputValue<T>::_MetaInputIterator MetaOutputValue<T>::_MetaInputBegin() { return MetaOutputValue<T>::_MetaInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaOutputValue<T>::_MetaInputIterator MetaOutputValue<T>::_MetaInputEnd() { return MetaOutputValue<T>::_MetaInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaOutputValue<T>::OutputValueIterator MetaOutputValue<T>::OutputValueBegin() { return MetaOutputValue<T>::OutputValueIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputValue<T>::OutputValueIterator MetaOutputValue<T>::OutputValueEnd() { return MetaOutputValue<T>::OutputValueIterator::create(mOutputs.end()); }
template <typename T>
inline MetaOutputValue<T>::_OutputIterator MetaOutputValue<T>::_OutputBegin() { return MetaOutputValue<T>::_OutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputValue<T>::_OutputIterator MetaOutputValue<T>::_OutputEnd() { return MetaOutputValue<T>::_OutputIterator::create(mOutputs.end()); }
template <typename T>
inline MetaOutputValue<T>::OutputIterator MetaOutputValue<T>::OutputBegin() { return MetaOutputValue<T>::OutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaOutputValue<T>::OutputIterator MetaOutputValue<T>::OutputEnd() { return MetaOutputValue<T>::OutputIterator::create(mOutputs.end()); }

template <typename T>
inline void MetaOutputValue<T>::addSender(std::shared_ptr<Sender<T>> sender)
{
}

template <typename T>
inline void MetaOutputValue<T>::removeSender(std::shared_ptr<Sender<T>> sender)
{
}

template <typename T>
inline void MetaOutputValue<T>::broadcast(SendParam_t<T> value)
{
}

template <typename T>
inline MetaOutputValue<T> &MetaOutputValue<T>::operator>>(std::shared_ptr<MetaInputValue<T>> input)
{
	return *this;
}

template <typename T>
inline MetaOutputValue<T> &MetaOutputValue<T>::operator>>(std::shared_ptr<MetaInputArray<T>> input)
{
	return *this;
}

}}} // namespace btrack::nodes::system