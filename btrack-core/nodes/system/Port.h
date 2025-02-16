#ifndef __PORT_H__
#define __PORT_H__

#include <Variant.h>
#include "nodes/system/Item.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class NodeBase;

enum class PortType {
    UNKNOWN,
    INPUT,
    OUTPUT,
};
enum class ConnectionResult {
    UNHANDLED,
    SUCCESS,
    NULL_POINTER,
    ALREADY_CONNECTED,
    NOT_CONNECTED,
    OTHER,
};

template <VariantTemplate VariantType>
class PortBase : public Item
{
public:
    virtual const VariantType& get() const = 0;
    virtual VariantType& get() = 0;
    virtual PortType type() const = 0;
    virtual ConnectionResult connect(PortBase* other) = 0;
    virtual ConnectionResult disconnect(PortBase* other) = 0;
    virtual NodeBase<VariantType>* parent() = 0;
    virtual const NodeBase<VariantType>* parent() const = 0;
    void update() override { id() = ((const NodeBase<VariantType>*)parent())->id().child(id().index(), true).get(); }
    virtual ~PortBase() = default;

    friend NodeBase<VariantType>;
};

template <VariantTemplate VariantType>
class Port : public PortBase<VariantType>
{
private:
    NodeBase<VariantType>* mParent = nullptr;
    ID_t mID;
    std::string mName;
    std::string mDescription;
protected:
    ID_t& id() override { return mID; }

    Port(NodeBase<VariantType>* _parent, const ID_t& _id, const std::string& _name, const std::string& _description = "")
        : mParent(_parent), mID(_id), mName(_name), mDescription(_description) {}
public:
    const ID_t& id() const override { return mID; }
    std::string_view name() const override { return mName; }
    std::string& name() override { return mName; }
    std::string_view description() const override { return mDescription; }
    std::string& description() override { return mDescription; }

    NodeBase<VariantType>* parent() override { return mParent; }
    const NodeBase<VariantType>* parent() const override { return mParent; }
    virtual ~Port() = default;
};

}

#endif // __PORT_H__