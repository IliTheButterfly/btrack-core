#pragma once

#include <string>
#include "nodes/system/NodeIO.h"


namespace btrack::nodes::system {

class MetaNodeIO : public NodeIO
{
protected:
	MetaNodeIO(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaNodeIO::NodeIO(_name, _nodeType | NodeItemType::META, _friendlyName, _description) {}
};


} // namespace btrack::nodes::system