#ifndef __PARALLELRUNNERPROVIDER_H__
#define __PARALLELRUNNERPROVIDER_H__

#include "nodes/runners/NodeRunnerProvider.h"

namespace btrack::nodes::runners {

class ParallelNodeRunnerProvider : public NodeRunnerProvider
{
public:
	std::shared_ptr<NodeRunner> getRunner() override;
	std::shared_ptr<NodeRunnerProvider> step() override;
	std::shared_ptr<NodeRunnerProvider> branch() override;
};

}

#endif // __PARALLELRUNNERPROVIDER_H__