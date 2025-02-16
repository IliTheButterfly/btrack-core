#ifndef __NODEREGISTER_H__
#define __NODEREGISTER_H__

#include "Variant.h"
#include "nodes/system/Item.h"
#include <boost/unordered_map.hpp>

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class Node;
template <VariantTemplate VariantType>
class Port;

class _NodeToolset
{
private:
    boost::unordered_map<ID_t, Item> mItems;
public:
    constexpr virtual std::type_index variantType() const = 0;
};

template <VariantTemplate VariantType>
class NodeToolset : public _NodeToolset
{
public:
    constexpr std::type_index variantType() const override { return std::type_index(typeid(VariantType)); }
};

class NodeRegisterClass
{
public:
    template <VariantTemplate VariantType>
    NodeToolset<VariantType>& registerToolset();

};

extern NodeRegisterClass NodeRegister;

}

#endif // __NODEREGISTER_H__