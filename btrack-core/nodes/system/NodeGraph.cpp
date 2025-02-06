#include "NodeGraph.h"

void btrack::nodes::system::NodeGraph::addNode(MetaNodePtr node)
{
	auto it = std::find_if(this->mNodes.begin(), this->mNodes.end(), node);
	if (it == mNodes.end())
	{
		mNodes.emplace_back(node);
	}
}

void btrack::nodes::system::NodeGraph::removeNode(MetaNodePtr node)
{
	auto it = std::find_if(this->mNodes.begin(), this->mNodes.end(), node);
	if (it != mNodes.end())
	{
		mNodes.erase(it);
	}
}

void btrack::nodes::system::NodeGraph::run()
{
	
}

void btrack::nodes::system::NodeGraph::update()
{
}
