#ifndef __COMPOSITE_H__
#define __COMPOSITE_H__

#include "nodes/system/Item.h"
#include <stack>

namespace btrack::nodes::system
{
template <typename T>
class _CompositeIterator;

template <typename T>
using CompositeIterator = _CompositeIterator<std::remove_pointer_t<std::remove_reference_t<T>>>;

class Composite : public Item
{
public:
    virtual Item *relativeAt(const ID_t &_id);
    virtual const Item *relativeAt(const ID_t &_id) const;

    virtual Item *at(const ID_e &_id, const bool& port = false) = 0;
    virtual const Item *at(const ID_e &_id, const bool& port = false) const = 0;

    CompositeIterator<Item> recursiveIterator(int depth = -1);
    CompositeIterator<const Item> recursiveIterator(int depth = -1) const;
    template <typename U>
    CompositeIterator<U> recursiveIteratorFilter(int depth = -1);
    template <typename U>
    CompositeIterator<std::add_const_t<U>> recursiveIteratorFilter(int depth = -1) const;
};


template <typename T>
class _CompositeIterator
{
private:
public:
    typedef T   value_type;
    typedef std::conditional_t<std::is_const_v<T>, const Item*, Item*> item_pointer;
    typedef int difference_type;
    typedef T*  pointer;
    typedef T&  reference;
    typedef std::conditional_t<std::is_const_v<T>, const Composite*, Composite*> root_pointer;
    typedef const Composite* const_root_pointer;

    explicit _CompositeIterator(root_pointer root, int depth = -1) : mDepth(depth)
    {
        if (root)
            stack.push({root, 0, root->isNode()});
    }

    pointer next()
    {
        while (!stack.empty())
        {
            auto &[composite, index, port] = stack.top();

            item_pointer item = composite->at(index++, port);
            if (item)
            {
                if (root_pointer subComposite = dynamic_cast<root_pointer>(item))
                {
                    // Keep going deeper if mDepth == -1 or if mDepth not reached
                    if (mDepth > -1 || mDepth > currentDepth())
                        stack.push({subComposite, 0, ((const_root_pointer)subComposite)->isNode()});
                }
                if (pointer res = dynamic_cast<pointer>(item)) return res;
            }
            else // Reached end
            {
                if (port) port = false;
                else stack.pop();
                index = 0;
            }
        }
        return nullptr;
    }

    bool hasNext() const { return !stack.empty(); }

    int currentDepth() const { return stack.size() - 1; }

    const ID_e &currentID() const
    {
        if (stack.empty())
            throw std::runtime_error("Iterator exhausted");
        return ((const_root_pointer)stack.top().composite)->id();
    }

    std::string_view currentName() const
    {
        if (stack.empty())
            throw std::runtime_error("Iterator exhausted");
        return ((const_root_pointer)stack.top().composite)->name();
    }

    std::string_view currentDescription() const
    {
        if (stack.empty())
            throw std::runtime_error("Iterator exhausted");
        return ((const_root_pointer)stack.top().composite)->description();
    }

    typename std::enable_if<!std::is_const_v<T>, std::string&> currentName()
    {
        if (stack.empty())
            throw std::runtime_error("Iterator exhausted");
        return stack.top().composite->name();
    }

    typename std::enable_if<!std::is_const_v<T>, std::string&> currentDescription()
    {
        if (stack.empty())
            throw std::runtime_error("Iterator exhausted");
        return stack.top().composite->description();
    }

private:
    struct StackFrame
    {
        root_pointer composite;
        ID_e index;
        bool port;
    };
    std::stack<StackFrame> stack;
    int mDepth;
};

template <typename U>
inline CompositeIterator<U> Composite::recursiveIteratorFilter(int depth)
{
    return CompositeIterator<U>(this, depth);
}

template <typename U>
inline CompositeIterator<std::add_const_t<U>> Composite::recursiveIteratorFilter(int depth) const
{
    return CompositeIterator<std::add_const_t<U>>(this, depth);
}
}

#endif // __COMPOSITE_H__