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
TEST(NodeTests, OutputConnections)
{
    auto start = NodeStart(ID_t(ID_t::NODE, {0, 0}));
    auto mid = NodeMid(ID_t(ID_t::NODE, {0, 1}));
    auto end = NodeEnd(ID_t(ID_t::NODE, {0, 2}));

    ASSERT_EQ(start.valueOut->connect(mid.valueIn), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->connect(mid.valueIn), ConnectionResult::ALREADY_CONNECTED);
    ASSERT_EQ(start.valueOut->connect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(start.valueOut->disconnect(mid.valueIn), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->disconnect(mid.valueIn), ConnectionResult::NOT_CONNECTED);
    ASSERT_EQ(start.valueOut->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(start.valueOut->connect(mid.valueIn), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueOut->connect(end.valueIn), ConnectionResult::SUCCESS);

    start.valueOut->get().get<int>() = 5;

    start.run();
    mid.run();
    end.run();
    ASSERT_EQ(end.valueIn->get().get<int>(), 5);
}

TEST(NodeTests, InputConnections)
{
    auto start = NodeStart(ID_t(ID_t::NODE, {0, 0}));
    auto mid = NodeMid(ID_t(ID_t::NODE, {0, 1}));
    auto end = NodeEnd(ID_t(ID_t::NODE, {0, 2}));

    ASSERT_EQ(mid.valueIn->connect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn->connect(start.valueOut), ConnectionResult::ALREADY_CONNECTED);
    ASSERT_EQ(mid.valueIn->connect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueIn->disconnect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn->disconnect(start.valueOut), ConnectionResult::NOT_CONNECTED);
    ASSERT_EQ(mid.valueIn->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueIn->connect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(end.valueIn->connect(mid.valueOut), ConnectionResult::SUCCESS);

    start.valueOut->get().get<int>() = 5;

    start.run();
    mid.run();
    end.run();
    ASSERT_EQ(end.valueIn->get().get<int>(), 5);
}

TEST(NodeTests, OutputDestructors)
{
    auto start = NodeStart(ID_t(ID_t::NODE, {0, 0}));
    auto end = NodeEnd(ID_t(ID_t::NODE, {0, 2}));
    
    {
        auto mid = NodeMid(ID_t(ID_t::NODE, {0, 1}));

        ASSERT_EQ(start.valueOut->connect(mid.valueIn), ConnectionResult::SUCCESS);
        ASSERT_EQ(mid.valueOut->connect(end.valueIn), ConnectionResult::SUCCESS);
        ASSERT_EQ(start.valueOut->connectionCount(), 1);
        ASSERT_EQ(mid.valueOut->connectionCount(), 1);
        ASSERT_EQ(mid.valueIn->connectionCount(), 1);
        ASSERT_EQ(end.valueIn->connectionCount(), 1);
    }
    ASSERT_EQ(start.valueOut->connectionCount(), 0);
    ASSERT_EQ(end.valueIn->connectionCount(), 0);
}
