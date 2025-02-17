#ifndef __ITEM_H__
#define __ITEM_H__

#include "nodes/system/Clonable.h"
#include "boost/container/small_vector.hpp"
#include <inttypes.h>
#include <boost/static_string.hpp>
#include "nodes/metadata.h"
#include <boost/optional.hpp>

namespace btrack::nodes::system {

#ifndef BTRACK_ID_DEFAULT_LENGTH
#define BTRACK_ID_DEFAULT_LENGTH 8
#endif // BTRACK_ID_DEFAULT_LENGTH

using ID_e = uint_fast16_t;
using ID_v = boost::container::small_vector<ID_e, BTRACK_ID_DEFAULT_LENGTH>;

struct ItemID
{
    enum Type {
        UNKNOWN,
        PORT,
        NODE,
    };

    Type type;
    ID_v id;

    explicit ItemID(const Type& _type, const ID_v& _id) : type(_type), id(_id) {}
    explicit ItemID(const Type& _type, const ID_v& _id, const ID_e& _index) : type(_type), id(_id) { id.emplace_back(_index); }
    ItemID(const ItemID&) = default;
    ItemID(ItemID&&) = default;
    void operator=(const ItemID& other)
    {
        type = other.type;
        id = ID_v(other.id.begin(), other.id.end());
    }

    void operator=(ItemID&& other)
    {
        type = other.type;
        id = ID_v(other.id.begin(), other.id.end());
    }

    bool operator==(const ItemID& other) const { return this->type == other.type && this->id == other.id; }
    bool operator!=(const ItemID& other) const { return this->type != other.type || this->id != other.id; }
    boost::optional<ItemID> parent() const
    {
        if (id.size() == 1) return {};
        return ItemID(type == PORT ? NODE : PORT, ID_v(id.begin(), id.begin() + (id.size() - 1)));
    }
    boost::optional<ItemID> child(const ID_e& _id, const bool& port) const
    {
        if (type == PORT) return {};
        return ItemID(port ? PORT : type, id, _id);
    }
    bool isNode() const { return type == NODE; }
    bool isPort() const { return type == PORT; }
    bool isUnknown() const { return type == UNKNOWN; }
    ID_e index() const { return id.size() ? id.at(id.size() - 1) : 0; }

    const ID_e& operator[](const size_t& index) const { return id.at(index); }
    ID_e& operator[](const size_t& index) { return id.at(index); }

    ItemID operator+(const ItemID& other) const 
    {
        auto i = id;
        for (auto d : other.id) { i.emplace_back(d); }
        return ItemID(other.type == PORT ? PORT : type, i);
    }

    ItemID uncheckedSub(const ItemID& other) const
    {
        return ItemID(type, ID_v(id.begin() + other.id.size(), id.end()));
    }
};

using ID_t = ItemID;



class Item : public Clonable<Item>
{
protected:
    virtual ID_t& id() = 0;
public:
    virtual void update() {}
    virtual const ID_t& id() const = 0;
    virtual std::string_view name() const = 0;
    virtual std::string& name() = 0;
    virtual std::string_view description() const = 0;
    virtual std::string& description() = 0;

    void clone(Item* to) const override
    {
        if (!to) return;
        to->id() = this->id();
        to->name() = this->name();
        to->description() = this->description();
    }

    friend struct NodeRegister;
};

struct NodeRegister
{
    static ID_t& id(Item& item) { return item.id(); }
};

}

#endif // __ITEM_H__