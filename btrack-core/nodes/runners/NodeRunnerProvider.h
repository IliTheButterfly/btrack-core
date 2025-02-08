#ifndef __NODERUNNERPROVIDER_H__
#define __NODERUNNERPROVIDER_H__

#include "nodes/runners/NodeRunner.h"

namespace btrack::nodes::runners {

class NodeRunnerProvider : public std::enable_shared_from_this<NodeRunnerProvider>
{
public:
	virtual std::shared_ptr<NodeRunner> getRunner() = 0;

	virtual std::shared_ptr<NodeRunnerProvider> step() = 0;

	virtual std::shared_ptr<NodeRunnerProvider> branch() = 0;
};

}

#endif // __NODERUNNERPROVIDER_H__