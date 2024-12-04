#pragma once

#include <string>
#include "nodes/NodeIO.h"


namespace btrack { namespace nodes
{

class MetaNodeIO : public NodeIO
{
protected:
	MetaNodeIO(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNodeIO::NodeIO(_name, _nodeType | NodeItemType::META, _friendlyName, _description) {}
};


}} // btrack::nodes