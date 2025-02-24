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
class GroupNode : public Node<VariantType>
{
private:
    GroupTemplate<VariantType>* mTemplate = nullptr;
    NodeTree<VariantType>* mInnerNode = nullptr;
    GroupNode() = default;
    GroupNode(GroupTemplate<VariantType>* _template) : mTemplate(_template)
    {
        if (!mTemplate) return;
        mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
        this->name() = mInnerNode->name();
        this->category() = mInnerNode->category();
        this->description() = mInnerNode->description();
    }
    GroupNode(const std::string& _name, const std::string& _category, const std::string& _description = "")
        : GroupNode::Node(_name, _category, _description) {}
public:
    bool usesTemplate(const GroupTemplate<VariantType>* t) const { return mTemplate && mTemplate == t; }
    void compile() override 
    {
        if (this->mInnerNode) delete this->mInnerNode;
        this->mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
        mInnerNode->name() = this->name();
        mInnerNode->category() = this->category();
        mInnerNode->description() = this->description();
        this->mInnerNode->compile();
    }
    void run() override { this->mInnerNode->run(); }

    void clone(Item* to) const override;
    Item* createClone() const override;

    Item *at(const ID_e &_id, const bool& port = false) override
    {
        if (port) return Node<VariantType>::at(_id, port);
        if (!mInnerNode) return nullptr;
        return mInnerNode->at(_id, port);
    }
    const Item *at(const ID_e &_id, const bool& port = false) const override
    {
        if (port) return Node<VariantType>::at(_id, port);
        if (!mInnerNode) return nullptr;
        return mInnerNode->at(_id, port);
    }

    virtual ~GroupNode()
    {
        if (this->mInnerNode) delete mInnerNode;
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
    Node<VariantType>::clone(to);
    GroupNode<VariantType>* group = dynamic_cast<GroupNode<VariantType>*>(to);
    if (!group) return;

    group->mTemplate = mTemplate;
    group->category() = this->category();
    group->description() = this->description();
    group->name() = this->name();
    group->mInnerNode = dynamic_cast<NodeTree<VariantType>*>(mTemplate->getTemplate()->createClone());
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