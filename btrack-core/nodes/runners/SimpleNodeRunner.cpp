#include "SimpleNodeRunner.h"
#include "nodes/system/nodes.h"

namespace btrack::nodes::runners
{

void SimpleNodeRunner::run()
{
}

void SimpleNodeRunner::update()
{
	// Check for released nodes
	for (auto node : mNodes)
	{
		if (node.expired()) throw ImproperGraphManagementError();
	}
}

void SimpleNodeRunner::addItem(std::shared_ptr<_NodeItem> node)
{
	// Only add node (not IOs) if not present and if node not null
	if (node && node->isNode() && std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == node; }) == mNodes.end())
	{
		mNodes.emplace_back(node);
		mGraph.addNode();
	}
}

void SimpleNodeRunner::removeItem(std::shared_ptr<_NodeItem> node)
{
	// Only remove node (not IOs) if not present and if node not null
	if (!node || !node->isNode()) return;
	auto it = std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == node; });
	if (it != mNodes.end())
	{
		mNodes.erase(it);
		mGraph.removeNode(it - mNodes.begin());
	}
}

void SimpleNodeRunner::addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
{
	auto ifrom = std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == from; });
	if (ifrom == mNodes.end()) return;
	auto ito = std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == to; });
	if (ito == mNodes.end()) return;
	mGraph.addEdge(ifrom - mNodes.begin(), ito - mNodes.begin());
}

void SimpleNodeRunner::removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
{
	auto ifrom = std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == from; });
	if (ifrom == mNodes.end()) return;
	auto ito = std::find_if(mNodes.begin(), mNodes.end(), 
		[&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == to; });
	if (ito == mNodes.end()) return;
	mGraph.removeEdge(ifrom - mNodes.begin(), ito - mNodes.begin());
}

}