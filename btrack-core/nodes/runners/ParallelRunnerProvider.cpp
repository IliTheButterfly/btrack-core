#include "ParallelRunnerProvider.h"
#include "SimpleNodeRunner.h"

namespace btrack::nodes::runners {
ParallelNodeRunnerProvider::ParallelNodeRunnerProvider(Private _)
{
	mRunner = SimpleNodeRunner::create();
}

std::shared_ptr<NodeRunner> ParallelNodeRunnerProvider::getRunner()
{
	return mRunner;
}

std::shared_ptr<NodeRunnerProvider> ParallelNodeRunnerProvider::step()
{
	return asParallelNodeRunnerProvider();
}
std::shared_ptr<NodeRunnerProvider> ParallelNodeRunnerProvider::branch()
{
	auto res = create(asParallelNodeRunnerProvider())
	mBranches.emplace_back(res->mRunner);
	return res;
}

std::shared_ptr<ParallelNodeRunnerProvider> ParallelNodeRunnerProvider::asParallelNodeRunnerProvider()
{

	return std::dynamic_pointer_cast<ParallelNodeRunnerProvider>(this->shared_from_this());
}

void ParallelNodeRunnerProvider::addItem(std::shared_ptr<_NodeItem> node)
{
	if (!node) return;
	if (node->isMeta())
	{
		step()->getRunner()->addItem(node);
	}
	else
	{
		if (node->isNode())
		{
			branch()->getRunner()->addItem(node);
		}
	}
}

void ParallelNodeRunnerProvider::removeItem(std::shared_ptr<_NodeItem> node)
{
}

void ParallelNodeRunnerProvider::addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
{

}

void ParallelNodeRunnerProvider::removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to)
{
}

void ParallelNodeRunnerProvider::Runner::run()
{
	if (mProvider.expired() || !mProvider.lock()) return;
	auto p = mProvider.lock();
	setCount(p->mBranches.size());
	for (auto b : p->mBranches)
	{
		if (b) b->run();
	}
}

void ParallelNodeRunnerProvider::Runner::setCount(int count)
{
	for (auto b : mProvider.lock()->mBranches)
	{
		
	}
}
int ParallelNodeRunnerProvider::Runner::getCount() const
{
	if (mProvider.expired() || !mProvider.lock()) return 0;
	return mProvider.lock()->mBranches.size();
}
}