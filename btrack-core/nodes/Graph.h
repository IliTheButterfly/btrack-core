#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include "memory_backend.h"

namespace btrack::nodes
{

using namespace boost::numeric::ublas;

class Graph
{
public:
    mapped_matrix<int> adjMatrix;
    size_t nodeCount;

    Graph(size_t initialSize) : adjMatrix(initialSize, initialSize), nodeCount(initialSize) {}

    // Add an edge between two nodes
    void addEdge(size_t u, size_t v)
    {
        if (u < nodeCount && v < nodeCount)
            adjMatrix(u, v) = 1; // Set edge
    }

    // Remove an edge between two nodes
    void removeEdge(size_t u, size_t v)
    {
        if (u < nodeCount && v < nodeCount)
            adjMatrix(u, v) = 0; // Remove edge
    }

    // Add a new node (increase matrix size)
    void addNode()
    {
        nodeCount++;
        mapped_matrix<int> newMatrix(nodeCount, nodeCount);

        // Copy existing data to new matrix
        for (size_t i = 0; i < nodeCount - 1; ++i)
            for (size_t j = 0; j < nodeCount - 1; ++j)
                newMatrix(i, j) = adjMatrix(i, j);

        adjMatrix = move(newMatrix);
    }

    void reset(size_t initialSize)
    {
        adjMatrix = {initialSize, initialSize};
        nodeCount = initialSize;
    }

    // Remove a node (rebuild a smaller matrix)
    void removeNode(size_t r)
    {
        if (r >= nodeCount)
            return;

        mapped_matrix<int> newMatrix(nodeCount - 1, nodeCount - 1);

        for (size_t i = 0, new_i = 0; i < nodeCount; ++i)
        {
            if (i == r)
                continue; // Skip row
            for (size_t j = 0, new_j = 0; j < nodeCount; ++j)
            {
                if (j == r)
                    continue; // Skip column
                newMatrix(new_i, new_j) = adjMatrix(i, j);
                ++new_j;
            }
            ++new_i;
        }

        nodeCount--;
        adjMatrix = move(newMatrix);
    }
};

}

#endif // __GRAPH_H__