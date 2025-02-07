#include "nodes/utilities/Graph.h"


namespace btrack::nodes::utilities {

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

}