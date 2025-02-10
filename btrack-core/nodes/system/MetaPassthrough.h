#ifndef __METAPASSTHROUGH_H__
#define __METAPASSTHROUGH_H__

#include "nodes/system/MetaOutputArray.h"
#include "nodes/system/MetaOutputValue.h"

namespace btrack::nodes::system {

class _MetaPassthrough : public MetaNodeIO
{
protected:
	_MetaPassthrough(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaPassthrough::MetaNodeIO(_name, _nodeType | NodeItemType::INPUT | NodeItemType::OUTPUT, _friendlyName, _description) {}
};

}

#endif // __METAPASSTHROUGH_H__