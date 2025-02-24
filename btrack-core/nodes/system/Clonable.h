#ifndef __CLONABLE_H__
#define __CLONABLE_H__

#include "memory.h"

namespace btrack::nodes::system {


/**
 * @brief Interface for objects that can clone themselves, ensuring
 * cloned instances maintain the same behavior as the original.
 */
template <typename T>
class Clonable
{
public:
    virtual ~Clonable() = default;

    /**
     * @brief Clones the current object into the provided instance.
     * @param to Pointer to an instance where the clone should be stored.
     */
    virtual void clone(T* to) const = 0;

    virtual T* createClone() const = 0;

    /**
     * @brief Checks if the given object is the same instance.
     * @param other The object to compare against.
     * @return True if the object is the same as this instance.
     */
    virtual bool isSame(const T* other) const { return this == other; }
};

}

#endif // __CLONABLE_H__