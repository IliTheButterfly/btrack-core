#ifndef __PASSTHROUGHPORT_H__
#define __PASSTHROUGHPORT_H__

#include "nodes/system/Port.h"
#include "memory_backend.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class PassthroughPort : public Port<VariantType>
{
private:
    PortBase<VariantType>* mSource = nullptr;
    boost::container::vector<PortBase<VariantType>*> mDestinations;
    VariantType mDefault;
    volatile bool mConnecting = false;
    PortType mType;
public:
    PassthroughPort() : PassthroughPort::Port() { }
    PassthroughPort(NodeBase<VariantType>* _parent, const ID_e& _id, PortType _type, const std::string& _name, const std::string& _description = "", VariantType _default = VariantType())
        : PassthroughPort::Port(_parent, _id, _name, _description), mDefault(_default), mType(_type) { }
    const VariantType& get() const override;
    VariantType& get() override;
    Item* createClone() const override;
    size_t connectionCount() const override { return (mSource ? 1 : 0) + mDestinations.size(); }
    const PortBase<VariantType>* connectionAt(const ID_e& _id) const override {
        return _id == mDestinations.size() ?
            mSource :
            _id > mDestinations.size() ?
                nullptr :
                mDestinations.at(_id);
    }
    PortType type() const override { return mType; }
    bool isPassthrough() const override { return true; }
    ConnectionResult connect(PortBase<VariantType>* other) override;
    ConnectionResult disconnect(PortBase<VariantType>* other) override;

    virtual ~PassthroughPort();
};

template <VariantTemplate VariantType>
inline const VariantType &PassthroughPort<VariantType>::get() const
{
    if (!mSource) return mDefault;
    return mSource->get();
}

template <VariantTemplate VariantType>
inline VariantType &PassthroughPort<VariantType>::get()
{
    if (!mSource) return mDefault;
    return mSource->get();
}
template <VariantTemplate VariantType>
inline ConnectionResult PassthroughPort<VariantType>::connect(PortBase<VariantType>* other)
{
    if (mConnecting) return ConnectionResult::OTHER;
    Recursion r(mConnecting);
    if (!other) return ConnectionResult::NULL_POINTER;
    PortType otherPortType = other->type();
    if (otherPortType == PortType::UNKNOWN) return ConnectionResult::UNHANDLED;
    if (other->isPassthrough())
    {
        // other is a passthrough so we need to determine whether it acts as an input or an output
        // If other is contained in parent, then it acts as it says, otherwise, it acts as the opposite
        bool found = false;
        for (auto i = 0; this->parent()->at(i) != nullptr; ++i)
        {
            const auto* n = dynamic_cast<const NodeBase<VariantType>*>(this->parent()->at(i));
            if (std::find(n->pbegin(), n->pend(), other) != n->pend())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (otherPortType == PortType::INPUT) otherPortType = PortType::OUTPUT;
            else otherPortType = PortType::INPUT;
        }
    }
    if (otherPortType == PortType::OUTPUT) // Acting as an input
    {
        if (mSource == other) return ConnectionResult::ALREADY_CONNECTED;
        if (mSource)
        {
            if (auto r = mSource->disconnect(this); r != ConnectionResult::SUCCESS) return r;
            mSource = nullptr;
        }
        if (auto rr = other->connect(this); rr == ConnectionResult::SUCCESS || rr == ConnectionResult::ALREADY_CONNECTED || rr == ConnectionResult::OTHER)
        {
            mSource = other;
            return ConnectionResult::SUCCESS;
        }
        else return rr;
    }
    if (otherPortType == PortType::INPUT) // Acting as an output
    {
        if (auto r = other->connect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::OTHER)
        {
            mDestinations.emplace_back(other);
            return ConnectionResult::SUCCESS;
        }
        else return r;
    }
    return ConnectionResult::UNHANDLED;
}
template <VariantTemplate VariantType>
inline ConnectionResult PassthroughPort<VariantType>::disconnect(PortBase<VariantType>* other)
{
    if (!other) return ConnectionResult::NULL_POINTER;
    PortType otherPortType = other->type();
    if (otherPortType == PortType::UNKNOWN) return ConnectionResult::UNHANDLED;
    if (other->isPassthrough())
    {
        // other is a passthrough so we need to determine whether it acts as an input or an output
        // If other is contained in parent, then it acts as it says, otherwise, it acts as the opposite
        bool found = false;
        for (auto i = 0; this->parent()->at(i) != nullptr; ++i)
        {
            const auto* n = dynamic_cast<const NodeBase<VariantType>*>(this->parent()->at(i));
            if (std::find(n->pbegin(), n->pend(), other) != n->pend())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (otherPortType == PortType::INPUT) otherPortType = PortType::OUTPUT;
            else otherPortType = PortType::INPUT;
        }
    }
    if (otherPortType == PortType::OUTPUT) // Acting as an input
    {
        if (mSource == other)
        {
            if (auto r = mSource->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED)
            {
                mSource = nullptr;
                return ConnectionResult::SUCCESS;
            }
            else return r;
        }
        return ConnectionResult::NOT_CONNECTED;
    }
    if (otherPortType == PortType::INPUT) // Acting as an output
    {
        if (auto it = std::find(mDestinations.begin(), mDestinations.end(), other); it == mDestinations.end()) return ConnectionResult::NOT_CONNECTED;
        else
        {
            mDestinations.erase(it);
            if (auto r = other->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED) return ConnectionResult::SUCCESS;
            else return r;
        }
    }
    return ConnectionResult::NOT_CONNECTED;
}
template <VariantTemplate VariantType>
inline Item* PassthroughPort<VariantType>::createClone() const
{
    auto res = new PassthroughPort<VariantType>();
    Port<VariantType>::clone(res);
    return res;
}
template <VariantTemplate VariantType>
inline PassthroughPort<VariantType>::~PassthroughPort()
{
    if (mSource)
    {
        mSource->disconnect(this);
        mSource = nullptr;
    }
    for (auto o : mDestinations)
    {
        o->disconnect(this);
    }
}
}

#endif // __PASSTHROUGHPORT_H__