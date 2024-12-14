#pragma once
#include "nodes/system/Input.h"

namespace btrack { namespace nodes { namespace system {

template <typename T>
class InputArray : public Input<T>
{
public:
	InputArray(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			InputArray::Input(_name, NodeItemType::ARRAY, _friendlyName, _description) {}
};

}}} // namespace btrack::nodes::system