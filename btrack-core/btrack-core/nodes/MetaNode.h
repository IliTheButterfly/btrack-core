#pragma once

#include <vector>
#include <memory>
#include "nodes/NodeIO.h"

namespace btrack { namespace nodes
{

class Node;

class _MetaNode : public _Node
{
public:
	// using NodeIOIterator = _Node::NodeIOIterator;

	using NodeType = Node;
	using NodePtr = std::shared_ptr<Node>;
	using NodeIt = NodeIterator<_MetaNode, NodeType, NodePtr>;
	NodeIteratorAccessor(NodeIt, Node);
protected:

public:
	_MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaNode::_Node(_name, NodeItemType::META | NodeItemType::NODE, _friendlyName, _description) {}




};

class MetaNode : public _MetaNode
{
protected:
	std::vector<NodePtr> mNodes;
public:
	MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNode::_MetaNode(_name, _friendlyName, _description) {}
	// using NodeIOIterator = _MetaNode::NodeIOIterator;
	using NodeIt = _MetaNode::NodeIt;

	NodeIt NodeBegin() override { return NodeIt(mNodes.begin()); }
	NodeIt NodeEnd() override { return NodeIt(mNodes.end()); }
};

}} // btrack::nodes