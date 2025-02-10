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
public:
    class NodeOrderIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::shared_ptr<_NodeItem>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<_NodeItem>*;
        using reference = std::shared_ptr<_NodeItem>&;

        NodeOrderIterator(const std::vector<std::weak_ptr<_NodeItem>>& nodes, 
                          const std::vector<int>& executionOrder, 
                          size_t index = 0)
            : mNodes(nodes), mExecutionOrder(executionOrder), mIndex(index) {}

        value_type operator*() const {
            int nodeIndex = mExecutionOrder[mIndex];
            return mNodes[nodeIndex].lock();
        }

        NodeOrderIterator& operator++() {
            ++mIndex;
            return *this;
        }

        NodeOrderIterator operator++(int) {
            NodeOrderIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const NodeOrderIterator& other) const {
            return mIndex == other.mIndex;
        }

        bool operator!=(const NodeOrderIterator& other) const {
            return !(*this == other);
        }
    
    private:
        const std::vector<std::weak_ptr<_NodeItem>>& mNodes;
        const std::vector<int>& mExecutionOrder;
        size_t mIndex;
    };

    class NodeOrderIterable {
    public:
        NodeOrderIterable(const SimpleNodeRunner& runner) : mRunner(runner) {}
        NodeOrderIterator begin() const { return NodeOrderIterator(mRunner.mNodes, mRunner.mExecutionOrder, 0); }
        NodeOrderIterator end() const { return NodeOrderIterator(mRunner.mNodes, mRunner.mExecutionOrder, mRunner.mExecutionOrder.size()); }
    private:
        const SimpleNodeRunner& mRunner;
    };


private:
    const std::shared_ptr<_NodeItem> blank = nullptr;
    std::vector<std::weak_ptr<_NodeItem>> mNodes { blank };
    Graph mGraph = {1};
    std::vector<int> mExecutionOrder;

    struct Private { explicit Private() {} };
public:
    SimpleNodeRunner(Private _) = default;

    static std::shared_ptr<SimpleNodeRunner> create()
    {
        return std::make_shared<SimpleNodeRunner>(Private());
    }

    void run() override;
    void update() override;
    void addItem(std::shared_ptr<_NodeItem> node) override;
    void removeItem(std::shared_ptr<_NodeItem> node) override;
    void addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
    void removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) override;
    NodeOrderIterable nodeOrder() const { return NodeOrderIterable(*this); }
};

}

#endif // __SIMPLENODERUNNER_H__
