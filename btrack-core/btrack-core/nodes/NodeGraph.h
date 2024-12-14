#pragma once
#include "nodes/MetaNode.h"


namespace btrack { namespace nodes {


class NodeGraph
{
public:
	using MetaNodeType = MetaNode;
	using MetaNodePtr = std::shared_ptr<MetaNode>;
	using MetaNodeIterator = NodeIterator<MetaNodePtr>;

private:
	std::vector<MetaNode> mChildren;

public:
	NodeIteratorAccessorConcrete(MetaNodeIterator, Children, NodeGraph);

	MetaNodeIterator ChildrenBegin() { return MetaNodeIterator::create(mChildren.begin()); }
	MetaNodeIterator ChildrenEnd() { return MetaNodeIterator::create(mChildren.end()); }

};


}} // btrack::nodes