#ifndef __TEST_NODES_H__
#define __TEST_NODES_H__

#include "nodes/system/nodes.h"

using namespace btrack;
using namespace btrack::nodes::system;

typedef VariantBase<bool, int, float> VariantTest;

struct ExecutionOrder
{
    boost::container::vector<nodes::system::NodeBase<VariantTest>*> order;
    void executed(nodes::system::NodeBase<VariantTest>* node) { order.emplace_back(node); }
};

class NodeStart : public nodes::system::Node<VariantTest>
{
public:
    Output<VariantTest>* valueOut;
    ExecutionOrder* order;
    NodeStart(ExecutionOrder* _order, const ID_t& _id, const std::string& _name = "NodeStart", const std::string& _description = "")
        : NodeStart::Node(_id, _name, "test", _description), order(_order)
    {
        valueOut = addOutput("Output", "", 0);
    }

    void run() override
    {
        if (order) order->executed(this);
    }
};

class NodeMid : public nodes::system::Node<VariantTest>
{
public:
    Input<VariantTest>* valueIn1;
    Input<VariantTest>* valueIn2;
    Output<VariantTest>* valueOut;
    ExecutionOrder* order;
    NodeMid(ExecutionOrder* _order, const ID_t& _id, const std::string& _name = "NodeMid", const std::string& _description = "")
        : NodeMid::Node(_id, _name, "test", _description), order(_order)
    {
        valueIn1 = addInput("Input1", "", 0);
        valueIn2 = addInput("Input2", "", 0);
        valueOut = addOutput("Output", "", 0);
    }

    void run() override
    {
        valueOut->get().get<int>() = valueIn1->get().get<int>() + valueIn2->get().get<int>() + 1;
        if (order) order->executed(this);
    }
};

class NodeMultiMid : public nodes::system::Node<VariantTest>
{
public:
    Output<VariantTest>* valueOut;
    ExecutionOrder* order;
    NodeMultiMid(ExecutionOrder* _order, const ID_t& _id, const std::string& _name = "NodeMultiMid", const std::string& _description = "")
        : NodeMultiMid::Node(_id, _name, "test", _description), order(_order)
    {
        valueOut = addOutput("Output", "", 0);
    }

    void run() override
    {
        int res = 1;
        for (auto i : this->mPorts)
        {
            if (i->type() == PortType::INPUT) res += i->get().get<int>();
        }
        valueOut->get().get<int>() = res;
        if (order) order->executed(this);
    }
};

class NodeEnd : public nodes::system::Node<VariantTest>
{
public:
    Input<VariantTest>* valueIn1;
    Input<VariantTest>* valueIn2;
    ExecutionOrder* order;
    NodeEnd(ExecutionOrder* _order, const ID_t& _id, const std::string& _name = "NodeEnd", const std::string& _description = "")
        : NodeEnd::Node(_id, _name, "test", _description), order(_order)
    {
        valueIn1 = addInput("Input1", "", 0);
        valueIn2 = addInput("Input2", "", 0);
    }

    bool isForced() const override { return true; }

    void run() override
    {
        if (order) order->executed(this);
    }

};

#endif // __TEST_NODES_H__