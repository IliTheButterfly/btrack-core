#pragma once

#include <vector>
#include <memory>
#include "nodes/NodeIO.h"
#include "nodes/Node.h"

namespace btrack { namespace nodes
{

class _MetaNode : public _Node
{
public:
	// using NodeIOIterator = _Node::NodeIOIterator;

	using NodeType = Node;
	using NodePtr = Node*;
	using NodeIt = NodeIterator<NodeType, NodePtr>;
	NodeIteratorAccessor(NodeIt, Node, _MetaNode);
protected:

public:
	_MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaNode::_Node(_name, NodeItemType::META | NodeItemType::NODE, _friendlyName, _description) {}
private:
	virtual void generate(int count) = 0;
	friend class NodeGraph;
};

class MetaNode : public _MetaNode
{
protected:
	std::vector<Node> mNodes;
public:
	MetaNode(
		const std::string& _name, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaNode::_MetaNode(_name, _friendlyName, _description) {}
	// using NodeIOIterator = _MetaNode::NodeIOIterator;
	using NodeIt = _MetaNode::NodeIt;

	NodeIt NodeBegin() override { return NodeIt::create(mNodes.begin()); }
	NodeIt NodeEnd() override { return NodeIt::create(mNodes.end()); }
};

}} // btrack::nodes