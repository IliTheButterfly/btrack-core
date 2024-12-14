#pragma once
#include "nodes/MetaNodeOutput.h"
#include "nodes/NodeOutputValue.h"
#include "nodes/MetaNodeInputArray.h"

namespace btrack { namespace nodes {

template <typename T>
class MetaNodeOutputArray : public MetaNodeOutput<T>
{
public:
	// Grab iterators from parent
	using _NodeOutputIterator = MetaNodeOutput<T>::_NodeOutputIterator;
	using NodeOutputIterator = MetaNodeOutput<T>::NodeOutputIterator;
	using _MetaNodeInputIterator = MetaNodeOutput<T>::_MetaNodeInputIterator;
	using MetaNodeInputIterator = MetaNodeOutput<T>::MetaNodeInputIterator;

	// Define iterators
	using NodeOutputValueType = NodeOutputValue<T>;
	using NodeOutputValuePtr = std::shared_ptr<NodeOutputValue<T>>;
	using NodeOutputValueIterator = NodeIterator<NodeOutputValuePtr>;
	NodeIteratorAccessorConcrete(NodeOutputValueIterator, NodeOutputValue, MetaNodeOutputArray<T>);
	
	using MetaNodeInputArrayType = MetaNodeInputArray<T>;
	using MetaNodeInputArrayPtr = std::shared_ptr<MetaNodeInputArray<T>>;
	using MetaNodeInputArrayIterator = NodeIterator<MetaNodeInputArrayPtr>;
	NodeIteratorAccessorConcrete(MetaNodeInputArrayIterator, MetaNodeInputArray, MetaNodeOutputArray<T>);
protected:
	using Sender_t = BroadcastChannel<T>;
	std::shared_ptr<Sender_t> mBroadcast;
	std::vector<NodeOutputValuePtr> mOutputs;
	std::vector<MetaNodeInputArrayPtr> mChildren;
public:
	MetaNodeOutputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeOutputArray::MetaNodeOutput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	
	
	// Implement iterators
	MetaNodeInputArrayIterator MetaNodeInputArrayBegin();
	MetaNodeInputArrayIterator MetaNodeInputArrayEnd();
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
	MetaNodeOutputArray<T>& operator>>(MetaNodeInputArrayPtr input);
};


template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputArrayIterator MetaNodeOutputArray<T>::MetaNodeInputArrayBegin() { return MetaNodeOutputArray<T>::MetaNodeInputArrayIterator::create(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputArrayIterator MetaNodeOutputArray<T>::MetaNodeInputArrayEnd() { return MetaNodeOutputArray<T>::MetaNodeInputArrayIterator::create(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputIterator MetaNodeOutputArray<T>::MetaNodeInputBegin() { return MetaNodeOutputArray<T>::MetaNodeInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputIterator MetaNodeOutputArray<T>::MetaNodeInputEnd() { return MetaNodeOutputArray<T>::MetaNodeInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::_MetaNodeInputIterator MetaNodeOutputArray<T>::_MetaNodeInputBegin() { return MetaNodeOutputArray<T>::_MetaNodeInputIterator::create(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::_MetaNodeInputIterator MetaNodeOutputArray<T>::_MetaNodeInputEnd() { return MetaNodeOutputArray<T>::_MetaNodeInputIterator::create(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputValueIterator MetaNodeOutputArray<T>::NodeOutputValueBegin() { return MetaNodeOutputArray<T>::NodeOutputValueIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputValueIterator MetaNodeOutputArray<T>::NodeOutputValueEnd() { return MetaNodeOutputArray<T>::NodeOutputValueIterator::create(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::_NodeOutputIterator MetaNodeOutputArray<T>::_NodeOutputBegin() { return MetaNodeOutputArray<T>::_NodeOutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::_NodeOutputIterator MetaNodeOutputArray<T>::_NodeOutputEnd() { return MetaNodeOutputArray<T>::_NodeOutputIterator::create(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputIterator MetaNodeOutputArray<T>::NodeOutputBegin() { return MetaNodeOutputArray<T>::NodeOutputIterator::create(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputIterator MetaNodeOutputArray<T>::NodeOutputEnd() { return MetaNodeOutputArray<T>::NodeOutputIterator::create(mOutputs.end()); }

template <typename T>
inline void MetaNodeOutputArray<T>::broadcast(SendParam_t<T> value)
{
	mBroadcast->send(value);
}

template <typename T>
inline void MetaNodeOutputArray<T>::addSender(std::shared_ptr<Sender<T>> sender)
{
	mBroadcast->addChannel(sender);
}

template <typename T>
inline void MetaNodeOutputArray<T>::removeSender(std::shared_ptr<Sender<T>> sender)
{
	mBroadcast->removeChannel(sender);
}

template <typename T>
inline MetaNodeOutputArray<T> &MetaNodeOutputArray<T>::operator>>(MetaNodeOutputArray<T>::MetaNodeInputArrayPtr input)
{
	mChildren.push_back(input);
	return *this;
}

}} // btrack::nodes
