#pragma once
#include "nodes/NodeInput.h"

namespace btrack { namespace nodes {

template <typename T>
class NodeInputArray : public NodeInput<T>
{
public:
	NodeInputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeInputArray::NodeInput(_name, NodeItemType::ARRAY, _friendlyName, _description) {}
};

}} // namespace btrack::nodes