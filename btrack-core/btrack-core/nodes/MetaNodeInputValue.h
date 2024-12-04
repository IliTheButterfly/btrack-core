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
	std::vector<boost::shared_ptr<NodeInputValue<T>>> mInputs;
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
	using NodeInputValuePtr = boost::shared_ptr<NodeInputValue<T>>;
	using NodeInputValueIterator = NodeIterator<MetaNodeInputValue, NodeInputValueType, NodeInputValuePtr>;
	NodeIteratorAccessorConcrete(NodeInputValueIterator, NodeInputValue);

	NodeInputValueIterator NodeInputValueBegin();
	NodeInputValueIterator NodeInputValueEnd();
	_NodeInputIterator _NodeInputBegin() override;
    _NodeInputIterator _NodeInputEnd() override;
	NodeInputIterator NodeInputBegin() override;
	NodeInputIterator NodeInputEnd() override;
};


}} // namespace btrack::nodes