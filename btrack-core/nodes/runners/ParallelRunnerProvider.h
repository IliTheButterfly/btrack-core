#ifndef __PARALLELRUNNERPROVIDER_H__
#define __PARALLELRUNNERPROVIDER_H__

#include "nodes/runners/NodeRunnerProvider.h"

namespace btrack::nodes::runners {

class ParallelNodeRunnerProvider : public NodeRunnerProvider
{
public:
	class Runner : public NodeRunner
	{
	private:
		std::weak_ptr<ParallelNodeRunnerProvider> mProvider;
	public:
		Runner(std::shared_ptr<ParallelNodeRunnerProvider> provider) :
			mProvider(provider) {}

		void run();
		void setCount(int count);
		int getCount() const;
			
	};
private:
	std::vector<std::shared_ptr<NodeRunner>> mBranches;
	std::weak_ptr<ParallelNodeRunnerProvider> mParent;
	std::shared_ptr<NodeRunner> mRunner;

	struct Private { explicit Private() {} };

	static std::shared_ptr<ParallelNodeRunnerProvider> create(std::shared_ptr<ParallelNodeRunnerProvider> parent)
	{
		auto res = std::make_shared<ParallelNodeRunnerProvider>(Private());
		res->mParent = parent;
		return res;
	}
public:
	ParallelNodeRunnerProvider(Private _);
	
	static std::shared_ptr<ParallelNodeRunnerProvider> create()
	{
		return create(std::make_shared<ParallelNodeRunnerProvider::Runner>(asParallelNodeRunnerProvider()));
	}

	std::shared_ptr<NodeRunner> getRunner() override;
	std::shared_ptr<NodeRunnerProvider> step() override;
	std::shared_ptr<NodeRunnerProvider> branch() override;

	std::shared_ptr<ParallelNodeRunnerProvider> asParallelNodeRunnerProvider();

	void addItem(std::shared_ptr<_NodeItem> node) override;
	void removeItem(std::shared_ptr<_NodeItem> node) override;
	void addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
	void removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
};

}

#endif // __PARALLELRUNNERPROVIDER_H__