#pragma once
#include "nodes/MetaNodeInput.h"
#include "nodes/NodeInputValue.h"

namespace btrack { namespace nodes {

template <typename T>
class MetaNodeInputArray : public MetaNodeInput<T>
{
public:
	using _NodeInputIterator = MetaNodeInput<T>::_NodeInputIterator;
	using NodeInputIterator = MetaNodeInput<T>::NodeInputIterator;
	

	using NodeInputValueType = NodeInputValue<T>;
	using NodeInputValuePtr = std::shared_ptr<NodeInputValue<T>>;
	using NodeInputValueIterator = NodeIterator<NodeInputValuePtr>;
	NodeIteratorAccessorConcrete(NodeInputValueIterator, NodeInputValue, MetaNodeInputArray<T>);
protected:
	std::vector<NodeInputValuePtr> mInputs;
public:
	MetaNodeInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeInputArray::MetaNodeInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}

	

	NodeInputValueIterator NodeInputValueBegin();
	NodeInputValueIterator NodeInputValueEnd();
	_NodeInputIterator _NodeInputBegin() override;
    _NodeInputIterator _NodeInputEnd() override;
	NodeInputIterator NodeInputBegin() override;
	NodeInputIterator NodeInputEnd() override;
};

template <typename T> 
inline MetaNodeInputArray<T>::NodeInputValueIterator MetaNodeInputArray<T>::NodeInputValueBegin() { return MetaNodeInputArray<T>::NodeInputValueIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaNodeInputArray<T>::NodeInputValueIterator MetaNodeInputArray<T>::NodeInputValueEnd() { return MetaNodeInputArray<T>::NodeInputValueIterator::create(mInputs.end()); }
template <typename T> 
inline MetaNodeInputArray<T>::_NodeInputIterator MetaNodeInputArray<T>::_NodeInputBegin() { return MetaNodeInputArray<T>::_NodeInputIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaNodeInputArray<T>::_NodeInputIterator MetaNodeInputArray<T>::_NodeInputEnd() { return MetaNodeInputArray<T>::_NodeInputIterator::create(mInputs.end()); }
template <typename T> 
inline MetaNodeInputArray<T>::NodeInputIterator MetaNodeInputArray<T>::NodeInputBegin() { return MetaNodeInputArray<T>::NodeInputIterator::create(mInputs.begin()); }
template <typename T> 
inline MetaNodeInputArray<T>::NodeInputIterator MetaNodeInputArray<T>::NodeInputEnd() { return MetaNodeInputArray<T>::NodeInputIterator::create(mInputs.end()); }

}} // namespace btrack::nodes