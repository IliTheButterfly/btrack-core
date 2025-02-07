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
	std::vector<std::weak_ptr<_NodeItem>> mNodes;
	Graph mGraph;
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