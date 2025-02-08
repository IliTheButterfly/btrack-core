#include "nodes/utilities/Graph.h"


namespace btrack::nodes::utilities {

void Graph::addEdge(std::size_t u, std::size_t v)
{
	if (u < mNodeCount && v < mNodeCount)
		mAdjMatrix(u, v) = 1; // Set edge
}

void Graph::removeEdge(std::size_t u, std::size_t v)
{
	if (u < mNodeCount && v < mNodeCount)
		mAdjMatrix(u, v) = 0; // Remove edge
}

void Graph::addNode()
{
	mNodeCount++;
	mapped_matrix<int> newMatrix(mNodeCount, mNodeCount);

	// Copy existing data to new matrix
	for (std::size_t i = 0; i < mNodeCount - 1; ++i)
		for (std::size_t j = 0; j < mNodeCount - 1; ++j)
			newMatrix(i, j) = mAdjMatrix(i, j);

	mAdjMatrix = std::move(newMatrix);
}

const int &Graph::operator()(size_t u, size_t v) const
{
	return mAdjMatrix(u, v);
}


void Graph::removeNode(std::size_t r)
{
	if (r >= mNodeCount) return;

	mapped_matrix<int> newMatrix(mNodeCount - 1, mNodeCount - 1);
	
	for (std::size_t i = 0, new_i = 0; i < mNodeCount; ++i) {
		if (i == r) continue; // Skip row
		for (std::size_t j = 0, new_j = 0; j < mNodeCount; ++j) {
			if (j == r) continue; // Skip column
			newMatrix(new_i, new_j) = mAdjMatrix(i, j);
			++new_j;
		}
		++new_i;
	}

	mNodeCount--;
	mAdjMatrix = std::move(newMatrix);
}

size_t Graph::nodeCount() const
{
	return mNodeCount;
}

void Graph::print() const
{
	std::cout << "Adjacency Matrix (" << mNodeCount << "x" << mNodeCount << "):\n";
	for (std::size_t i = 0; i < mNodeCount; ++i) {
		for (std::size_t j = 0; j < mNodeCount; ++j)
			std::cout << mAdjMatrix(i, j) << " ";
		std::cout << "\n";
	}
}

}