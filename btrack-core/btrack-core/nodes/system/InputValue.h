#pragma once
#include "nodes/system/Input.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class InputValue : public Input<T>
{
private:
	
public:
	InputValue(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			InputValue::Input(_name, NodeItemType::VALUE, _friendlyName, _description) {}
	

};

}}} // namespace btrack::nodes::system