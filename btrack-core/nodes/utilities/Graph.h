#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>

namespace btrack::nodes::utilities {

using namespace boost::numeric::ublas;

class Graph
{
private:
    mapped_matrix<int> mAdjMatrix;
    std::size_t mNodeCount;
public:

    Graph(std::size_t initialSize) : mAdjMatrix(initialSize, initialSize), mNodeCount(initialSize) {}

    // Add an edge between two nodes
    void addEdge(std::size_t u, std::size_t v);

    // Remove an edge between two nodes
    void removeEdge(std::size_t u, std::size_t v);

    // Add a new node (increase matrix size)
    void addNode();

    const int& operator()(size_t u, size_t v) const;

    // Remove a node (rebuild a smaller matrix)
    void removeNode(std::size_t r);

	size_t nodeCount() const;

    // Print adjacency matrix
    void print() const;
};

}

#endif // __GRAPH_H__