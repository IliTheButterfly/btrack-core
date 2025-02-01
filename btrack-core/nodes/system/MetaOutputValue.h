#pragma once
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
	using OutputValuePtr = type_traits::ownership::borrowed_ptr_p<OutputValue<T, I>>;
	// using OutputValueIterator = NodeIterator<OutputValuePtr>;
	// NodeIteratorAccessorConcrete(OutputValueIterator, OutputValue, MetaOutputValue);
protected:
	std::vector<OutputValuePtr> mOutputs;
	std::vector<MetaInputPtr> mChildren;
public:
	MetaOutputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaOutputValue::MetaOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	NodeAtConcrete(MetaInput, mChildren)
	NodeAtWeakCastImpl(_MetaInput, mChildren)
	NodeAtConcrete(OutputValue, mOutputs)
	NodeAtWeakCastImpl(_Output, mOutputs)
	NodeAtWeakCastImpl(Output, mOutputs)
	
	// Implement iterators
	// MetaInputIterator MetaInputBegin() override;
	// MetaInputIterator MetaInputEnd() override;
	// _MetaInputIterator _MetaInputBegin() override;
	// _MetaInputIterator _MetaInputEnd() override;
	// OutputValueIterator OutputValueBegin();
	// OutputValueIterator OutputValueEnd();
	// _OutputIterator _OutputBegin() override;
    // _OutputIterator _OutputEnd() override;
	// OutputIterator OutputBegin() override;
	// OutputIterator OutputEnd() override;


	void addSender(std::weak_ptr<Sender<T, I>> sender) override;
	void removeSender(std::weak_ptr<Sender<T, I>> sender) override;
	void broadcast(typename I::readonlyRef value) override;
	MetaOutputValue<T, I>& operator>>(type_traits::ownership::borrowed_ptr_p<MetaInputValue<T, I>> input);
	MetaOutputValue<T, I>& operator>>(type_traits::ownership::borrowed_ptr_p<MetaInputArray<T, I>> input);
};

// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::MetaInputIterator MetaOutputValue<T, I>::MetaInputBegin() { return MetaOutputValue<T, I>::MetaInputIterator::create(mChildren.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::MetaInputIterator MetaOutputValue<T, I>::MetaInputEnd() { return MetaOutputValue<T, I>::MetaInputIterator::create(mChildren.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::_MetaInputIterator MetaOutputValue<T, I>::_MetaInputBegin() { return MetaOutputValue<T, I>::_MetaInputIterator::create(mChildren.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::_MetaInputIterator MetaOutputValue<T, I>::_MetaInputEnd() { return MetaOutputValue<T, I>::_MetaInputIterator::create(mChildren.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::OutputValueIterator MetaOutputValue<T, I>::OutputValueBegin() { return MetaOutputValue<T, I>::OutputValueIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::OutputValueIterator MetaOutputValue<T, I>::OutputValueEnd() { return MetaOutputValue<T, I>::OutputValueIterator::create(mOutputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::_OutputIterator MetaOutputValue<T, I>::_OutputBegin() { return MetaOutputValue<T, I>::_OutputIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::_OutputIterator MetaOutputValue<T, I>::_OutputEnd() { return MetaOutputValue<T, I>::_OutputIterator::create(mOutputs.end()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::OutputIterator MetaOutputValue<T, I>::OutputBegin() { return MetaOutputValue<T, I>::OutputIterator::create(mOutputs.begin()); }
// template <typename T, ChannelTypeConcept<T> I>
// inline MetaOutputValue<T, I>::OutputIterator MetaOutputValue<T, I>::OutputEnd() { return MetaOutputValue<T, I>::OutputIterator::create(mOutputs.end()); }

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::addSender(std::weak_ptr<Sender<T, I>> sender)
{
	
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::removeSender(std::weak_ptr<Sender<T, I>> sender)
{
}

template <typename T, ChannelTypeConcept<T> I>
inline void MetaOutputValue<T, I>::broadcast(typename I::readonlyRef value)
{
	for (type_traits::ownership::borrowed_ptr_p<OutputValue<T, I>> output : mOutputs)
	{
		// *output.lock() << value;
	}
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputValue<T, I> &MetaOutputValue<T, I>::operator>>(type_traits::ownership::borrowed_ptr_p<MetaInputValue<T, I>> input)
{
	mChildren.push_back(input);
	return *this;
}

template <typename T, ChannelTypeConcept<T> I>
inline MetaOutputValue<T, I> &MetaOutputValue<T, I>::operator>>(type_traits::ownership::borrowed_ptr_p<MetaInputArray<T, I>> input)
{
	mChildren.push_back(input);
	return *this;
}

} // namespace btrack::nodes::system