#ifndef __PORT_H__
#define __PORT_H__

#include <Variant.h>
#include "nodes/system/Item.h"

namespace btrack::nodes::system {

template <typename ..._Types>
class Node;

template <typename ..._Types>
class Port : public Item
{
public:
    enum Type {
        UNKNOWN,
        INPUT,
        OUTPUT,
    };

    virtual const VariantBase<..._Types>& get() const = 0;
    virtual VariantBase<..._Types>& get() = 0;
    virtual Type type() const = 0;
    virtual bool connect(std::shared_ptr<Port> other) = 0;
    virtual bool disconnect(std::shared_ptr<Port> other) = 0;
    virtual shared_ptr<Node<..._Types>> parent() = 0;
};

}

#endif // __PORT_H__