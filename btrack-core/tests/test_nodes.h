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
    NodeStart() = default;
    Output<VariantTest>* valueOut;
    ExecutionOrder* order = nullptr;
    NodeStart(ExecutionOrder* _order, const std::string& _name = "NodeStart", const std::string& _description = "")
        : NodeStart::Node(_name, "test", _description), order(_order)
    {
        valueOut = addOutput("Output", "", 0);
    }
    void clone(Item* to) const override
    {
        Node<VariantTest>::clone(to);
        NodeStart* node = dynamic_cast<NodeStart*>(to);
        if (!node) return;
        node->valueOut = dynamic_cast<Output<VariantTest>*>(this->mPorts.at(0));
    }

    Item* createClone() const override
    {
        auto res = new NodeStart();
        this->clone(res);
        return res;
    }

    void run() override
    {
        if (order) order->executed(this);
    }
};

class NodeMid : public nodes::system::Node<VariantTest>
{
public:
    NodeMid() = default;
    Input<VariantTest>* valueIn1;
    Input<VariantTest>* valueIn2;
    Output<VariantTest>* valueOut;
    ExecutionOrder* order;
    NodeMid(ExecutionOrder* _order, const std::string& _name = "NodeMid", const std::string& _description = "")
        : NodeMid::Node(_name, "test", _description), order(_order)
    {
        valueIn1 = addInput("Input1", "", 0);
        valueIn2 = addInput("Input2", "", 0);
        valueOut = addOutput("Output", "", 0);
    }
    void clone(Item* to) const override
    {
        Node<VariantTest>::clone(to);
        NodeMid* node = dynamic_cast<NodeMid*>(to);
        if (!node) return;
        node->valueIn1 = dynamic_cast<Input<VariantTest>*>(this->mPorts.at(0));
        node->valueIn2 = dynamic_cast<Input<VariantTest>*>(this->mPorts.at(1));
        node->valueOut = dynamic_cast<Output<VariantTest>*>(this->mPorts.at(2));
    }

    Item* createClone() const override
    {
        auto res = new NodeMid();
        this->clone(res);
        return res;
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
    NodeMultiMid() = default;
    Output<VariantTest>* valueOut;
    ExecutionOrder* order;
    NodeMultiMid(ExecutionOrder* _order, const std::string& _name = "NodeMultiMid", const std::string& _description = "")
        : NodeMultiMid::Node(_name, "test", _description), order(_order)
    {
        valueOut = addOutput("Output", "", 0);
    }
    void clone(Item* to) const override
    {
        Node<VariantTest>::clone(to);
        NodeMultiMid* node = dynamic_cast<NodeMultiMid*>(to);
        if (!node) return;
        node->valueOut = dynamic_cast<Output<VariantTest>*>(this->mPorts.at(0));
    }

    Item* createClone() const override
    {
        auto res = new NodeMultiMid();
        this->clone(res);
        return res;
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
    NodeEnd() = default;
    Input<VariantTest>* valueIn1;
    Input<VariantTest>* valueIn2;
    ExecutionOrder* order;
    NodeEnd(ExecutionOrder* _order, const std::string& _name = "NodeEnd", const std::string& _description = "")
        : NodeEnd::Node(_name, "test", _description), order(_order)
    {
        valueIn1 = addInput("Input1", "", 0);
        valueIn2 = addInput("Input2", "", 0);
    }

    void clone(Item* to) const override
    {
        Node<VariantTest>::clone(to);
        NodeEnd* node = dynamic_cast<NodeEnd*>(to);
        if (!node) return;
        node->valueIn1 = dynamic_cast<Input<VariantTest>*>(this->mPorts.at(0));
        node->valueIn2 = dynamic_cast<Input<VariantTest>*>(this->mPorts.at(1));
    }

    Item* createClone() const override
    {
        auto res = new NodeEnd();
        this->clone(res);
        return res;
    }

    bool isForced() const override { return true; }

    void run() override
    {
        if (order) order->executed(this);
    }

};

#endif // __TEST_NODES_H__