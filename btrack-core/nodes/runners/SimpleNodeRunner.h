#ifndef __SIMPLENODERUNNER_H__
#define __SIMPLENODERUNNER_H__


#include "nodes/runners/NodeRunner.h"
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/algorithm/algorithm.hpp>
#include "nodes/utilities/Graph.h"

namespace btrack::nodes::runners {

using Graph = btrack::nodes::utilities::Graph;
using namespace boost::numeric::ublas;

class SimpleNodeRunner : public NodeRunner
{
private:
	// Empty item acting as the end point (goal) to solve the graph for
	const std::shared_ptr<_NodeItem> blank = nullptr;
	std::vector<std::weak_ptr<_NodeItem>> mNodes { blank };
	Graph mGraph = {1};
private:
public:
	void run() override;
	void update() override;
	void addItem(std::shared_ptr<_NodeItem> node) override;
	void removeItem(std::shared_ptr<_NodeItem> node) override;
	void addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
	void removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
};

}

#endif // __SIMPLENODERUNNER_H__