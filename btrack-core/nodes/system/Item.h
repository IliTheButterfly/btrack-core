#ifndef __ITEM_H__
#define __ITEM_H__

#include "nodes/system/Clonable.h"
#include "boost/container/small_vector.hpp"
#include <inttypes.h>
#include <boost/static_string.hpp>

namespace btrack::nodes::system {

#ifndef BTRACK_ID_DEFAULT_LENGTH
#define BTRACK_ID_DEFAULT_LENGTH 8
#endif // BTRACK_ID_DEFAULT_LENGTH

using ID_e = uint_fast16_t;
using ID_t = boost::container::small_vector<ID_e, BTRACK_ID_DEFAULT_LENGTH>;

class Item : public Clonable<Item>
{
public:
    virtual const ID_t& id() const = 0;
    virtual std::string_view name() const = 0;
    virtual std::string& name() = 0;
    virtual std::string_view description() const = 0;
    virtual std::string& description() = 0;
};

}

#endif // __ITEM_H__