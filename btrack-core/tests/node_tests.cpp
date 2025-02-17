#include <gtest/gtest.h>
#include "test_nodes.h"
#include <boost/timer/timer.hpp>

// expected: vector<vector<NodeBase<VariantTest>*>> actual: vector<NodeBase<VariantTest>*>
#define ASSERT_ORDER(expected, actual)                                                                                                                                        \
    {                                                                                                                                                                         \
        int __i = 0, __j = 0, __c = 0;                                                                                                                                        \
        while (__i < actual.size())                                                                                                                                           \
        {                                                                                                                                                                     \
            if (std::find(expected.at(__j).begin(), expected.at(__j).end(), actual.at(__i)) == expected.at(__j).end())                                                        \
            {                                                                                                                                                                 \
                auto msg = ::testing::AssertionFailure() << "Expected '" __i << " (" << actual.at(__i)->name() << ")' to be ";                                          \
                bool first = true;                                                                                                                                            \
                for (auto e : expected.at(__j))                                                                                                                               \
                {                                                                                                                                                             \
                    if (!first)                                                                                                                                               \
                        msg << " or ";                                                                                                                                        \
                    first = false;                                                                                                                                            \
                    msg << "'" << std::find(actual.begin(), actual.end(), e) - actual.begin() << " (" << (*std::find(actual.begin(), actual.end(), e))->name() << ")'"; \
                }                                                                                                                                                             \
                GTEST_MESSAGE_(msg.message(), ::testing::TestPartResult::kFatalFailure);                                                                                      \
            }                                                                                                                                                                 \
            ++__i;                                                                                                                                                            \
            ++__c;                                                                                                                                                            \
            if (__c > expected.at(__j).size())                                                                                                                                \
            {                                                                                                                                                                 \
                __c = 0;                                                                                                                                                      \
                ++__j;                                                                                                                                                        \
            }                                                                                                                                                                 \
        }                                                                                                                                                                     \
    }

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
    ExecutionOrder o;
    auto start = NodeStart(&o, ID_t(ID_t::NODE, {0, 0}));
    auto mid = NodeMid(&o, ID_t(ID_t::NODE, {0, 1}));
    auto end = NodeEnd(&o, ID_t(ID_t::NODE, {0, 2}));

    ASSERT_EQ(start.valueOut->connect(mid.valueIn1), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->connect(mid.valueIn1), ConnectionResult::ALREADY_CONNECTED);
    ASSERT_EQ(start.valueOut->connect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(start.valueOut->disconnect(mid.valueIn1), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->disconnect(mid.valueIn1), ConnectionResult::NOT_CONNECTED);
    ASSERT_EQ(start.valueOut->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(start.valueOut->connect(mid.valueIn1), ConnectionResult::SUCCESS);
    ASSERT_EQ(start.valueOut->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueOut->connect(end.valueIn1), ConnectionResult::SUCCESS);

    start.valueOut->get().get<int>() = 5;

    start.run();
    mid.run();
    end.run();
    ASSERT_EQ(end.valueIn1->get().get<int>(), 6);
}

TEST(NodeTests, InputConnections)
{
    ExecutionOrder o;
    auto start = NodeStart(&o, ID_t(ID_t::NODE, {0, 0}));
    auto mid = NodeMid(&o, ID_t(ID_t::NODE, {0, 1}));
    auto end = NodeEnd(&o, ID_t(ID_t::NODE, {0, 2}));

    ASSERT_EQ(mid.valueIn1->connect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn1->connect(start.valueOut), ConnectionResult::ALREADY_CONNECTED);
    ASSERT_EQ(mid.valueIn1->connect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueIn1->disconnect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn1->disconnect(start.valueOut), ConnectionResult::NOT_CONNECTED);
    ASSERT_EQ(mid.valueIn1->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(mid.valueIn1->connect(start.valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid.valueIn1->disconnect(nullptr), ConnectionResult::NULL_POINTER);
    ASSERT_EQ(end.valueIn1->connect(mid.valueOut), ConnectionResult::SUCCESS);

    start.valueOut->get().get<int>() = 5;

    start.run();
    mid.run();
    end.run();
    ASSERT_EQ(end.valueIn1->get().get<int>(), 6);
}

TEST(NodeTests, OutputDestructors)
{
    ExecutionOrder o;
    auto start = NodeStart(&o, ID_t(ID_t::NODE, {0, 0}));
    auto end = NodeEnd(&o, ID_t(ID_t::NODE, {0, 2}));

    {
        auto mid = NodeMid(&o, ID_t(ID_t::NODE, {0, 1}));

        ASSERT_EQ(start.valueOut->connect(mid.valueIn1), ConnectionResult::SUCCESS);
        ASSERT_EQ(mid.valueOut->connect(end.valueIn1), ConnectionResult::SUCCESS);
        ASSERT_EQ(start.valueOut->connectionCount(), 1);
        ASSERT_EQ(mid.valueOut->connectionCount(), 1);
        ASSERT_EQ(mid.valueIn1->connectionCount(), 1);
        ASSERT_EQ(end.valueIn1->connectionCount(), 1);
    }
    ASSERT_EQ(start.valueOut->connectionCount(), 0);
    ASSERT_EQ(end.valueIn1->connectionCount(), 0);
}

TEST(NodeTests, TreeExecutionOrder)
{
    ExecutionOrder o;
    NodeTree<VariantTest> tree;
    auto start = tree.addNode<NodeStart>(&o, ID_t(ID_t::NODE, {0, 0}));
    auto mid = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 1}));
    auto end = tree.addNode<NodeEnd>(&o, ID_t(ID_t::NODE, {0, 2}));

    ASSERT_EQ(mid->valueIn1->connect(start->valueOut), ConnectionResult::SUCCESS);
    ASSERT_EQ(end->valueIn1->connect(mid->valueOut), ConnectionResult::SUCCESS);

    start->valueOut->get().get<int>() = 5;

    tree.compile();
    tree.run();
    ASSERT_EQ(end->valueIn1->get().get<int>(), 6);
    ASSERT_EQ(o.order.at(0), start);
    ASSERT_EQ(o.order.at(1), mid);
    ASSERT_EQ(o.order.at(2), end);
}

/**
 * @brief Create a chain of Nodes and link their input and outputs
 *
 * This will create a graph like so:
 *
 * S1 -> M1, M2, M3
 * S2 -> M2
 * M1 -> M3, E1
 * M2 -> M4
 * M3 -> E2
 * M4 -> M5
 * M5 -> E2
 *
 * Execution order should be:
 * (S1, S2), (M1, M2), (M3, M4), (E1, M5), E2
 */
TEST(NodeTests, TreeExecutionOrderComplex)
{
    ExecutionOrder o;
    NodeTree<VariantTest> tree;
    auto start1 = tree.addNode<NodeStart>(&o, ID_t(ID_t::NODE, {0, 0}), "S1");
    auto start2 = tree.addNode<NodeStart>(&o, ID_t(ID_t::NODE, {0, 1}), "S2");
    auto mid1 = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 2}), "M1");
    auto mid2 = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 3}), "M2");
    auto mid3 = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 4}), "M3");
    auto mid4 = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 5}), "M4");
    auto mid5 = tree.addNode<NodeMid>(&o, ID_t(ID_t::NODE, {0, 6}), "M5");
    auto end1 = tree.addNode<NodeEnd>(&o, ID_t(ID_t::NODE, {0, 7}), "E1");
    auto end2 = tree.addNode<NodeEnd>(&o, ID_t(ID_t::NODE, {0, 8}), "E2");

    ASSERT_EQ(start1->valueOut->connect(mid1->valueIn1), ConnectionResult::SUCCESS);
    ASSERT_EQ(start1->valueOut->connect(mid2->valueIn1), ConnectionResult::SUCCESS);
    ASSERT_EQ(start1->valueOut->connect(mid3->valueIn1), ConnectionResult::SUCCESS);

    ASSERT_EQ(start2->valueOut->connect(mid2->valueIn2), ConnectionResult::SUCCESS);

    ASSERT_EQ(mid1->valueOut->connect(mid3->valueIn2), ConnectionResult::SUCCESS);
    ASSERT_EQ(mid1->valueOut->connect(end1->valueIn1), ConnectionResult::SUCCESS);

    ASSERT_EQ(mid2->valueOut->connect(mid4->valueIn1), ConnectionResult::SUCCESS);

    ASSERT_EQ(mid3->valueOut->connect(end2->valueIn1), ConnectionResult::SUCCESS);

    ASSERT_EQ(mid4->valueOut->connect(mid5->valueIn1), ConnectionResult::SUCCESS);

    ASSERT_EQ(mid5->valueOut->connect(end2->valueIn2), ConnectionResult::SUCCESS);

    start1->valueOut->get().get<int>() = 5;
    start2->valueOut->get().get<int>() = 15;

    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.compile();
    }
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.run();
    }


    ASSERT_EQ(mid1->valueOut->get().get<int>(), 6);  // S1 + 1 = 5 + 1 = 6
    ASSERT_EQ(mid2->valueOut->get().get<int>(), 21); // S1 + S2 + 1 = 5 + 15 + 1 = 21
    ASSERT_EQ(mid3->valueOut->get().get<int>(), 12); // S1 + M1 + 1 = 5 + 6 + 1 = 12
    ASSERT_EQ(mid4->valueOut->get().get<int>(), 22); // M2 + 1 = 21 + 1 = 22
    ASSERT_EQ(mid5->valueOut->get().get<int>(), 23); // M4 + 1 = 22 + 1 = 23
    ASSERT_EQ(end1->valueIn1->get().get<int>(), 6);  // M1 = 6
    ASSERT_EQ(end2->valueIn1->get().get<int>(), 12); // M3 = 12
    ASSERT_EQ(end2->valueIn2->get().get<int>(), 23); // M5 = 23
}

TEST(NodeTests, TreeExecutionOrderVeryComplex0)
{
    constexpr int count = 10000;
    ExecutionOrder o;
    NodeTree<VariantTest> tree;

    auto start = tree.addNode<NodeStart>(nullptr, ID_t(ID_t::NODE, {0, 0}), "S");
    auto end = tree.addNode<NodeEnd>(nullptr, ID_t(ID_t::NODE, {0, 0}), "E");

    boost::container::vector<NodeMultiMid*> nodes;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        for (int i = 0; i < count; ++i)
        {
            nodes.emplace_back(tree.addNode<NodeMultiMid>(nullptr, ID_t(ID_t::NODE, {0, 0}), "M"));
            if (i % 10 == 0) start->valueOut->connect(nodes.back()->addInput("StartInput"));
            if (i % 8 == 1) nodes.back()->valueOut->connect(end->addInput("StartInput"));
        }

        for (int i = 0; i < count; ++i)
        {
            if (i % 5 == 0 && i > 5)
            {
                nodes.at(i-5)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-2)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 == 1 && i > 5)
            {
                nodes.at(i-4)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-1)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 >= 2 && i > 5)
            {
                nodes.at(i-3)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 7 == 0 && i > 10)
            {
                nodes.at(i-7)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-9)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
        }
    }

    std::cout << "count: " << count << std::endl;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.compile<0>();
    }
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.run();
    }
}

TEST(NodeTests, TreeExecutionOrderVeryComplex1)
{
    constexpr int count = 10000;
    ExecutionOrder o;
    NodeTree<VariantTest> tree;

    auto start = tree.addNode<NodeStart>(nullptr, ID_t(ID_t::NODE, {0, 0}), "S");
    auto end = tree.addNode<NodeEnd>(nullptr, ID_t(ID_t::NODE, {0, 0}), "E");

    boost::container::vector<NodeMultiMid*> nodes;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        for (int i = 0; i < count; ++i)
        {
            nodes.emplace_back(tree.addNode<NodeMultiMid>(nullptr, ID_t(ID_t::NODE, {0, 0}), "M"));
            if (i % 10 == 0) start->valueOut->connect(nodes.back()->addInput("StartInput"));
            if (i % 8 == 1) nodes.back()->valueOut->connect(end->addInput("StartInput"));
        }

        for (int i = 0; i < count; ++i)
        {
            if (i % 5 == 0 && i > 5)
            {
                nodes.at(i-5)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-2)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 == 1 && i > 5)
            {
                nodes.at(i-4)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-1)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 >= 2 && i > 5)
            {
                nodes.at(i-3)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 7 == 0 && i > 10)
            {
                nodes.at(i-7)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-9)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
        }
    }

    std::cout << "count: " << count << std::endl;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.compile<1>();
    }
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.run();
    }
}

TEST(NodeTests, TreeExecutionOrderVeryComplex2)
{
    constexpr int count = 10000;
    ExecutionOrder o;
    NodeTree<VariantTest> tree;

    auto start = tree.addNode<NodeStart>(nullptr, ID_t(ID_t::NODE, {0, 0}), "S");
    auto end = tree.addNode<NodeEnd>(nullptr, ID_t(ID_t::NODE, {0, 0}), "E");

    boost::container::vector<NodeMultiMid*> nodes;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        for (int i = 0; i < count; ++i)
        {
            nodes.emplace_back(tree.addNode<NodeMultiMid>(nullptr, ID_t(ID_t::NODE, {0, 0}), "M"));
            if (i % 10 == 0) start->valueOut->connect(nodes.back()->addInput("StartInput"));
            if (i % 8 == 1) nodes.back()->valueOut->connect(end->addInput("StartInput"));
        }

        for (int i = 0; i < count; ++i)
        {
            if (i % 5 == 0 && i > 5)
            {
                nodes.at(i-5)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-2)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 == 1 && i > 5)
            {
                nodes.at(i-4)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-1)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 5 >= 2 && i > 5)
            {
                nodes.at(i-3)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
            if (i % 7 == 0 && i > 10)
            {
                nodes.at(i-7)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
                nodes.at(i-9)->valueOut->connect(nodes.at(i)->addInput("InputFrom"));
            }
        }
    }

    std::cout << "count: " << count << std::endl;
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.compile<2>();
    }
    {
        boost::timer::auto_cpu_timer t{std::cout};
        tree.run();
    }
}

/**
 * @brief Create a chain of Nodes and link their input and outputs
 *
 * This will create a graph like so:
 *
 * S1 -> M1, M2, M3
 * S2 -> M2
 * M1 -> M3, E1
 * M2 -> M4
 * M3 -> E2
 * M4 -> M5, M6
 * M5 -> E2
 *
 * Execution order should be:
 * (S1, S2), (M1, M2), (M3, M4), (E1, M5), E2
 * M6 does not go to an end point
 */
// TEST(NodeTests, TreeExecutionOrderComplexOptimize)
// {
//     ExecutionOrder o;
//     NodeTree<VariantTest> tree;
//     auto start1 = tree.addNode<NodeStart>(o, ID_t(ID_t::NODE, {0, 0}), "S1");
//     auto start2 = tree.addNode<NodeStart>(o, ID_t(ID_t::NODE, {0, 1}), "S2");
//     auto mid1 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 2}), "M1");
//     auto mid2 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 3}), "M2");
//     auto mid3 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 4}), "M3");
//     auto mid4 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 5}), "M4");
//     auto mid5 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 6}), "M5");
//     auto mid6 = tree.addNode<NodeMid>(o, ID_t(ID_t::NODE, {0, 7}), "M6");
//     auto end1 = tree.addNode<NodeEnd>(o, ID_t(ID_t::NODE, {0, 8}), "E1");
//     auto end2 = tree.addNode<NodeEnd>(o, ID_t(ID_t::NODE, {0, 9}), "E2");

//     ASSERT_EQ(start1->valueOut->connect(mid1->valueIn1), ConnectionResult::SUCCESS);
//     ASSERT_EQ(start1->valueOut->connect(mid2->valueIn1), ConnectionResult::SUCCESS);
//     ASSERT_EQ(start1->valueOut->connect(mid3->valueIn1), ConnectionResult::SUCCESS);

//     ASSERT_EQ(start2->valueOut->connect(mid2->valueIn2), ConnectionResult::SUCCESS);

//     ASSERT_EQ(mid1->valueOut->connect(mid3->valueIn2), ConnectionResult::SUCCESS);
//     ASSERT_EQ(mid1->valueOut->connect(end1->valueIn1), ConnectionResult::SUCCESS);

//     ASSERT_EQ(mid2->valueOut->connect(mid4->valueIn1), ConnectionResult::SUCCESS);

//     ASSERT_EQ(mid3->valueOut->connect(end2->valueIn1), ConnectionResult::SUCCESS);

//     ASSERT_EQ(mid4->valueOut->connect(mid5->valueIn1), ConnectionResult::SUCCESS);
//     ASSERT_EQ(mid4->valueOut->connect(mid6->valueIn1), ConnectionResult::SUCCESS);

//     ASSERT_EQ(mid5->valueOut->connect(end2->valueIn2), ConnectionResult::SUCCESS);

//     start1->valueOut->get().get<int>() = 5;
//     start2->valueOut->get().get<int>() = 15;
    
//     tree.compile();
//     tree.run();
//     ASSERT_EQ(mid1->valueOut->get().get<int>(), 6);  // S1 + 1 = 5 + 1 = 6
//     ASSERT_EQ(mid2->valueOut->get().get<int>(), 21); // S1 + S2 + 1 = 5 + 15 + 1 = 21
//     ASSERT_EQ(mid3->valueOut->get().get<int>(), 12); // S1 + M1 + 1 = 5 + 6 + 1 = 12
//     ASSERT_EQ(mid4->valueOut->get().get<int>(), 22); // M2 + 1 = 21 + 1 = 22
//     ASSERT_EQ(mid5->valueOut->get().get<int>(), 23); // M4 + 1 = 22 + 1 = 23
//     ASSERT_EQ(end1->valueIn1->get().get<int>(), 6);  // M1 = 6
//     ASSERT_EQ(end2->valueIn1->get().get<int>(), 12); // M3 = 12
//     ASSERT_EQ(end2->valueIn2->get().get<int>(), 23); // M5 = 23

//     ASSERT_TRUE(std::find(o.order.begin(), o.order.end(), mid6) == o.order.end());
// }