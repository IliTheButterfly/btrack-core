#pragma once
#include "nodes/MetaNodeInput.h"

namespace btrack { namespace nodes {

template <typename T>
class NodeInputValue;

template <typename T>
class NodeInputArray;

template <typename T>
class MetaNodeInputValue : public MetaNodeInput<T>
{
protected:
	std::vector<NodeInputValue<T>> mInputs;
public:
	MetaNodeInputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeInputValue::MetaNodeInput(_name, NodeItemType::VALUE, _friendlyName, _description) {}

	using _NodeInputIterator = MetaNodeInput<T>::_NodeInputIterator;
	using NodeInputIterator = MetaNodeInput<T>::NodeInputIterator;


	using NodeInputValueType = NodeInputValue<T>;
	using NodeInputValuePtr = NodeInputValue<T>*;
	using NodeInputValueIterator = NodeIterator<NodeInputValueType, NodeInputValuePtr>;
	NodeIteratorAccessorConcrete(NodeInputValueIterator, NodeInputValue, MetaNodeInputValue);

	NodeInputValueIterator NodeInputValueBegin();
	NodeInputValueIterator NodeInputValueEnd();
	_NodeInputIterator _NodeInputBegin() override;
    _NodeInputIterator _NodeInputEnd() override;
	NodeInputIterator NodeInputBegin() override;
	NodeInputIterator NodeInputEnd() override;
};

template <typename T>
MetaNodeInputValue<T>::NodeInputValueIterator MetaNodeInputValue<T>::NodeInputValueBegin() { return MetaNodeInputValue<T>::NodeInputValueIterator::create(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputValueIterator MetaNodeInputValue<T>::NodeInputValueEnd() { return MetaNodeInputValue<T>::NodeInputValueIterator::create(mInputs.end()); }
template <typename T>
MetaNodeInputValue<T>::_NodeInputIterator MetaNodeInputValue<T>::_NodeInputBegin() { return MetaNodeInputValue<T>::_NodeInputIterator::create(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::_NodeInputIterator MetaNodeInputValue<T>::_NodeInputEnd() { return MetaNodeInputValue<T>::_NodeInputIterator::create(mInputs.end()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputIterator MetaNodeInputValue<T>::NodeInputBegin() { return MetaNodeInputValue<T>::NodeInputIterator::create(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputIterator MetaNodeInputValue<T>::NodeInputEnd() { return MetaNodeInputValue<T>::NodeInputIterator::create(mInputs.end()); }


}} // namespace btrack::nodes