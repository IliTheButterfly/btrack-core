#include "SimpleNodeRunner.h"
#include "nodes/system/nodes.h"

namespace btrack::nodes::runners
{
	void SimpleNodeRunner::run()
	{
		// mAdjacent.assign()
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
		if (node && node->isNode() && std::find_if(mNodes.begin(), mNodes.end(), [&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == node; }) == mNodes.end())
		{
			mNodes.emplace_back(node);
		}
	}
	
	void SimpleNodeRunner::removeItem(std::shared_ptr<_NodeItem> node)
	{
		if (!node || !node->isNode()) return;
		auto it = std::find_if(mNodes.begin(), mNodes.end(), [&](std::weak_ptr<_NodeItem> i){ return !i.expired() && i.lock() == node; });
		if (it != mNodes.end())
		{
			mNodes.erase(it);
		}
	}
	
	void SimpleNodeRunner::addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
	{
		
	}
	
	void SimpleNodeRunner::removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
	{
		
	}
}