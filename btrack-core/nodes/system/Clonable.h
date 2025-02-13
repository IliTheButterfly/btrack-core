#ifndef __CLONABLE_H__
#define __CLONABLE_H__

#include "memory.h"

namespace btrack::nodes::system {


/**
 * @brief Interface for any object that can clone itself and it's members such that
 * cloned instances will have the same behavior as the original.
 */
template <typename T>
class Clonable
{
public:
    virtual shared_ptr<T> clone() = 0;
    template <typename U>
    shared_ptr<U> clone() { return dynamic_pointer_cast<U>(this->clone()); }
    virtual bool isClone(shared_ptr<T> other) = 0;
    virtual bool isSame(shared_ptr<T> other) = 0;
};

}

#endif // __CLONABLE_H__