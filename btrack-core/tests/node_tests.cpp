#include <gtest/gtest.h>
#include "test_nodes.h"
#include <atomic>
#include <boost/chrono.hpp>

/**
 * @brief Create a chain of Nodes and link their input and outputs
 * 
 * This will create a graph like so:
 * 
 *   Start         Mid            End
 *      V *------* V  V *--------* V 
 */
TEST(NodeTests, SimpleChain)
{
    {
        auto start = NodeStart(ID_t(ID_t::NODE, {0, 0}));
        auto mid = NodeMid(ID_t(ID_t::NODE, {0, 1}));
        auto end = NodeEnd(ID_t(ID_t::NODE, {0, 2}));

        start.valueOut->connect(mid.valueIn);
        mid.valueOut->connect(end.valueIn);

        start.valueOut->get().get<int>() = 5;

        start.run();
        mid.run();
        end.run();
        ASSERT_EQ(end.valueIn->get().get<int>(), 5);
    }
}

