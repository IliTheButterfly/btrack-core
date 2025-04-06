#ifndef __GROUPNODE_H__
#define __GROUPNODE_H__

#include "nodes/system/NodeTree.h"

namespace btrack::nodes::system {

constexpr ID_e GROUP_ID = 0b1000000000000000;

template <VariantTemplate VariantType>
class GroupNode;

template <VariantTemplate VariantType>
class GroupTemplate : public Item
{
private:
    NodeTree<VariantType> mTemplate;
protected:
    ID_e& _id() override { return NodeRegister::id(mTemplate); }
public:
    GroupTemplate() = default;
    GroupTemplate(const std::string& _name, const std::string& _category, const std::string& _description = "");
    GroupNode<VariantType>* create();
    NodeTree<VariantType>* getTemplate();
    void clone(Item* to) const override;
    Item* createClone() const override;
    const ID_e& id() const override { return mTemplate.id(); }
    std::string_view name() const override { return mTemplate.name(); }
    std::string& name() override { return mTemplate.name(); }
    std::string_view description() const override { return mTemplate.description(); }
    std::string& description() override { return mTemplate.description(); }
};

template <VariantTemplate VariantType>
class GroupNode : public NodeDecorator<VariantType, NodeTree<VariantType>>
{
private:
    ID_e mID = 0;
    std::string mName;
    std::string mDescription;
    GroupTemplate<VariantType>* mTemplate = nullptr;
    GroupNode() = default;
    GroupNode(GroupTemplate<VariantType>* _template) : mTemplate(_template)
    {
        if (!mTemplate) return;
        this->mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
        this->name() = this->mInnerNode->name();
        this->category() = this->mInnerNode->category();
        this->description() = this->mInnerNode->description();
    }
    GroupNode(const std::string& _name, const std::string& _category, const std::string& _description = "")
        : GroupNode::Node(_name, _category, _description) {}

    boost::container::vector<PortBase<VariantType>*> mPorts;

    ID_e& _id() override { return mID; }
    boost::container::vector<PortBase<VariantType>*>::iterator _pbegin() override { return mPorts.begin(); }
    boost::container::vector<PortBase<VariantType>*>::iterator _pend() override { return mPorts.end(); }

    void linkIOs()
    {
        for (ID_e i = 0; i < mPorts.size(); ++i)
        {
            if (mPorts[i]->type() == PortType::INPUT)
            {
                dynamic_cast<PortBase<VariantType>*>(this->at(i, true))->connect(dynamic_cast<PortBase<VariantType>*>(this->mInnerNode->at(i, true)));
            }
            else
            {
                dynamic_cast<PortBase<VariantType>*>(this->mInnerNode->at(i, true))->connect(dynamic_cast<PortBase<VariantType>*>(this->at(i, true)));
            }
        }
    }

    void matchIOs()
    {
        if (mPorts.size() != (this->mInnerNode->inputCount() + this->mInnerNode->outputCount()))
        {
            mPorts.clear();
            for (ID_e i = 0; i < this->mInnerNode->inputCount() + this->mInnerNode->outputCount(); ++i)
            {
                auto innerPort = dynamic_cast<PortBase<VariantType>*>(this->mInnerNode->at(i, true));
                mPorts.emplace_back(new PassthroughPort<VariantType>(this, mPorts.size(), innerPort->type(), innerPort->name(), innerPort->description(), innerPort->get()));
            }
        }
    }
public:
    const ID_e& id() const override { return mID; }
    std::string_view name() const override { return mName; }
    std::string& name() override { return mName; }
    std::string_view description() const override { return mDescription; }
    std::string& description() override { return mDescription; }
    bool usesTemplate(const GroupTemplate<VariantType>* t) const { return mTemplate && mTemplate == t; }
    PortBase<VariantType>* addInput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (PortBase<VariantType>*)(mPorts.emplace_back(new Input<VariantType>(this, mPorts.size(), _name, _description, _default)));
    }
    PortBase<VariantType>* addOutput(const std::string& _name, const std::string& _description = "", VariantType _default = VariantType()) override
    {
        return (PortBase<VariantType>*)(mPorts.emplace_back(new Output<VariantType>(this, mPorts.size(), _name, _description, _default)));
    }
    void compile() override 
    {
        if (this->mInnerNode) delete this->mInnerNode;
        this->mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
        this->mInnerNode->name() = this->name();
        this->mInnerNode->category() = this->category();
        this->mInnerNode->description() = this->description();

        // Update ports
        if (mPorts.size() == (this->mInnerNode->inputCount() + this->mInnerNode->outputCount()))
        {
            // We assume no ports were added or removed
            linkIOs();
        }
        else
        {
            matchIOs();
            linkIOs();
        }

        this->mInnerNode->compile();
    }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pbegin() const override { return mPorts.cbegin(); }
    boost::container::vector<PortBase<VariantType>*>::const_iterator pend() const override { return mPorts.cend(); }
    void clone(Item* to) const override;
    Item* createClone() const override;

    Item *at(const ID_e &_id, const bool& port = false) override
    {
        if (!port) return this->mInnerNode->at(_id, false);
        if (_id >= mPorts.size()) return nullptr;
        return mPorts.at(_id);
    }
    const Item *at(const ID_e &_id, const bool& port = false) const override
    {
        if (!port) return this->mInnerNode->at(_id, false);
        if (_id >= mPorts.size()) return nullptr;
        return mPorts.at(_id);
    }

    virtual ~GroupNode()
    {
        if (this->mInnerNode) delete this->mInnerNode;
        this->mInnerNode = nullptr;
    }
    friend GroupTemplate<VariantType>;
};

template <VariantTemplate VariantType>
inline GroupTemplate<VariantType>::GroupTemplate(const std::string &_name, const std::string &_category, const std::string &_description)
{
    mTemplate = NodeTree<VariantType>(_name, _category, _description);
}

template <VariantTemplate VariantType>
inline GroupNode<VariantType>* GroupTemplate<VariantType>::create()
{
    return new GroupNode<VariantType>(this);
}

template <VariantTemplate VariantType>
inline NodeTree<VariantType> *GroupTemplate<VariantType>::getTemplate()
{
    return &mTemplate;
}

template <VariantTemplate VariantType>
inline void GroupTemplate<VariantType>::clone(Item *to) const
{
    Item::clone(to);
    GroupTemplate<VariantType>* cloned = dynamic_cast<GroupTemplate<VariantType>*>(to);
    if (!cloned) return;
    mTemplate.clone(&cloned->mTemplate);
}

template <VariantTemplate VariantType>
inline Item *GroupTemplate<VariantType>::createClone() const
{
    auto res = new GroupTemplate<VariantType>();
    this->clone(res);
    return res;
}

template <VariantTemplate VariantType>
inline void GroupNode<VariantType>::clone(Item *to) const
{
    NodeBase<VariantType>::clone(to);
    GroupNode<VariantType>* group = dynamic_cast<GroupNode<VariantType>*>(to);
    if (!group) return;

    group->mTemplate = mTemplate;
    group->mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
    group->matchIOs();
    group->linkIOs();
}

template <VariantTemplate VariantType>
inline Item *GroupNode<VariantType>::createClone() const
{
    auto res = new GroupNode<VariantType>();
    this->clone(res);
    return res;
}

}

#endif // __GROUPNODE_H__