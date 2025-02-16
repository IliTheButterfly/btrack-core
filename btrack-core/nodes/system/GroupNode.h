#ifndef __GROUPNODE_H__
#define __GROUPNODE_H__

#include "nodes/system/NodeTree.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class GroupNodeTemplate;

template <VariantTemplate VariantType>
class GroupNode;

template <VariantTemplate VariantType>
class GroupTemplate : public Item
{
private:
    GroupNodeTemplate<VariantType> mTemplate;
public:
    unique_ptr<GroupNode<VariantType>> create();
    GroupNodeTemplate<VariantType>* getTemplate();
};



template <VariantTemplate VariantType>
class GroupNodeTemplate : public Node<VariantType>
{
protected:
    NodeTree<VariantType> mTree;
public:
    void run() override { mTree.run(); }
    void clone(Item* to) const override;
};

template <VariantTemplate VariantType>
class GroupNode : public NodeDecorator<VariantType, GroupNodeTemplate<VariantType>>
{
private:
    GroupTemplate<VariantType>* mTemplate;
public:
    bool usesTemplate(const GroupTemplate<VariantType>* t) const { return mTemplate == t; }
    void compile() override { this->mInnerNode = move(mTemplate->create()); }
    void run() override { this->mInnerNode->run(); }

    void clone(Item* to) const override;
};

template <VariantTemplate VariantType>
inline unique_ptr<GroupNode<VariantType>> GroupTemplate<VariantType>::create()
{
    return make_unique<GroupNode<VariantType>>();
}

template <VariantTemplate VariantType>
inline GroupNodeTemplate<VariantType> *GroupTemplate<VariantType>::getTemplate()
{
    return mTemplate;
}

template <VariantTemplate VariantType>
inline void GroupNode<VariantType>::clone(Item *to) const
{
    GroupNode<VariantType>* group = dynamic_cast<GroupNode<VariantType>*>(to);
    if (!group) return;

    group->mTemplate = mTemplate;
    group->category() = this->category();
    group->description() = this->description();
    group->name() = this->name();
    group->mInnerNode = move(this->mInnerNode->clone());
}

template <VariantTemplate VariantType>
inline void GroupNodeTemplate<VariantType>::clone(Item *to) const
{
    GroupNodeTemplate<VariantType>* group = dynamic_cast<GroupNodeTemplate<VariantType>*>(to);
    if (!group) return;
    
}

}

#endif // __GROUPNODE_H__