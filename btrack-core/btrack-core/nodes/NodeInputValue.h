#pragma once
#include "nodes/NodeInput.h"

namespace btrack { namespace nodes {

template <typename T>
class NodeInputValue : public NodeInput<T>
{
private:
	
public:
	NodeInputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeInputValue::NodeInput(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	

};

}} // namespace btrack::nodes