#include <gtest/gtest.h>
#include "nodes/utilities/Graph.h"
#include <boost/numeric/ublas/matrix_sparse.hpp>

using Graph = btrack::nodes::utilities::Graph;
using namespace boost::numeric::ublas;

void linearGraph(Graph &g)
{
	for (auto i = 0, ii = 1; ii < g.nodeCount(); ++i, ++ii)
	{
		g.addEdge(i, ii);
	}
}

#define ASSERT_MAT_EQUAL(mat1, mat2, h, w)                                       \
	{                                                                            \
		for (int _i = 0; _i < w; ++_i)                                           \
		{                                                                        \
			for (int _ii = 0; _ii < h; ++_ii)                                    \
			{                                                                    \
				std::cout << "Index [" << _i << ", " << _ii << "]" << std::endl; \
				ASSERT_EQ(mat1(_i, _ii), mat2(_i, _ii));                         \
			}                                                                    \
		}                                                                        \
	}

TEST(GraphTests, LinearGraph3)
{
	Graph g = Graph(3);
	mapped_matrix<int> expected(3, 3);

	// 0 -> 1 ; 1 -> 2
	expected(0, 1) = 1;
	expected(1, 2) = 1;

	linearGraph(g);

	ASSERT_MAT_EQUAL(g, expected, 3, 3);
}

TEST(GraphTests, LinearGraph5)
{
	Graph g = Graph(5);
	mapped_matrix<int> expected(5, 5);

	expected(0, 1) = 1;
	expected(1, 2) = 1;
	expected(2, 3) = 1;
	expected(3, 4) = 1;

	linearGraph(g);

	ASSERT_MAT_EQUAL(g, expected, 5, 5);
}

TEST(GraphTests, BranchedGraph5)
{
	Graph g = Graph(5);
	mapped_matrix<int> expected(5, 5);

	g.addEdge(1, 0);
	g.addEdge(2, 0);
	g.addEdge(3, 2);
	g.addEdge(4, 2);
	g.addEdge(4, 3);


	expected(1, 0) = 1;
	expected(2, 0) = 1;
	expected(3, 2) = 1;
	expected(4, 2) = 1;
	expected(4, 3) = 1;


	ASSERT_MAT_EQUAL(g, expected, 5, 5);
}
