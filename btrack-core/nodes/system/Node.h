#ifndef __NODE_H__
#define __NODE_H__


#include "nodes/system/Port.h"
#include "nodes/system/Item.h"
#include "nodes/system/Input.h"
#include <boost/container/vector.hpp>

namespace btrack::nodes::system {

template <typename ..._Types>
class Node : public Item
{
protected:
    boost::container::vector<shared_ptr<Port<..._Types>> mPorts;
public:
    virtual void run() = 0;
};

} // namespace btrack::nodes::system
#endif // __NODE_H__