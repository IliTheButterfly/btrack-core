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
    virtual void compile() { update(); }
    virtual std::string_view category() const = 0;
    virtual std::string& category() = 0;
    virtual Input<VariantType>* addInput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) = 0;
    virtual Output<VariantType>* addOutput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) = 0;
    virtual bool isForced() const { return false; }
    virtual boost::container::vector<PortBase<VariantType>*>::const_iterator pbegin() const = 0;
    virtual boost::container::vector<PortBase<VariantType>*>::const_iterator pend() const = 0;
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
    ID_t mID;
    std::string mName;
    std::string mDescription;
    std::string mCategory;
protected:
    boost::container::vector<PortBase<VariantType>*> mPorts;

    ID_t& id() override { return mID; }

    Node(const ID_t& _id, const std::string& _name, const std::string& _category, const std::string& _description = "")
        : mID(_id), mName(_name), mCategory(_category), mDescription(_description) {}
public:
    const ID_t& id() const override { return mID; }
    std::string_view name() const override { return mName; }
    std::string& name() override { return mName; }
    std::string_view description() const override { return mDescription; }
    std::string& description() override { return mDescription; }
    std::string_view category() const override { return mCategory; }
    std::string& category() override { return mCategory; }
    Input<VariantType>* addInput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (Input<VariantType>*)(mPorts.emplace_back(new Input<VariantType>(this, id().child(mPorts.size(), true).get(), _name, _description, _default)));
    }

    Output<VariantType>* addOutput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (Output<VariantType>*)(mPorts.emplace_back(new Output<VariantType>(this, id().child(mPorts.size(), true).get(), _name, _description, _default)));
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

    ID_t& id() override { return mInnerNode->id(); }
public:
    void run() { mInnerNode->run(); }
    void compile() { mInnerNode->compile(); }
    const std::string& category() const { return mInnerNode->category(); }
    std::string& category() { return mInnerNode->category(); }
    const ID_t& id() const override { return mInnerNode->id(); }
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

    virtual ~NodeDecorator() = default;
};

} // namespace btrack::nodes::system
#endif // __NODE_H__