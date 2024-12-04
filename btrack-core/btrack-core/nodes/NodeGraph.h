#pragma once
#include "nodes/MetaNode.h"


namespace btrack { namespace nodes {


class NodeGraph
{
public:
	using MetaNodeType = MetaNode;
	using MetaNodePtr = boost::shared_ptr<MetaNode>;
	using MetaNodeIterator = NodeIterator<NodeGraph, MetaNodeType, MetaNodePtr>;

private:
	std::vector<MetaNodePtr> mChildren;

public:
	NodeIteratorAccessorConcrete(MetaNodeIterator, Children);

	MetaNodeIterator ChildrenBegin() { return MetaNodeIterator(mChildren.begin()); }
	MetaNodeIterator ChildrenEnd() { return MetaNodeIterator(mChildren.end()); }

};


}} // btrack::nodes