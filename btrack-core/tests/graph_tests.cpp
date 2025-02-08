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
	mapped_matrix<int> expected;
	linearGraph(g);

	{
		for (int _i = 0; _i < 3; ++_i)
		{
			for (int _ii = 0; _ii < 3; ++_ii)
			{
				std::cout << "Index [" << _i << ", " << _ii << "]" << std::endl;
				switch (0)
				case 0:
				default:
					if (const ::testing::AssertionResult gtest_ar = (::testing::internal::EqHelper::Compare("g(_i, _ii)", "expected(_i, _ii)", g(_i, _ii), expected(_i, _ii))))
						;
					else
						return ::testing::internal::AssertHelper(::testing::TestPartResult::kFatalFailure, "/media/capra/efe2920a-7c6d-4a40-bf38-4ab551f556af/home/ili/prog/btrack-core/btrack-core/tests/graph_tests.cpp", 34, gtest_ar.failure_message()) = ::testing::Message();
			}
		}
	};
}
