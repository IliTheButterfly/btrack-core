#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>

namespace btrack::nodes::utilities {

using namespace boost::numeric::ublas;

class Graph {
private:
    mapped_matrix<int> mAdjMatrix;
    std::size_t mNodeCount;
public:

    Graph(std::size_t initialSize) : mAdjMatrix(initialSize, initialSize), mNodeCount(initialSize) {}

    // Add an edge between two nodes
    void addEdge(std::size_t u, std::size_t v) {
        if (u < mNodeCount && v < mNodeCount)
            mAdjMatrix(u, v) = 1; // Set edge
    }

    // Remove an edge between two nodes
    void removeEdge(std::size_t u, std::size_t v) {
        if (u < mNodeCount && v < mNodeCount)
            mAdjMatrix(u, v) = 0; // Remove edge
    }

    // Add a new node (increase matrix size)
    void addNode() {
        mNodeCount++;
        mapped_matrix<int> newMatrix(mNodeCount, mNodeCount);

        // Copy existing data to new matrix
        for (std::size_t i = 0; i < mNodeCount - 1; ++i)
            for (std::size_t j = 0; j < mNodeCount - 1; ++j)
                newMatrix(i, j) = mAdjMatrix(i, j);

        mAdjMatrix = std::move(newMatrix);
    }

    // Remove a node (rebuild a smaller matrix)
    void removeNode(std::size_t r);

	size_t nodeCount() const;

    // Print adjacency matrix
    void print() {
        std::cout << "Adjacency Matrix (" << mNodeCount << "x" << mNodeCount << "):\n";
        for (std::size_t i = 0; i < mNodeCount; ++i) {
            for (std::size_t j = 0; j < mNodeCount; ++j)
                std::cout << mAdjMatrix(i, j) << " ";
            std::cout << "\n";
        }
    }
};

}

#endif // __GRAPH_H__