#ifndef __METANODEIO_H__
#define __METANODEIO_H__



#include <string>
#include "nodes/system/NodeIO.h"


namespace btrack::nodes::system {

class MetaNodeIO : public NodeIO
{
protected:
	MetaNodeIO(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaNodeIO::NodeIO(runner, _name, _nodeType | NodeItemType::META, _friendlyName, _description) {}
};


} // namespace btrack::nodes::system
#endif // __METANODEIO_H__