#pragma once
#include "nodes/MetaNodeOutput.h"
#include "nodes/MetaNodeInputValue.h"
#include "nodes/MetaNodeInputArray.h"
#include "nodes/NodeOutputValue.h"


namespace btrack { namespace nodes {

template <typename T>
class MetaNodeOutputValue : public MetaNodeOutput<T>
{
public:
	// Grab iterators from parent
	using _NodeOutputIterator = MetaNodeOutput<T>::_NodeOutputIterator;
	using NodeOutputIterator = MetaNodeOutput<T>::NodeOutputIterator;
	using _MetaNodeInputIterator = MetaNodeOutput<T>::_MetaNodeInputIterator;
	using MetaNodeInputIterator = MetaNodeOutput<T>::MetaNodeInputIterator;
	using MetaNodeInputPtr = MetaNodeOutput<T>::MetaNodeInputPtr;

	// Define iterators
	using NodeOutputValueType = NodeOutputValue<T>;
	using NodeOutputValuePtr = std::shared_ptr<NodeOutputValue<T>>;
	using NodeOutputValueIterator = NodeIterator<NodeOutputValuePtr>;
	NodeIteratorAccessorConcrete(NodeOutputValueIterator, NodeOutputValue, MetaNodeOutputValue<T>);
protected:
	std::vector<NodeOutputValuePtr> mOutputs;
	std::vector<MetaNodeInputPtr> mChildren;
public:
	MetaNodeOutputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeOutputValue::MetaNodeOutput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	
	
	
	// Implement iterators
	MetaNodeInputIterator MetaNodeInputBegin() override;
	MetaNodeInputIterator MetaNodeInputEnd() override;
	_MetaNodeInputIterator _MetaNodeInputBegin() override;
	_MetaNodeInputIterator _MetaNodeInputEnd() override;
	NodeOutputValueIterator NodeOutputValueBegin();
	NodeOutputValueIterator NodeOutputValueEnd();
	_NodeOutputIterator _NodeOutputBegin() override;
    _NodeOutputIterator _NodeOutputEnd() override;
	NodeOutputIterator NodeOutputBegin() override;
	NodeOutputIterator NodeOutputEnd() override;


	void addSender(std::shared_ptr<Sender<T>> sender) override;
	void removeSender(std::shared_ptr<Sender<T>> sender) override;
	void broadcast(SendParam_t<T> value) override;
	MetaNodeOutputValue<T>& operator>>(std::shared_ptr<MetaNodeInputValue<T>> input);
	MetaNodeOutputValue<T>& operator>>(std::shared_ptr<MetaNodeInputArray<T>> input);
};

template <typename T>
inline MetaNodeOutputValue<T>::MetaNodeInputIterator MetaNodeOutputValue<T>::MetaNodeInputBegin() { return MetaNodeOutputValue<T>::MetaNodeInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::MetaNodeInputIterator MetaNodeOutputValue<T>::MetaNodeInputEnd() { return MetaNodeOutputValue<T>::MetaNodeInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::_MetaNodeInputIterator MetaNodeOutputValue<T>::_MetaNodeInputBegin() { return MetaNodeOutputValue<T>::_MetaNodeInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::_MetaNodeInputIterator MetaNodeOutputValue<T>::_MetaNodeInputEnd() { return MetaNodeOutputValue<T>::_MetaNodeInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputValueIterator MetaNodeOutputValue<T>::NodeOutputValueBegin() { return MetaNodeOutputValue<T>::NodeOutputValueIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputValueIterator MetaNodeOutputValue<T>::NodeOutputValueEnd() { return MetaNodeOutputValue<T>::NodeOutputValueIterator::create(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::_NodeOutputIterator MetaNodeOutputValue<T>::_NodeOutputBegin() { return MetaNodeOutputValue<T>::_NodeOutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::_NodeOutputIterator MetaNodeOutputValue<T>::_NodeOutputEnd() { return MetaNodeOutputValue<T>::_NodeOutputIterator::create(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputIterator MetaNodeOutputValue<T>::NodeOutputBegin() { return MetaNodeOutputValue<T>::NodeOutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputIterator MetaNodeOutputValue<T>::NodeOutputEnd() { return MetaNodeOutputValue<T>::NodeOutputIterator::create(mOutputs.end()); }

template <typename T>
inline void MetaNodeOutputValue<T>::addSender(std::shared_ptr<Sender<T>> sender)
{
}

template <typename T>
inline void MetaNodeOutputValue<T>::removeSender(std::shared_ptr<Sender<T>> sender)
{
}

template <typename T>
inline void MetaNodeOutputValue<T>::broadcast(SendParam_t<T> value)
{
}

template <typename T>
inline MetaNodeOutputValue<T> &MetaNodeOutputValue<T>::operator>>(std::shared_ptr<MetaNodeInputValue<T>> input)
{
	return *this;
}

template <typename T>
inline MetaNodeOutputValue<T> &MetaNodeOutputValue<T>::operator>>(std::shared_ptr<MetaNodeInputArray<T>> input)
{
	return *this;
}

}} // namespace btrack::nodes