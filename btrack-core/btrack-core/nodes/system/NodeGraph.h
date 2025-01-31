#pragma once
#include "nodes/system/MetaNode.h"


namespace btrack { namespace nodes { namespace system {


class NodeGraph
{
public:
	using MetaNodeType = MetaNode;
	using MetaNodePtr = std::shared_ptr<MetaNode>;

private:
	std::vector<std::shared_ptr<MetaNode>> mChildren;

public:
	// NodeIteratorAccessorConcrete(MetaNodeIterator, Children, NodeGraph);

	NodeAtConcrete(MetaNode, mChildren)

};


}}} // namespace btrack::nodes::system