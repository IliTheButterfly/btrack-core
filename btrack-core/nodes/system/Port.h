#ifndef __PORT_H__
#define __PORT_H__

#include <Variant.h>
#include "nodes/system/Item.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class NodeBase;

struct Recursion
{
    volatile bool& var;
    Recursion(volatile bool& v) : var(v) { var = true; }
    ~Recursion() { var = false; }
};

enum class PortType {
    UNKNOWN,
    INPUT,
    OUTPUT,
};
enum class PassthroughType {
    NOT_PASSTHROUGH,
    INNER,
    OUTTER,
};
enum class ConnectionResult {
    UNHANDLED,
    SUCCESS,
    NULL_POINTER,
    ALREADY_CONNECTED,
    NOT_CONNECTED,
    INCOMPATIBLE,
    OTHER,
};

template <VariantTemplate VariantType>
class PortBase : public Item
{
protected:
    std::ostream& writeTo(std::ostream& os) const override
    {
        Item::writeTo(os) << "Port type: ";
        switch (this->type())
        {
            case PortType::INPUT:
                os << "Input";
            break;
            case PortType::OUTPUT:
                os << "Output";
            break;
            default:
                os << "Unknown";
            break;
        }
        os << std::endl <<
        "Parent: ";
        if (parent())
        {
            os << "\tID: " << parent()->id() << std::endl <<
            "\tName: " << parent()->name() << std::endl;
        }
        else os << "nullptr" << std::endl;
        os << "Connections: " << connectionCount() << std::endl;
        if (isPassthrough()) os << "Is a passthrough" << std::endl;
        if (isSplitter()) os << "Is a splitter" << std::endl << "Spread count: " << spreadCount() << std::endl;
        if (isMerger()) os << "Is a merger" << std::endl << "Spread count: " << spreadCount() << std::endl;
        return os;
    }
public:
    virtual const VariantType& get() const = 0;
    virtual VariantType& get() = 0;
    virtual PortType type() const = 0;
    virtual PassthroughType passthroughType() const { return PassthroughType::NOT_PASSTHROUGH; }
    virtual ConnectionResult connect(PortBase* other) = 0;
    virtual ConnectionResult disconnect(PortBase* other) = 0;
    virtual NodeBase<VariantType>* parent() = 0;
    virtual const NodeBase<VariantType>* parent() const = 0;
    virtual const PortBase<VariantType>* connectionAt(const ID_e& _id) const = 0;
    virtual size_t connectionCount() const = 0;
    bool isPort() const override { return true; }
    virtual bool isPassthrough() const { return false; }
    virtual bool isSplitter() const { return false; }
    virtual bool isMerger() const { return false; }
    virtual int spreadCount() const { return -1; }
    virtual bool setSpreadCount(int count) { return false; }
    virtual bool isConnectedTo(const PortBase* other) const
    {
        if (!other) return false;
        for (ID_e i = 0; i < other->connectionCount(); ++i)
        {
            if (other->connectionAt(i) == this) return true;
        }
        return false;
    }
    void clone(Item* to) const override 
    {
        PortBase* port = dynamic_cast<PortBase*>(to);
        if (!port) return;
        port->get() = this->get();
    }
    virtual ~PortBase() = default;

    friend NodeBase<VariantType>;
};

template <VariantTemplate VariantType>
class Port : public PortBase<VariantType>
{
private:
    NodeBase<VariantType>* mParent = nullptr;
    ID_e mID;
    std::string mName;
    std::string mDescription;
protected:
    ID_e& _id() override { return mID; }
    Port() = default;
    Port(NodeBase<VariantType>* _parent, const ID_e& _id, const std::string& _name, const std::string& _description = "")
        : mParent(_parent), mID(_id), mName(_name), mDescription(_description) {}
public:
    const ID_e& id() const override { return mID; }
    std::string_view name() const override { return mName; }
    std::string& name() override { return mName; }
    std::string_view description() const override { return mDescription; }
    std::string& description() override { return mDescription; }
    NodeBase<VariantType>* parent() override { return mParent; }
    const NodeBase<VariantType>* parent() const override { return mParent; }
    virtual ~Port() = default;
};

template <VariantTemplate VariantType, std::derived_from<PortBase<VariantType>>>
class PortDecorator : public PortBase<VariantType>
{
protected:
    PortBase<VariantType>* mInnerPort = nullptr;
public:
    const VariantType& get() const override { return mInnerPort->get(); }
    VariantType& get() override { return mInnerPort->get(); }
    PortType type() const override { return mInnerPort->type(); }
    ConnectionResult connect(PortBase<VariantType>* other) override { return mInnerPort->connect(other); }
    ConnectionResult disconnect(PortBase<VariantType>* other) override { return mInnerPort->disconnect(other); }
    NodeBase<VariantType>* parent() override { return mInnerPort->parent(); }
    const NodeBase<VariantType>* parent() const override { return mInnerPort->parent(); }
    const PortBase<VariantType>* connectionAt(const ID_e& _id) const override { return mInnerPort->connectionAt(_id); }
    size_t connectionCount() const override { return mInnerPort->connectionCount(); }
    virtual bool isPassthrough() const { return mInnerPort->isPassthrough(); }
    virtual bool isSplitter() const { return mInnerPort->isSplitter(); }
    virtual bool isMerger() const { return mInnerPort->isMerger(); }
    virtual int spreadCount() const { return mInnerPort->isMerger(); }
    virtual bool setSpreadCount(int count) { return mInnerPort->setSpreadCount(count); }
    void clone(Item* to) const override 
    {
        PortBase<VariantType>* port = dynamic_cast<PortBase<VariantType>*>(to);
        if (!port) return;
        port->get() = this->get();
    }
    virtual ~PortDecorator() = default;
};

}

#endif // __PORT_H__