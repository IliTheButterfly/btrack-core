#ifndef __NODEGRAPH_H__
#define __NODEGRAPH_H__

#include "nodes/system/MetaNode.h"
#include <algorithm>

namespace btrack::nodes::system {

class NodeGraph : public NodeObserver
{
public:
	using MetaNodePtr = std::shared_ptr<MetaNode>;
private:
	std::vector<MetaNodePtr> mNodes;
public:

	void addNode(MetaNodePtr node);
	void removeNode(MetaNodePtr node);

	void run();

private:
	void update();
};

}

#endif // __NODEGRAPH_H__