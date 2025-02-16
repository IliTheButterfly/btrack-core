#ifndef __TEST_NODES_H__
#define __TEST_NODES_H__

#include "nodes/system/nodes.h"

using namespace btrack;
using namespace btrack::nodes::system;

typedef VariantBase<bool, int, float> VariantTest;

class NodeStart : public nodes::system::Node<VariantTest>
{
public:
    Output<VariantTest>* valueOut;
    NodeStart(const ID_t& _id, const std::string& _name = "NodeStart", const std::string& _description = "")
        : NodeStart::Node(_id, _name, "test", _description)
    {
        valueOut = addOutput("Output", "", 0);
    }

    void run() override
    {}

};

class NodeMid : public nodes::system::Node<VariantTest>
{
public:
    Input<VariantTest>* valueIn;
    Output<VariantTest>* valueOut;
    NodeMid(const ID_t& _id, const std::string& _name = "NodeMid", const std::string& _description = "")
        : NodeMid::Node(_id, _name, "test", _description)
    {
        valueIn = addInput("Input", "", 0);
        valueOut = addOutput("Output", "", 0);
    }

    void run() override
    {
        valueOut->get().get<int>() = valueIn->get().get<int>();
    }

};

class NodeEnd : public nodes::system::Node<VariantTest>
{
public:
    Input<VariantTest>* valueIn;
    NodeEnd(const ID_t& _id, const std::string& _name = "NodeEnd", const std::string& _description = "")
        : NodeEnd::Node(_id, _name, "test", _description)
    {
        valueIn = addInput("Input", "", 0);
    }

    void run() override
    {}

};

#endif // __TEST_NODES_H__