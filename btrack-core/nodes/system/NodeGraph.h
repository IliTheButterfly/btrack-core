#ifndef __NODEGRAPH_H__
#define __NODEGRAPH_H__


#include "nodes/system/MetaNode.h"


namespace btrack::nodes::system {


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


} // namespace btrack::nodes::system
#endif // __NODEGRAPH_H__