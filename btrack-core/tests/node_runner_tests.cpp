#include <gtest/gtest.h>
#include "nodes/system/nodes.h"
#include "nodes/runners/SimpleNodeRunner.h"
#include "nodes/runners/ParallelRunnerProvider.h"
#include "test_nodes.h"

using namespace btrack::nodes::system;
using namespace btrack::nodes::runners;




TEST(NodeRunnerTests, BaseOrdering)
{
	auto provider = std::make_shared<ParallelNodeRunnerProvider>();
	
	auto mStart = TestNodeStart::create(provider->step());
	auto mMid = TestNodeMid1::create(provider->step());
	auto mEnd = TestNodeEnd::create(provider->step());

	mStart->arrayOutput->connectTo(mMid->arrayInput);
	mMid->arrayOutput->connectTo(mEnd->arrayInput);
	auto values = std::vector<int>();
	values.emplace_back(1);
	values.emplace_back(2);
	values.emplace_back(3);
	mStart->start(10, values);
	provider->getRunner()->run();
}