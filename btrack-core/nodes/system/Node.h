#ifndef __NODE_H__
#define __NODE_H__


#include "nodes/system/Port.h"
#include "nodes/system/Composite.h"
#include "nodes/system/Input.h"
#include "nodes/system/Output.h"
#include <boost/container/vector.hpp>

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class NodeTree;

template <VariantTemplate VariantType>
class NodeBase : public Composite
{
public:
    virtual void run() = 0;
    virtual void compile() { }
    virtual std::string_view category() const = 0;
    virtual std::string& category() = 0;
    virtual Input<VariantType>* addInput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) = 0;
    virtual Output<VariantType>* addOutput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) = 0;
    virtual bool isForced() const { return false; }
    virtual size_t inputCount() const = 0;
    virtual size_t outputCount() const = 0;
    virtual boost::container::vector<PortBase<VariantType>*>::const_iterator pbegin() const = 0;
    virtual boost::container::vector<PortBase<VariantType>*>::const_iterator pend() const = 0;
    bool isNode() const override { return true; }
    void clone(Item* to) const override
    {
        Item::clone(to);
        NodeBase<VariantType>* node = dynamic_cast<NodeBase<VariantType>*>(to);
        if (!node) return;
        node->category() = this->category();
    }

    virtual ~NodeBase() = default;

    friend NodeTree<VariantType>;
};

template <VariantTemplate VariantType>
class Node : public NodeBase<VariantType>
{
private:
    ID_e mID = 0;
    std::string mName;
    std::string mDescription;
    std::string mCategory;
protected:
    boost::container::vector<PortBase<VariantType>*> mPorts;

    ID_e& id() override { return mID; }

    Node(const std::string& _name, const std::string& _category, const std::string& _description = "")
        : mName(_name), mCategory(_category), mDescription(_description) {}
public:
    const ID_e& id() const override { return mID; }
    std::string_view name() const override { return mName; }
    std::string& name() override { return mName; }
    std::string_view description() const override { return mDescription; }
    std::string& description() override { return mDescription; }
    std::string_view category() const override { return mCategory; }
    std::string& category() override { return mCategory; }
    Input<VariantType>* addInput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (Input<VariantType>*)(mPorts.emplace_back(new Input<VariantType>(this, mPorts.size(), _name, _description, _default)));
    }

    Output<VariantType>* addOutput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (Output<VariantType>*)(mPorts.emplace_back(new Output<VariantType>(this, mPorts.size(), _name, _description, _default)));
    }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pbegin() const override { return mPorts.begin(); }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pend() const override { return mPorts.end(); }
    void clone(Item* to) const override
    {
        NodeBase<VariantType>::clone(to);
        NodeBase<VariantType>* node = dynamic_cast<NodeBase<VariantType>*>(to);
        if (!node) return;
        for (const PortBase<VariantType>* p : mPorts)
        {
            if (p->type() == PortType::INPUT) node->addInput(std::string(p->name()), std::string(p->description()), p->get());
            if (p->type() == PortType::OUTPUT) node->addOutput(std::string(p->name()), std::string(p->description()), p->get());
        }
    }

    Item *at(const ID_e &_id, const bool& port = false) override
    {
        if (!port) return nullptr;
        if (_id >= mPorts.size()) return nullptr;
        return mPorts.at(_id);
    }
    const Item *at(const ID_e &_id, const bool& port = false) const override
    {
        if (!port) return nullptr;
        if (_id >= mPorts.size()) return nullptr;
        return mPorts.at(_id);
    }
    size_t inputCount() const override
    {
        size_t count = 0;
        for (const auto* p : mPorts)
        {
            if (p->type() == PortType::INPUT) ++count;
        }
        return count;
    }
    size_t outputCount() const override
    {
        size_t count = 0;
        for (const auto* p : mPorts)
        {
            if (p->type() == PortType::OUTPUT) ++count;
        }
        return count;
    }

    virtual ~Node()
    {
        for (PortBase<VariantType>* p : this->mPorts)
        {
            delete p;
            p = nullptr;
        }
    }
};

template <VariantTemplate VariantType, std::derived_from<NodeBase<VariantType>> NodeType>
class NodeDecorator : public NodeBase<VariantType>
{
protected:
    unique_ptr<NodeType> mInnerNode;

    ID_e& id() override { return mInnerNode->id(); }
public:
    void run() { mInnerNode->run(); }
    void compile() { mInnerNode->compile(); }
    const std::string& category() const { return mInnerNode->category(); }
    std::string& category() { return mInnerNode->category(); }
    const ID_e& id() const override { return mInnerNode->id(); }
    std::string_view name() const override { return mInnerNode->name(); }
    std::string& name() override { return mInnerNode->name(); }
    std::string_view description() const override { return mInnerNode->description(); }
    std::string& description() override { return mInnerNode->description(); }
    void clone(Item* to) const override
    {
        NodeBase<VariantType>::clone(to);
        NodeDecorator* node = dynamic_cast<NodeDecorator*>(to);
        if (!node) return;
        node->mInnerNode = mInnerNode.clone();
    }
    size_t inputCount() const override { return mInnerNode->inputCount(); }
    size_t outputCount() const override { return mInnerNode->outputCount(); }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pbegin() const override { return mInnerNode->pbegin(); }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pend() const override { return mInnerNode->pend(); }

    virtual ~NodeDecorator() = default;
};

} // namespace btrack::nodes::system
#endif // __NODE_H__